#define LOG_LOCAL_LEVEL ESP_LOG_NONE

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "sdkconfig.h"
#include "Device.h"
#include "WiFi.h"
#include "Message.h"
#include "System.h"
#include "driver/gpio.h"
#include <cmath>
#include "cJSON.h"
#include "HttpsOTAUpdate.h"
#include "PayloadEncryption.h"

static const char *TAG = "Device";

void Device::DeviceEventHandler(void *handlerArguments, esp_event_base_t base, int32_t id, void *eventData)
{
    Device *device = (Device *)handlerArguments;
    ESP_LOGI(TAG, "Event Base : %s, Event Id : %d", base, id);
    /* Send data from here to respective callbacks */

    if (strcmp(base, device->mTemplateId.c_str()) == 0)
    {
        ESP_LOGW(TAG, "This is device event");
        switch (id)
        {
        case Device::DEVICE_EVENT_DATA_RECEIVE:
        {
            MQTTData *data = (MQTTData *)eventData;

            std::string Data = std::string(data->data, data->data_len);
            std::string Topic = std::string(data->topic, data->topic_len);

            ESP_LOGI(TAG, "data : %s", Data.c_str());
            ESP_LOGI(TAG, "topic : %s", Topic.c_str());

            std::string decryptedData = device->DecryptPayload(Data);

            /* Check if the data is Device data */
            if (Topic == device->mDeviceSubTopic)
            {
                switch (device->GetCommandType(decryptedData))
                {
                case Command::CommandType::COMMAND_TYPE_DECONFIGURE:
                {
                    ESP_LOGI(TAG, "Deconfigure message");
                    device->EraseData();
                    std::string resTypeString = "success";
                    std::string dataData = device->cJsonToSendAck(resTypeString, decryptedData);
                    device->MQTTClient::Send(device->mDevicePubTopic, dataData, 0, false);
                }
                break;
                case Command::CommandType::COMMAND_TYPE_UPDATE:
                {
                    ESP_LOGI(TAG, "Update firmware");
                    std::string server_certificate = "";
                    HttpsOTA.begin((char *)device->GetURL(decryptedData).c_str(), server_certificate.c_str(), false); 
                }
                break;
                case Command::CommandType::COMMAND_TYPE_CONFIG:
                {
                    esp_err_t err;

                    std::string var = device->ConfigGetVar(decryptedData);
                    double value = device->GetValueDouble(decryptedData);

                    device->mConfigCallback((char *)var.c_str(),value);

                    err = device->SaveConfig((char *)var.c_str(), value);
                    
                    std::string resTypeString = "";

                    if (err == ESP_OK)
                    {
                        resTypeString = "success";
                    }
                    else
                    {
                        resTypeString = "failed";
                    }

                    std::string dataData = device->cJsonToSendAck(resTypeString, decryptedData);

                    device->MQTTClient::Send(device->mDevicePubTopic, dataData, 0, false);
                }
                break;
                default:
                {
                    ESP_LOGE(TAG, "Invalid message for Device");
                }
                break;
                }
            }
            else
            {
                switch (device->GetCommandType(decryptedData))
                {
                case Command::CommandType::COMMAND_TYPE_CONTROL:
                {
                    esp_err_t err = ESP_OK;
                    if (device->GetDataType(decryptedData) == Data::DATA_TYPE_INTEGER)
                    {
                        device->mDataCallback((char *)device->GetSlot(decryptedData).c_str(), device->GetValueDouble(decryptedData), NULL);
                    }
                    else if (device->GetDataType(decryptedData) == Data::DATA_TYPE_STRING)
                    {
                        device->mDataCallback((char *)device->GetSlot(decryptedData).c_str(), NULL, (char *)device->GetValueStr(decryptedData).c_str());
                    }
                    else
                    {
                        ESP_LOGE(TAG, "Unknown dataType");
                    }

                    std::string resTypeString = "";

                    if (err == ESP_OK)
                    {
                        resTypeString = "success";
                    }
                    else
                    {
                        resTypeString = "failed";
                    }

                    std::string dataData = device->cJsonToSendAck(resTypeString, decryptedData);

                    device->MQTTClient::Send(device->mDataPubTopic, dataData, 0, false);
                }
                break;
                default:
                {
                }
                break;
                }
            }
        }
        break;
        case Device::DEVICE_EVENT_DATA_SEND:
        {
            ESP_LOGI(TAG, "DEVICE_EVENT_DATA_SEND");
            MQTTData *data = (MQTTData *)eventData;

            std::string Data = std::string(data->data, data->data_len);
            std::string Topic = std::string(data->topic, data->topic_len);

            ESP_LOGI(TAG, "data : %s", Data.c_str());
            ESP_LOGI(TAG, "topic : %s", Topic.c_str());

            if (!Topic.empty())
            {
                device->MQTTClient::Send(Topic, Data, 0, false);
            }
        }
        break;
        case Device::DEVICE_EVENT_CONNECTED:
        {
            ESP_LOGI(TAG, "DEVICE_EVENT_CONNECTED");
            // device->MQTTClient::Send(device->mDevicePubTopic, message->ToString());
        }
        break;
        case Device::DEVICE_EVENT_DISCONNECTED:
        {
            ESP_LOGI(TAG, "DEVICE_EVENT_DISCONNECTED");
            // device->MQTTClient::Send(device->mDevicePubTopic, message->ToString());
        }
        break;
        case Device::DEVICE_SLOT_EVENT_DATA_SEND:
        {
            MQTTData *data = (MQTTData *)eventData;
            std::string dataData = std::string(data->data, data->data_len);
            device->MQTTClient::Send(device->mDataPubTopic, dataData, 0, false);
        }
        break;
        case Device::DEVICE_SLOT_EVENT_DATA_RECEIVE:
        {
            ESP_LOGI(TAG, "DEVICE_EVENT_DISCONNECTED");
        }
        break;
        default:
        {
        }
        break;
        }
    }
}

void Device::MQTTEventHandlerDevice(void *handlerArgs, esp_event_base_t base, int32_t id, void *eventData)
{
    Device *device = (Device *)handlerArgs;
    esp_mqtt_event_t *mqttData = (esp_mqtt_event_t *)eventData;
    ESP_LOGI(TAG, "Event Base : %s, Event Id : %d", base, id);
    switch (id)
    {
    case MQTT_EVENT_ANY:
    {
    }
    break;
    case MQTT_EVENT_ERROR: /*!< on error event, additional context: connection return code, error handle from esp_tls (if supported) */
    {
    }
    break;
    case MQTT_EVENT_CONNECTED: /*!< connected event, additional context: session_present flag */
    {
        device->MQTTClient::Subscribe(device->mDeviceSubTopic);
        device->MQTTClient::Subscribe(device->mDataSubTopic);

        MessageCreate *message = new MessageCreate();
        message->SetCommandType(Message::CommandType::COMMAND_TYPE_CONNECTED);
        device->MQTTClient::Send(device->mDevicePubTopic, message->ToString(), 0, true);
    }
    break;
    case MQTT_EVENT_DISCONNECTED: /*!< disconnected event */
    {
    }
    break;
    case MQTT_EVENT_SUBSCRIBED: /*!< subscribed event, additional context: msg_id */
    {
    }
    break;
    case MQTT_EVENT_UNSUBSCRIBED: /*!< unsubscribed event */
    {
    }
    break;
    case MQTT_EVENT_PUBLISHED: /*!< published event, additional context:  msg_id */
    {
    }
    break;
    case MQTT_EVENT_DATA: /*!< data event, additional context: */
    {
        MQTTData mqttDataObject;
        strncpy(&mqttDataObject.data[0], mqttData->data, mqttData->data_len);
        strncpy(&mqttDataObject.topic[0], mqttData->topic, mqttData->topic_len);
        mqttDataObject.data_len = mqttData->data_len;
        mqttDataObject.topic_len = mqttData->topic_len;

        ESP_ERROR_CHECK_WITHOUT_ABORT(device->Post(device->mTemplateId.c_str(), DEVICE_EVENT_DATA_RECEIVE, &mqttDataObject, sizeof(mqttDataObject)));
    }
    break;
    case MQTT_EVENT_BEFORE_CONNECT: /*!< The event occurs before connecting */
    {
    }
    break;
    case MQTT_EVENT_DELETED: /*!< Notification on delete of one message from the internal outbox */
    {
    }
    break;
    default:
    {
    }
    break;
    }
}

Device::Device(std::string templateId, dataCallback_t dataCallback, configCallback_t configCallback) : NVStorage(), DefaultEventLoop(), MQTTClient()
{
    mTemplateId = templateId;
    mDataCallback = dataCallback;
    mConfigCallback = configCallback;
    NVStorage::Open("Provision");
    ESP_ERROR_CHECK(Device::RegisterDefaultLoopHandler(mTemplateId.c_str(), ESP_EVENT_ANY_ID, DeviceEventHandler, this));
    resetButtonTask = new Task();
    resetButtonTask->SetName("resetButtonTask")->SetStackSize(2048)->SetPriority(5)->SetTaskFunction(ResetButtonTask)->SetContext(this)->Start();
}

void Device::ResetButtonTask(void *arg)
{
    int count = 0;
    gpio_num_t gpio = GPIO_NUM_0;
    Task *taskData = (Task *)arg;
    Device *device = (Device *)taskData->GetContext();
    gpio_set_direction(gpio, GPIO_MODE_INPUT);
    while (true)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
        int level = gpio_get_level(gpio);
        if (level == 0)
        {
            count++;
        }
        else
        {
            count = 0;
        }
        if (count >= 5)
        {
            device->EraseData();
            vTaskDelay(pdMS_TO_TICKS(1000));
            esp_restart();
        }
    }
    vTaskDelete(NULL);
}

esp_err_t Device::Send(char *slot, double data, int decimalPlaces)
{

    const double multiplier = std::pow(10.0, decimalPlaces);
    data = std::ceil(data * multiplier) / multiplier;

    std::string slotStr(slot);
    std::string messageString = cJsonToSendDouble(slotStr, data);
    MQTTData mqttData;
    mqttData.data_len = messageString.length();
    strncpy(&mqttData.data[0], messageString.c_str(), messageString.length());
    return Post(mTemplateId.c_str(), Device::DEVICE_SLOT_EVENT_DATA_SEND, &mqttData, sizeof(mqttData));
}

esp_err_t Device::Send(char *slot, char *data)
{

    std::string slotStr(slot);
    std::string messageString = cJsonToSendStr(slotStr, data);
    MQTTData mqttData;
    mqttData.data_len = messageString.length();
    strncpy(&mqttData.data[0], messageString.c_str(), messageString.length());
    return Post(mTemplateId.c_str(), Device::DEVICE_SLOT_EVENT_DATA_SEND, &mqttData, sizeof(mqttData));
}

std::string Device::cJsonToSendStr(std::string key, char *data)
{
    cJSON *mObj, *eObj;

    std::string stringified = "";
    std::string commandTypeString = "status";
    std::string encryptedData = "";
    std::string iv = "";

    char *str;

    mObj = cJSON_CreateObject();
    if (mObj == NULL)
    {
        ESP_LOGE(TAG, "Error Creating Object mObj");
        goto end;
    }

    if (!commandTypeString.empty())
    {
        cJSON_AddStringToObject(mObj, "command_type", commandTypeString.c_str());
        cJSON_AddStringToObject(mObj, "slot", key.c_str());
        cJSON_AddStringToObject(mObj, "value", data);
    }

    str = cJSON_PrintUnformatted(mObj);
    stringified = std::string(str);
    free(str);

    PayloadEncryption::EncryptCBC(&encryptedData, &iv, stringified);

    eObj = cJSON_CreateObject();
    cJSON_AddStringToObject(eObj, "iv", iv.c_str());
    cJSON_AddStringToObject(eObj, "eData", encryptedData.c_str());

    str = cJSON_PrintUnformatted(eObj);
    stringified = std::string(str);
    free(str);

    cJSON_Delete(eObj);
end:
    cJSON_Delete(mObj);
    return stringified;
}

std::string Device::cJsonToSendDouble(std::string key, double data)
{
    cJSON *mObj, *eObj;

    std::string stringified = "";
    std::string commandTypeString = "status";
    std::string encryptedData = "";
    std::string iv = "";

    char *str;

    mObj = cJSON_CreateObject();
    if (mObj == NULL)
    {
        ESP_LOGE(TAG, "Error Creating Object mObj");
        goto end;
    }

    if (!commandTypeString.empty())
    {
        cJSON_AddStringToObject(mObj, "command_type", commandTypeString.c_str());
        cJSON_AddStringToObject(mObj, "slot", key.c_str());
        cJSON_AddNumberToObject(mObj, "value", data);
    }

    str = cJSON_PrintUnformatted(mObj);
    stringified = std::string(str);
    free(str);

    PayloadEncryption::EncryptCBC(&encryptedData, &iv, stringified);

    eObj = cJSON_CreateObject();
    cJSON_AddStringToObject(eObj, "iv", iv.c_str());
    cJSON_AddStringToObject(eObj, "eData", encryptedData.c_str());

    str = cJSON_PrintUnformatted(eObj);
    stringified = std::string(str);
    free(str);

    cJSON_Delete(eObj);
end:
    cJSON_Delete(mObj);
    return stringified;
}

std::string Device::cJsonToSendAck(std::string responseType, std::string data)
{
    cJSON *mObj, *eObj;

    std::string stringified = "";
    std::string encryptedData = "";
    std::string iv = "";

    char *str;

    mObj = cJSON_CreateObject();
    if (mObj == NULL)
    {
        ESP_LOGE(TAG, "Error Creating Object mObj");
        goto end;
    }

    cJSON_AddStringToObject(mObj, "command_type", "acknowledgement");
    cJSON_AddStringToObject(mObj, "response_status", responseType.c_str());
    cJSON_AddStringToObject(mObj, "message_id", GetMessageId(data).c_str());

    if (GetCommandType(data) != Command::CommandType::COMMAND_TYPE_DECONFIGURE && GetCommandType(data) != Command::CommandType::COMMAND_TYPE_ACTIVATION)
        {
            cJSON_AddStringToObject(mObj, "slot", GetSlot(data).c_str());
            if (GetDataType(data) == Data::DATA_TYPE_INTEGER)
            {
                cJSON_AddNumberToObject(mObj, "value", GetValueDouble(data));
            }
            else if (GetDataType(data) == Data::DATA_TYPE_STRING)
            {
                cJSON_AddStringToObject(mObj, "value", GetValueStr(data).c_str());
            }
            else
            {
            }
        }

    str = cJSON_PrintUnformatted(mObj);
    stringified = std::string(str);
    free(str);

    PayloadEncryption::EncryptCBC(&encryptedData, &iv, stringified);

    eObj = cJSON_CreateObject();
    cJSON_AddStringToObject(eObj, "iv", iv.c_str());
    cJSON_AddStringToObject(eObj, "eData", encryptedData.c_str());

    str = cJSON_PrintUnformatted(eObj);
    stringified = std::string(str);
    free(str);

    cJSON_Delete(eObj);
end:
    cJSON_Delete(mObj);
    return stringified;
}

std::string Device::DecryptPayload(std::string data)
{

    cJSON *mObj;
    std::string decryptedData = "";

    mObj = cJSON_Parse(data.c_str());
    if (mObj == NULL)
    {
        ESP_LOGE(TAG, "Error Parsing Object mObj");
        goto end;
    }

    if (cJSON_IsString(cJSON_GetObjectItemCaseSensitive(mObj, "iv")) && cJSON_IsString(cJSON_GetObjectItemCaseSensitive(mObj, "eData")))
    {
        ESP_LOGI(TAG, "Message is Encrypted");
        PayloadEncryption::DecryptCBC(&decryptedData,
                                      std::string(cJSON_GetStringValue(cJSON_GetObjectItemCaseSensitive(mObj, "iv"))),
                                      std::string(cJSON_GetStringValue(cJSON_GetObjectItemCaseSensitive(mObj, "eData"))));
        ESP_LOGI(TAG, "Decrypted data : %s", decryptedData.c_str());
    }
    else
    {
        ESP_LOGE(TAG, "Message is invalid");
        goto end;
    }
end:
    cJSON_Delete(mObj);
    return decryptedData;
}

Message::DataType Device::GetDataType(std::string data)
{
    cJSON *eObj, *_data;

    eObj = cJSON_Parse(data.c_str());

    if (eObj == NULL)
    {
        ESP_LOGE(TAG, "Error Parsing data");
        goto end;
    }

    _data = cJSON_GetObjectItemCaseSensitive(eObj, "value");

    if (_data)
    {
        if (cJSON_IsNumber(_data))
        {
            return Message::DataType::DATA_TYPE_INTEGER;
        }
        else if (cJSON_IsString(_data))
        {
            return Message::DataType::DATA_TYPE_STRING;
        }
        else
        {
            return Message::DataType::DATA_TYPE_INVALID;
        }
    }
    else
    {
        return Message::DataType::DATA_TYPE_NONE;
    }
end:
    cJSON_Delete(eObj);
}

Message::Command::CommandType Device::GetCommandType(std::string data)
{
    cJSON *eObj, *commandType;
    std::string commandTypeString;

    Command::CommandType mCommandType;

    eObj = cJSON_Parse(data.c_str());

    if (eObj == NULL)
    {
        ESP_LOGE(TAG, "Error Parsing data");
        goto end;
    }

    commandType = cJSON_GetObjectItemCaseSensitive(eObj, "command_type");
    if (commandType)
    {
        commandTypeString = std::string(cJSON_GetStringValue(commandType));
    }

    if (commandTypeString == "activation")
    {
        mCommandType = Command::CommandType::COMMAND_TYPE_ACTIVATION;
    }
    else if (commandTypeString == "update")
    {
        mCommandType = Command::CommandType::COMMAND_TYPE_UPDATE;
    }
    else if (commandTypeString == "control")
    {
        mCommandType = Command::CommandType::COMMAND_TYPE_CONTROL;
    }
    else if (commandTypeString == "status")
    {
        mCommandType = Command::CommandType::COMMAND_TYPE_STATUS;
    }
    else if (commandTypeString == "deconfigure")
    {
        mCommandType = Command::CommandType::COMMAND_TYPE_DECONFIGURE;
    }
    else if (commandTypeString == "delete") //  not used
    {
        mCommandType = Command::CommandType::COMMAND_TYPE_DELETE;
    }
    else if (commandTypeString == "disconnected")
    {
        mCommandType = Command::CommandType::COMMAND_TYPE_DISCONNECTED;
    }
    else
    {
        mCommandType = Command::CommandType::COMMAND_TYPE_INVALID;
        ESP_LOGE(TAG, "Invalid commandTypeString");
    }

end:
    cJSON_Delete(eObj);
    return mCommandType;
}

std::string Device::GetSlot(std::string data)
{
    cJSON *eObj, *_slot;
    std::string mSlot;

    eObj = cJSON_Parse(data.c_str());

    if (eObj == NULL)
    {
        ESP_LOGE(TAG, "Error Parsing data");
        goto end;
    }

    _slot = cJSON_GetObjectItemCaseSensitive(eObj, "slot");

    if (_slot)
    {
        mSlot = std::string(cJSON_GetStringValue(_slot));
    }

end:
    cJSON_Delete(eObj);
    return mSlot;
}

std::string Device::GetValueStr(std::string data)
{
    cJSON *eObj, *_value;
    std::string mValue;

    eObj = cJSON_Parse(data.c_str());

    if (eObj == NULL)
    {
        ESP_LOGE(TAG, "Error Parsing data");
        goto end;
    }

    _value = cJSON_GetObjectItemCaseSensitive(eObj, "value");

    if (_value)
    {
        mValue = std::string(cJSON_GetStringValue(_value));
    }

end:
    cJSON_Delete(eObj);
    return mValue;
}

double Device::GetValueDouble(std::string data){
    cJSON *eObj, *_value;
    double mValue;

    eObj = cJSON_Parse(data.c_str());

    if (eObj == NULL)
    {
        ESP_LOGE(TAG, "Error Parsing data");
        goto end;
    }

    _value = cJSON_GetObjectItemCaseSensitive(eObj, "value");

    if (_value)
    {
        mValue = cJSON_GetNumberValue(_value);
    }

end:
    cJSON_Delete(eObj);
    return mValue;
}

std::string Device::GetMessageId(std::string data)
{
    cJSON *eObj, *messageId;
    std::string mMessageId;

    eObj = cJSON_Parse(data.c_str());

    if (eObj == NULL)
    {
        ESP_LOGE(TAG, "Error Parsing data");
        goto end;
    }

    messageId = cJSON_GetObjectItemCaseSensitive(eObj, "message_id");

    if (messageId)
    {
        mMessageId = std::string(cJSON_GetStringValue(messageId));
    }

end:
    cJSON_Delete(eObj);
    return mMessageId;
}

std::string Device::GetURL(std::string data)
{
    cJSON *eObj, *_url;
    std::string mURL;

    eObj = cJSON_Parse(data.c_str());

    if (eObj == NULL)
    {
        ESP_LOGE(TAG, "Error Parsing data");
        goto end;
    }

    _url = cJSON_GetObjectItemCaseSensitive(eObj, "url");

    if (_url)
    {
        mURL = std::string(cJSON_GetStringValue(_url));
    }

end:
    cJSON_Delete(eObj);
    return mURL;
}

std::string Device::ConfigGetVar(std::string data)
{
    cJSON *eObj, *_var;
    std::string mVar;

    eObj = cJSON_Parse(data.c_str());

    if (eObj == NULL)
    {
        ESP_LOGE(TAG, "Error Parsing data");
        goto end;
    }

    _var = cJSON_GetObjectItemCaseSensitive(eObj, "namespace");

    if (_var)
    {
        mVar = std::string(cJSON_GetStringValue(_var));
    }

end:
    cJSON_Delete(eObj);
    return mVar;
}

double Device::GetConfig(char * variableName){
    
    NVStorage::Open("Provision");

    double val = NVStorage::getDouble(variableName, 0);

    return val;
}

esp_err_t Device::SaveConfig(char * var, double value){
    
    NVStorage::Open("Provision");

    esp_err_t err = NVStorage::putDouble(var, value);

    if(err != ESP_OK){
        ESP_LOGE(TAG, "add config data fail: %s -- %f", var,value);
    }else{
        ESP_LOGE(TAG, "add config data : %s -- %f", var,value);
    }

    return err;
}

Device *Device::AddConfig(DeviceConfig * config){

    ESP_LOGE(TAG, "add config data : %s -- %f", config->mVar.c_str(),config->mValue);

    mConfigs.push_back(config);
    return this;
}