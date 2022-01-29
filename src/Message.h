#ifndef MESSAGE_H_
#define MESSAGE_H_

#include "string"
#include "PayloadEncryption.h"

class Status
{
private:
    /* data */
public:
    enum StatusType
    {
        STATUS_TYPE_NONE = 0,
        STATUS_TYPE_ACTIVATION_PENDING,
        STATUS_TYPE_ACTIVATION_TIMEOUT,
        STATUS_TYPE_INVALID,
        STATUS_TYPE_MAX,
    };

    Status(/* args */) {}
    ~Status() {}
};

class Response
{
private:
    /* data */
public:
    enum ResponseType
    {
        RESPONSE_TYPE_NONE = 0,
        RESPONSE_TYPE_SUCCESS,
        RESPONSE_TYPE_FAILED,
        RESPONSE_TYPE_MAX,
    };

    Response(/* args */) {}
    ~Response() {}
};

class Command
{
private:
    /* data */
public:
    enum CommandType
    {
        COMMAND_TYPE_NONE = 0,
        COMMAND_TYPE_STATUS,
        COMMAND_TYPE_CONTROL,
        COMMAND_TYPE_DECONFIGURE,
        COMMAND_TYPE_DELETE,
        COMMAND_TYPE_ACTIVATION,
        COMMAND_TYPE_CONNECTED,
        COMMAND_TYPE_DISCONNECTED,
        COMMAND_TYPE_ACKNOWLEDGEMENT,
        COMMAND_TYPE_INVALID,
        COMMAND_TYPE_MAX,
    };

    enum CommandSubType
    {
        COMMAND_SUBTYPE_NONE = 0,
        COMMAND_SUBTYPE_ADDITION_CONFIRMATION,
        COMMAND_SUBTYPE_DECONFIGURE,
        COMMAND_SUBTYPE_INVALID,
        COMMAND_SUBTYPE_MAX,
    };
};
class Message : public Status, public Response, public Command
{
public:
    static bool mIsEncryptionEnable;

    enum MessageType
    {
        MESSAGE_TYPE_NONE = 0,
        MESSAGE_TYPE_CONNECTED,
        MESSAGE_TYPE_DISCONNECTED,
        MESSAGE_TYPE_DATA,
        MESSAGE_TYPE_INVALID,
        MESSAGE_TYPE_MAX,
    };

    Message();
    std::string ToString();
    Message * FromString(std::string data);
    ~Message();

    static bool IsEncryptionEnable()
    {
        return mIsEncryptionEnable;
    }

    static void EncryptionEnable(bool encryptionFlag)
    {
        mIsEncryptionEnable = encryptionFlag;
    }
    Message * SetMessageType(MessageType messageType)
    {
        mMessageType = messageType;
        return this;
    }
    Message * SetCommandType(CommandType commandType)
    {
        mCommandType = commandType;
        return this;
    }
    Message * SetCommandSubType(CommandSubType commandSubType)
    {
        mCommandSubType = commandSubType;
        return this;
    }
    Message * SetResponseType(ResponseType responseType)
    {
        // mCommandType = CommandType::COMMAND_TYPE_ACKNOWLEDGEMENT;
        mResponseType = responseType;
        return this;
    }
    Message * SetStatusType(StatusType statusType)
    {
        mStatusType = statusType;
        return this;
    }

    /* data */
    MessageType mMessageType;
    CommandType mCommandType;
    CommandSubType mCommandSubType;
    ResponseType mResponseType;
    StatusType mStatusType;
    double mServerTimestamp; // store this somewhere as the referecne to the command arrival
    double mDeviceTimestamp;
    double mData;
    std::string mMessageId;

private:

};

class MessageCreate : public Message
{
private:
    /* data */
public:
    MessageCreate(/* args */) :Message()
    {

    }
    ~MessageCreate();
};

class MessageParse : public Message
{
private:
    /* data */
public:
    MessageParse(/* args */) :Message()
    {

    }
    ~MessageParse();
};


// ESP_LOGI(TAG, "Encrypted data : %d", message->mIsEncryptionEnable);
// ESP_LOGI(TAG, "Message type : %d", message->mMessageType);
// ESP_LOGI(TAG, "Command type : %d", message->mCommandType);
// ESP_LOGI(TAG, "Command Type type : %d", message->mCommandSubType);
// ESP_LOGI(TAG, "Response type : %d", message->mResponseType);
// ESP_LOGI(TAG, "Status type : %d", message->mStatusType);
// ESP_LOGI(TAG, "Message Id : %s", message->mMessageId.c_str());
// ESP_LOGI(TAG, "Server Timestamp : %f", message->mServerTimestamp);

#endif
