#define LOG_LOCAL_LEVEL ESP_LOG_NONE

#include "Message.h"
#include "esp_log.h"
#include "cJSON.h"

static const char * TAG = "Message";

bool Message::mIsEncryptionEnable = false;

Message::Message()
{
    mMessageType = MessageType::MESSAGE_TYPE_NONE;
    mCommandType = CommandType::COMMAND_TYPE_NONE;
    mCommandSubType = CommandSubType::COMMAND_SUBTYPE_NONE;
    mResponseType = ResponseType::RESPONSE_TYPE_NONE;
    mStatusType = StatusType::STATUS_TYPE_NONE;
    mServerTimestamp = 0;
    mDeviceTimestamp = 0;
    // mMessageId = "";
}

std::string Message::ToString()
{
    cJSON * mObj, * eObj;

    std::string stringified = "";
    std::string messageTypeString = "";
    std::string commandTypeString = "";
    std::string commandSubTypeString = "";
    std::string responseTypeString = "";
    std::string statusTypeString = "";
    std::string encryptedData = "";
    std::string iv = "";

    char * str;

    mObj = cJSON_CreateObject();
    if (mObj == NULL)
    {
        ESP_LOGE(TAG, "Error Creating Object mObj");
        goto end;
    }

    switch (mMessageType)
    {
        case MESSAGE_TYPE_NONE:
            messageTypeString = "";
            break;
        case MESSAGE_TYPE_CONNECTED:
            messageTypeString = "connected";
            break;
        case MESSAGE_TYPE_DISCONNECTED:
            messageTypeString = "disconnected";
            break;
        case MESSAGE_TYPE_DATA:
            messageTypeString = "data";
            break;
        case MESSAGE_TYPE_INVALID:
            messageTypeString = "invalid";
            break;
        case MESSAGE_TYPE_MAX:
            messageTypeString = "invalid";
            break;
        default:
            messageTypeString = "invalid";
            break;
    }

    switch (mCommandType)
    {
        case COMMAND_TYPE_NONE:
            commandTypeString = "";
            break;
        case COMMAND_TYPE_STATUS:
            commandTypeString = "status";
            break;
        case COMMAND_TYPE_CONTROL:
            commandTypeString = "control";
            break;
        case COMMAND_TYPE_DECONFIGURE:
            commandTypeString = "deconfigure";
            break;
        case COMMAND_TYPE_DELETE:
            commandTypeString = "delete";
            break;
        case COMMAND_TYPE_ACTIVATION:
            commandTypeString = "activation";
            break;
        case COMMAND_TYPE_CONNECTED:
            commandTypeString = "connected";
            break;
        case COMMAND_TYPE_DISCONNECTED:
            commandTypeString = "disconnected";
            break;
        case COMMAND_TYPE_INVALID:
            commandTypeString = "invalid";
            break;
        case COMMAND_TYPE_MAX:
            commandTypeString = "invalid";
            break;
        default:
            commandTypeString = "invalid";
            break;
    }

    switch (mCommandSubType)
    {
        case COMMAND_SUBTYPE_NONE:
            commandSubTypeString = "";
            break;
        case COMMAND_SUBTYPE_ADDITION_CONFIRMATION:
            commandSubTypeString = "additionConfirmation";
            break;
        case COMMAND_SUBTYPE_DECONFIGURE:
            commandSubTypeString = "deconfigure";
            break;
        case COMMAND_SUBTYPE_INVALID:
            commandSubTypeString = "invalid";
            break;
        case COMMAND_SUBTYPE_MAX:
            commandSubTypeString = "invalid";
            break;
        default:
            commandSubTypeString = "invalid";
            break;
    }

    switch (mResponseType)
    {
        case RESPONSE_TYPE_NONE:
            responseTypeString = "";
            break;
        case RESPONSE_TYPE_SUCCESS:
            responseTypeString = "success";
            break;
        case RESPONSE_TYPE_FAILED:
            responseTypeString = "failed";
            break;
        case RESPONSE_TYPE_MAX:
            responseTypeString = "invalid";
            break;
        default:
            responseTypeString = "invalid";
            break;
    }

    switch (mStatusType)
    {
        case STATUS_TYPE_NONE:
            statusTypeString = "";
            break;
        case STATUS_TYPE_ACTIVATION_PENDING:
            statusTypeString = "pending";
            break;
        case STATUS_TYPE_ACTIVATION_TIMEOUT:
            statusTypeString = "timeout";
            break;
        case STATUS_TYPE_INVALID:
            statusTypeString = "invalid";
            break;
        case STATUS_TYPE_MAX:
            statusTypeString = "invalid";
            break;
        default:
            statusTypeString = "invalid";
            break;
    }

    if (responseTypeString.empty())
    {
        if (!messageTypeString.empty())
        {
            cJSON_AddStringToObject(mObj, "message_type", messageTypeString.c_str());
        }

        if (!commandTypeString.empty())
        {
            cJSON_AddStringToObject(mObj, "command_type", commandTypeString.c_str());
            if (mCommandType == COMMAND_TYPE_STATUS)
            {
                if (mDataType == Data::DATA_TYPE_INTEGER)
                {
                    cJSON_AddNumberToObject(mObj, "data", mIntegerData);
                }
                else if (mDataType == Data::DATA_TYPE_STRING)
                {
                    cJSON_AddStringToObject(mObj, "data", mStringData.c_str());
                }
                else
                {
                }

            }
        }

        if (!commandSubTypeString.empty())
        {
            cJSON_AddStringToObject(mObj, "command_subtype", commandSubTypeString.c_str());
        }

        if (!statusTypeString.empty())
        {
            cJSON_AddStringToObject(mObj, "status", statusTypeString.c_str());
        }
    }
    else
    {
        cJSON_AddStringToObject(mObj, "command_type", "acknowledgement");
        cJSON_AddStringToObject(mObj, "response_status", responseTypeString.c_str());
        cJSON_AddStringToObject(mObj, "message_id", mMessageId.c_str());
        if (mCommandType != COMMAND_TYPE_DECONFIGURE && mCommandType != COMMAND_TYPE_ACTIVATION)
        {
            if (mDataType == Data::DATA_TYPE_INTEGER)
            {
                cJSON_AddNumberToObject(mObj, "data", mIntegerData);
            }
            else if (mDataType == Data::DATA_TYPE_STRING)
            {
                cJSON_AddStringToObject(mObj, "data", mStringData.c_str());
            }
            else
            {
            }
        }
    }

    str = cJSON_PrintUnformatted(mObj);
    stringified = std::string(str);
    free(str);

    ESP_LOGI(TAG, "Data : %s", stringified.c_str());

    if (IsEncryptionEnable())
    {
        PayloadEncryption::EncryptCBC(&encryptedData, &iv, stringified);

        eObj = cJSON_CreateObject();
        cJSON_AddStringToObject(eObj, "iv", iv.c_str());
        cJSON_AddStringToObject(eObj, "eData", encryptedData.c_str());

        str = cJSON_PrintUnformatted(eObj);
        stringified = std::string(str);
        free(str);

        ESP_LOGI(TAG, "Encrypted Data : %s", stringified.c_str());

        cJSON_Delete(eObj);
    }

end:
    cJSON_Delete(mObj);
    return stringified;
}

std::string Message::ToStringWithKey(std::string key)
{
    cJSON * mObj, * eObj;

    std::string stringified = "";
    std::string messageTypeString = "";
    std::string commandTypeString = "";
    std::string commandSubTypeString = "";
    std::string responseTypeString = "";
    std::string statusTypeString = "";
    std::string encryptedData = "";
    std::string iv = "";

    char * str;

    mObj = cJSON_CreateObject();
    if (mObj == NULL)
    {
        ESP_LOGE(TAG, "Error Creating Object mObj");
        goto end;
    }

    switch (mMessageType)
    {
        case MESSAGE_TYPE_NONE:
            messageTypeString = "";
            break;
        case MESSAGE_TYPE_CONNECTED:
            messageTypeString = "connected";
            break;
        case MESSAGE_TYPE_DISCONNECTED:
            messageTypeString = "disconnected";
            break;
        case MESSAGE_TYPE_DATA:
            messageTypeString = "data";
            break;
        case MESSAGE_TYPE_INVALID:
            messageTypeString = "invalid";
            break;
        case MESSAGE_TYPE_MAX:
            messageTypeString = "invalid";
            break;
        default:
            messageTypeString = "invalid";
            break;
    }

    switch (mCommandType)
    {
        case COMMAND_TYPE_NONE:
            commandTypeString = "";
            break;
        case COMMAND_TYPE_STATUS:
            commandTypeString = "status";
            break;
        case COMMAND_TYPE_CONTROL:
            commandTypeString = "control";
            break;
        case COMMAND_TYPE_DECONFIGURE:
            commandTypeString = "deconfigure";
            break;
        case COMMAND_TYPE_DELETE:
            commandTypeString = "delete";
            break;
        case COMMAND_TYPE_ACTIVATION:
            commandTypeString = "activation";
            break;
        case COMMAND_TYPE_CONNECTED:
            commandTypeString = "connected";
            break;
        case COMMAND_TYPE_DISCONNECTED:
            commandTypeString = "disconnected";
            break;
        case COMMAND_TYPE_INVALID:
            commandTypeString = "invalid";
            break;
        case COMMAND_TYPE_MAX:
            commandTypeString = "invalid";
            break;
        default:
            commandTypeString = "invalid";
            break;
    }

    switch (mCommandSubType)
    {
        case COMMAND_SUBTYPE_NONE:
            commandSubTypeString = "";
            break;
        case COMMAND_SUBTYPE_ADDITION_CONFIRMATION:
            commandSubTypeString = "additionConfirmation";
            break;
        case COMMAND_SUBTYPE_DECONFIGURE:
            commandSubTypeString = "deconfigure";
            break;
        case COMMAND_SUBTYPE_INVALID:
            commandSubTypeString = "invalid";
            break;
        case COMMAND_SUBTYPE_MAX:
            commandSubTypeString = "invalid";
            break;
        default:
            commandSubTypeString = "invalid";
            break;
    }

    switch (mResponseType)
    {
        case RESPONSE_TYPE_NONE:
            responseTypeString = "";
            break;
        case RESPONSE_TYPE_SUCCESS:
            responseTypeString = "success";
            break;
        case RESPONSE_TYPE_FAILED:
            responseTypeString = "failed";
            break;
        case RESPONSE_TYPE_MAX:
            responseTypeString = "invalid";
            break;
        default:
            responseTypeString = "invalid";
            break;
    }

    switch (mStatusType)
    {
        case STATUS_TYPE_NONE:
            statusTypeString = "";
            break;
        case STATUS_TYPE_ACTIVATION_PENDING:
            statusTypeString = "pending";
            break;
        case STATUS_TYPE_ACTIVATION_TIMEOUT:
            statusTypeString = "timeout";
            break;
        case STATUS_TYPE_INVALID:
            statusTypeString = "invalid";
            break;
        case STATUS_TYPE_MAX:
            statusTypeString = "invalid";
            break;
        default:
            statusTypeString = "invalid";
            break;
    }

    if (responseTypeString.empty())
    {
        if (!messageTypeString.empty())
        {
            cJSON_AddStringToObject(mObj, "message_type", messageTypeString.c_str());
        }

        if (!commandTypeString.empty())
        {
            cJSON_AddStringToObject(mObj, "command_type", commandTypeString.c_str());
            if (mCommandType == COMMAND_TYPE_STATUS)
            {
                cJSON_AddStringToObject(mObj, "slot" , key.c_str());
                if (mDataType == Data::DATA_TYPE_INTEGER)
                {
                    cJSON_AddNumberToObject(mObj, "value", mIntegerData);
                }
                else if (mDataType == Data::DATA_TYPE_STRING)
                {
                    cJSON_AddStringToObject(mObj, "value", mStringData.c_str());
                }
                else
                {
                }
            }
        }

        if (!commandSubTypeString.empty())
        {
            cJSON_AddStringToObject(mObj, "command_subtype", commandSubTypeString.c_str());
        }

        if (!statusTypeString.empty())
        {
            cJSON_AddStringToObject(mObj, "status", statusTypeString.c_str());
        }
    }
    else
    {
        cJSON_AddStringToObject(mObj, "command_type", "acknowledgement");
        cJSON_AddStringToObject(mObj, "response_status", responseTypeString.c_str());
        cJSON_AddStringToObject(mObj, "message_id", mMessageId.c_str());
        if (mCommandType != COMMAND_TYPE_DECONFIGURE && mCommandType != COMMAND_TYPE_ACTIVATION)
        {
            cJSON_AddStringToObject(mObj, "slot", key.c_str());
            if (mDataType == Data::DATA_TYPE_INTEGER)
            {
                cJSON_AddNumberToObject(mObj, "value", mIntegerData);
            }
            else if (mDataType == Data::DATA_TYPE_STRING)
            {
                cJSON_AddStringToObject(mObj, "value", mStringData.c_str());
            }
            else
            {
            }
        }
    }

    str = cJSON_PrintUnformatted(mObj);
    stringified = std::string(str);
    free(str);

    ESP_LOGI(TAG, "Data : %s", stringified.c_str());

    if (IsEncryptionEnable())
    {
        PayloadEncryption::EncryptCBC(&encryptedData, &iv, stringified);

        eObj = cJSON_CreateObject();
        cJSON_AddStringToObject(eObj, "iv", iv.c_str());
        cJSON_AddStringToObject(eObj, "eData", encryptedData.c_str());

        str = cJSON_PrintUnformatted(eObj);
        stringified = std::string(str);
        free(str);

        ESP_LOGI(TAG, "Encrypted Data : %s", stringified.c_str());

        cJSON_Delete(eObj);
    }

end:
    cJSON_Delete(mObj);
    return stringified;
}

void Message::FromString(std::string data)
{
    cJSON * mObj, * eObj, * iv, * eData, * commandType, * command, * serverTimestamp, * messageId, * _data, * _slot;

    std::string stringified;
    std::string messageTypeString;
    std::string commandTypeString;
    std::string commandString;
    std::string commandSubTypeString; 
    std::string responseTypeString;
    std::string statusTypeString;
    std::string encryptedData;
    std::string messageIdString;

    double serverTimestampValue;
    // std::string iv;

    mObj = cJSON_Parse(data.c_str());
    if (mObj == NULL)
    {
        ESP_LOGE(TAG, "Error Parsing Object mObj");
        goto end;
    }

    iv = cJSON_GetObjectItemCaseSensitive(mObj, "iv");
    eData = cJSON_GetObjectItemCaseSensitive(mObj, "eData");

    if (cJSON_IsString(iv) && cJSON_IsString(eData))
    {
        ESP_LOGI(TAG, "Message is Encrypted");
        // cJSON_GetObjectItemCaseSensitive()
        std::string decryptedData;
        PayloadEncryption::DecryptCBC(&decryptedData, std::string(cJSON_GetStringValue(iv)), std::string(cJSON_GetStringValue(eData)));
        ESP_LOGI(TAG, "Decrypted data : %s", decryptedData.c_str());

        eObj = cJSON_Parse(decryptedData.c_str());
        if (eObj == NULL)
        {
            ESP_LOGE(TAG, "Error Parsing Object eObj");
            goto end;
        }

        commandType = cJSON_GetObjectItemCaseSensitive(eObj, "command_type");
        command = cJSON_GetObjectItemCaseSensitive(eObj, "command");
        serverTimestamp = cJSON_GetObjectItemCaseSensitive(eObj, "server_timestamp");
        messageId = cJSON_GetObjectItemCaseSensitive(eObj, "message_id");
        _data = cJSON_GetObjectItemCaseSensitive(eObj, "value");
        _slot = cJSON_GetObjectItemCaseSensitive(eObj, "slot");
    }
    // else if (/* condition */)
    // {
    //     /* code */
    // }
    else
    {
        ESP_LOGE(TAG, "Message is invalid");
        goto end;
    }

    if (commandType)
    {
        commandTypeString = std::string(cJSON_GetStringValue(commandType));
    }
    if (command)
    {
        commandString = std::string(cJSON_GetStringValue(command));
    }
    if (serverTimestamp)
    {
        serverTimestampValue = cJSON_GetNumberValue(serverTimestamp);
        mServerTimestamp = serverTimestampValue;
    }
    if (messageId)
    {
        messageIdString = std::string(cJSON_GetStringValue(messageId));
        mMessageId = messageIdString;
    }
    if(_slot){
        mSlot = std::string(cJSON_GetStringValue(_slot));
    }
    if (_data)
    {
        if (cJSON_IsNumber(_data))
        {
            mIntegerData = cJSON_GetNumberValue(_data);
            mDataType = DATA_TYPE_INTEGER;
        }
        else if (cJSON_IsString(_data))
        {
            mStringData = std::string(cJSON_GetStringValue(_data));
            mDataType = DATA_TYPE_STRING;
        }
        else
        {
            mDataType = DATA_TYPE_INVALID;
        }
    }
    else
    {
        mDataType = DATA_TYPE_NONE;
    }

    if (commandTypeString == "activation")
    {
        mCommandType = CommandType::COMMAND_TYPE_ACTIVATION;
    }
    else if (commandTypeString == "control")
    {
        mCommandType = CommandType::COMMAND_TYPE_CONTROL;
    }
    else if (commandTypeString == "status")
    {
        mCommandType = CommandType::COMMAND_TYPE_STATUS;
    }
    else if (commandTypeString == "deconfigure")
    {
        mCommandType = CommandType::COMMAND_TYPE_DECONFIGURE;
    }
    else if (commandTypeString == "delete") //  not used
    {
        mCommandType = CommandType::COMMAND_TYPE_DELETE;
    }
    else if (commandTypeString == "disconnected")
    {
        mCommandType = CommandType::COMMAND_TYPE_DISCONNECTED;
    }
    else
    {
        ESP_LOGE(TAG, "Invalid commandTypeString");
    }

    if (commandString == "additionConfirmation")
    {
        mCommandSubType = CommandSubType::COMMAND_SUBTYPE_ADDITION_CONFIRMATION;
    }

    /* Assing string and values to the instance entities directly */

end:
    cJSON_Delete(mObj);
}

Message * Message::FromStringWithKey(std::string data, std::string key)
{
    cJSON * mObj, * eObj, * commandType, * command, * serverTimestamp, * messageId, * _data;

    std::string stringified;
    std::string messageTypeString;
    std::string commandTypeString;
    std::string commandString;
    std::string commandSubTypeString;
    std::string responseTypeString;
    std::string statusTypeString;
    std::string encryptedData;
    std::string messageIdString;
    double _dataDouble;

    double serverTimestampValue;
    // std::string iv;

    mObj = cJSON_Parse(data.c_str());
    if (mObj == NULL)
    {
        ESP_LOGE(TAG, "Error Parsing Object mObj");
        goto end;
    }

    if (cJSON_IsString(cJSON_GetObjectItemCaseSensitive(mObj, "iv")) && cJSON_IsString(cJSON_GetObjectItemCaseSensitive(mObj, "eData")))
    {
        ESP_LOGI(TAG, "Message is Encrypted");
        // cJSON_GetObjectItemCaseSensitive()
        std::string decryptedData;
        PayloadEncryption::DecryptCBC(&decryptedData, std::string(cJSON_GetStringValue(cJSON_GetObjectItemCaseSensitive(mObj, "iv"))), 
        std::string(cJSON_GetStringValue(cJSON_GetObjectItemCaseSensitive(mObj, "eData"))));
        ESP_LOGI(TAG, "Decrypted data : %s", decryptedData.c_str());

        eObj = cJSON_Parse(decryptedData.c_str());
        if (eObj == NULL)
        {
            ESP_LOGE(TAG, "Error Parsing Object eObj");
            goto end;
        }

        commandType = cJSON_GetObjectItemCaseSensitive(eObj, "command_type");
        command = cJSON_GetObjectItemCaseSensitive(eObj, "command");
        serverTimestamp = cJSON_GetObjectItemCaseSensitive(eObj, "server_timestamp");
        messageId = cJSON_GetObjectItemCaseSensitive(eObj, "message_id");
        _data = cJSON_GetObjectItemCaseSensitive(eObj, key.c_str());
        _data = cJSON_GetObjectItemCaseSensitive(_data, "value");
    }
    // else if (/* condition */)
    // {
    //     /* code */
    // }
    else
    {
        ESP_LOGE(TAG, "Message is invalid");
        goto end;
    }

    if (commandType)
    {
        commandTypeString = std::string(cJSON_GetStringValue(commandType));
    }
    if (command)
    {
        commandString = std::string(cJSON_GetStringValue(command));
    }
    if (serverTimestamp)
    {
        serverTimestampValue = cJSON_GetNumberValue(serverTimestamp);
        mServerTimestamp = serverTimestampValue;
    }
    if (messageId)
    {
        messageIdString = std::string(cJSON_GetStringValue(messageId));
        mMessageId = messageIdString;
    }
    if (_data)
    {
        if (cJSON_IsNumber(_data))
        {
            mIntegerData = cJSON_GetNumberValue(_data);
            mDataType = DATA_TYPE_INTEGER;
        }
        else if (cJSON_IsString(_data))
        {
            mStringData = std::string(cJSON_GetStringValue(_data));
            mDataType = DATA_TYPE_STRING;
        }
        else
        {
            mDataType = DATA_TYPE_INVALID;
        }
    }
    else
    {
        mDataType = DATA_TYPE_NONE;
    }

    // cJSON_IsString()


    /* If any one fails the entire object is invalid */
    /* Use switch case to store the value in the mTypes */

    if (commandTypeString == "activation")
    {
        mCommandType = CommandType::COMMAND_TYPE_ACTIVATION;
    }
    else if (commandTypeString == "control")
    {
        mCommandType = CommandType::COMMAND_TYPE_CONTROL;
    }
    else if (commandTypeString == "status")
    {
        mCommandType = CommandType::COMMAND_TYPE_STATUS;
    }
    else if (commandTypeString == "deconfigure")
    {
        mCommandType = CommandType::COMMAND_TYPE_DECONFIGURE;
    }
    else if (commandTypeString == "delete") //  not used
    {
        mCommandType = CommandType::COMMAND_TYPE_DELETE;
    }
    else if (commandTypeString == "disconnected")
    {
        mCommandType = CommandType::COMMAND_TYPE_DISCONNECTED;
    }
    else
    {
        ESP_LOGE(TAG, "Invalid commandTypeString");
    }

    if (commandString == "additionConfirmation")
    {
        mCommandSubType = CommandSubType::COMMAND_SUBTYPE_ADDITION_CONFIRMATION;
    }

    /* Assing string and values to the instance entities directly */

end:
    cJSON_Delete(mObj);
    cJSON_Delete(eObj);
    return this;
}
