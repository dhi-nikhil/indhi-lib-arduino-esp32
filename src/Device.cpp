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

static const char * TAG = "Device";

void Device::DeviceEventHandler(void * handlerArguments, esp_event_base_t base, int32_t id, void * eventData)
{
    Device * device = (Device *) handlerArguments;
    ESP_LOGI(TAG, "Device ID : %s", device->mName.c_str());
    ESP_LOGI(TAG, "Event Base : %s, Event Id : %d", base, id);
    /* Send data from here to respective callbacks */

    if (strcmp(base, device->mName.c_str()) == 0)
    {
        ESP_LOGW(TAG, "This is device event");
        switch (id)
        {
            case Device::DEVICE_EVENT_DATA_RECEIVE:
            {
                MQTTData * data = (MQTTData *) eventData;

                std::string Data = std::string(data->data, data->data_len);
                std::string Topic = std::string(data->topic, data->topic_len);

                ESP_LOGI(TAG, "data : %s", Data.c_str());
                ESP_LOGI(TAG, "topic : %s", Topic.c_str());

                MessageParse * message = new MessageParse();
                message->FromString(Data);

                /* Check if the data is Device data */
                if (Topic == device->mDeviceSubTopic)
                {
                    switch (message->mCommandType)
                    {
                        case Command::CommandType::COMMAND_TYPE_DECONFIGURE:
                        {
                            ESP_LOGI(TAG, "Deconfigure message");
                            device->EraseData();
                            message->mCommandType = Command::CommandType::COMMAND_TYPE_ACKNOWLEDGEMENT;
                            message->mResponseType = Response::ResponseType::RESPONSE_TYPE_SUCCESS;
                            std::string dataData = message->ToString();
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
                    switch (message->mCommandType)
                    {
                        case Command::CommandType::COMMAND_TYPE_CONTROL:
                        {
                            esp_err_t err = ESP_OK;
                            if (message->mDataType == Data::DATA_TYPE_INTEGER)
                            {
                                device->mDataCallback((char *)message->mSlot.c_str(), message->mIntegerData , NULL);
                            }
                            else if (message->mDataType == Data::DATA_TYPE_STRING)
                            {
                                device->mDataCallback((char *)message->mSlot.c_str(), NULL , (char *)message->mStringData.c_str());
                            }
                            else
                            {
                                ESP_LOGE(TAG, "Unknown dataType");
                            }

                            if (err == ESP_OK)
                            {
                                message->mResponseType = Response::ResponseType::RESPONSE_TYPE_SUCCESS;
                            }
                            else
                            {
                                message->mResponseType = Response::ResponseType::RESPONSE_TYPE_FAILED;
                            }

                            message->mCommandType = Command::CommandType::COMMAND_TYPE_ACKNOWLEDGEMENT;

                            std::string dataData = message->ToStringWithKey(message->mSlot);
                            
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
                MQTTData * data = (MQTTData *) eventData;

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
                MQTTData * data = (MQTTData *) eventData;
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

void Device::MQTTEventHandlerDevice(void * handlerArgs, esp_event_base_t base, int32_t id, void * eventData)
{
    Device * device = (Device *) handlerArgs;
    esp_mqtt_event_t * mqttData = (esp_mqtt_event_t *) eventData;
    ESP_LOGI(TAG, "Event Base : %s, Event Id : %d", base, id);
    switch (id)
    {
        case MQTT_EVENT_ANY:
        {

        }
        break;
        case MQTT_EVENT_ERROR:              /*!< on error event, additional context: connection return code, error handle from esp_tls (if supported) */
        {

        }
        break;
        case MQTT_EVENT_CONNECTED:          /*!< connected event, additional context: session_present flag */
        {
            device->MQTTClient::Subscribe(device->mDeviceSubTopic);
            device->MQTTClient::Subscribe(device->mDataSubTopic);

            MessageCreate * message = new MessageCreate();
            message->SetCommandType(Message::CommandType::COMMAND_TYPE_CONNECTED);
            device->MQTTClient::Send(device->mDevicePubTopic, message->ToString(), 0, true);
        }
        break;
        case MQTT_EVENT_DISCONNECTED:       /*!< disconnected event */
        {

        }
        break;
        case MQTT_EVENT_SUBSCRIBED:         /*!< subscribed event, additional context: msg_id */
        {

        }
        break;
        case MQTT_EVENT_UNSUBSCRIBED:       /*!< unsubscribed event */
        {

        }
        break;
        case MQTT_EVENT_PUBLISHED:          /*!< published event, additional context:  msg_id */
        {

        }
        break;
        case MQTT_EVENT_DATA:               /*!< data event, additional context: */
        {
            MQTTData mqttDataObject;
            strncpy(&mqttDataObject.data[0], mqttData->data, mqttData->data_len);
            strncpy(&mqttDataObject.topic[0], mqttData->topic, mqttData->topic_len);
            mqttDataObject.data_len = mqttData->data_len;
            mqttDataObject.topic_len = mqttData->topic_len;

            ESP_ERROR_CHECK_WITHOUT_ABORT(device->Post(device->mName.c_str(), DEVICE_EVENT_DATA_RECEIVE, &mqttDataObject, sizeof(mqttDataObject)));
        }
        break;
        case MQTT_EVENT_BEFORE_CONNECT:     /*!< The event occurs before connecting */
        {

        }
        break;
        case MQTT_EVENT_DELETED:            /*!< Notification on delete of one message from the internal outbox */
        {

        }
        break;
        default:
        {

        }
        break;
    }
}

Device::Device(std::string name, std::string templateId, dataCallback_t dataCallback) : NVStorage(), DefaultEventLoop(), MQTTClient()
{
    mName = name;
    mTemplateId = templateId;
    mDataCallback = dataCallback;
    ESP_LOGI(TAG, "Created Device with name %s", mName.c_str());
    NVStorage::Open("Provision");
    ESP_ERROR_CHECK(Device::RegisterDefaultLoopHandler(mName.c_str(), ESP_EVENT_ANY_ID, DeviceEventHandler, this));
    resetButtonTask = new Task();
    resetButtonTask->SetName("resetButtonTask")->SetStackSize(2048)->SetPriority(5)->SetTaskFunction(ResetButtonTask)->SetContext(this)->Start();
}

void Device::ResetButtonTask(void * arg)
{
    int count = 0;
    gpio_num_t gpio = GPIO_NUM_0;
    Task * taskData = (Task *) arg;
    Device * device = (Device *) taskData->GetContext();
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

    esp_err_t Device::Send(char * slot, double data, int decimalPlaces)
    {
    
        MessageCreate * message = new MessageCreate();

        const double multiplier = std::pow(10.0, decimalPlaces);
        data = std::ceil(data * multiplier) / multiplier;

        message->SetCommandType(Message::CommandType::COMMAND_TYPE_STATUS);
        message->SetData(data);
        std::string slotStr(slot);
        std::string messageString = message->ToStringWithKey(slotStr);
        MQTTData mqttData;
        mqttData.data_len = messageString.length();
        strncpy(&mqttData.data[0], messageString.c_str(), messageString.length());
        return Post(mName.c_str(), Device::DEVICE_SLOT_EVENT_DATA_SEND, &mqttData, sizeof(mqttData));

        end:
        delete message;
    }

    esp_err_t Device::Send(char * slot, char * data)
    {
    
        MessageCreate * message = new MessageCreate();

        message->SetCommandType(Message::CommandType::COMMAND_TYPE_STATUS);
        message->SetData(data);
        std::string slotStr(slot);
        std::string messageString = message->ToStringWithKey(slotStr);
        MQTTData mqttData;
        mqttData.data_len = messageString.length();
        strncpy(&mqttData.data[0], messageString.c_str(), messageString.length());
        return Post(mName.c_str(), Device::DEVICE_SLOT_EVENT_DATA_SEND, &mqttData, sizeof(mqttData));

        end:
        delete message;
    }
