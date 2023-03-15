#ifndef MESSAGE_H_
#define MESSAGE_H_

#include "string"
#include "PayloadEncryption.h"

/**
 * @brief Status
 *
 */
class Status
{
private:
    /* data */
public:
    /**
     * @brief StatusType
     *
     */
    enum StatusType
    {
        STATUS_TYPE_NONE = 0,   /* No status */
        STATUS_TYPE_ACTIVATION_PENDING, /* activation pending */
        STATUS_TYPE_ACTIVATION_TIMEOUT, /* activation timeout */
        STATUS_TYPE_INVALID, /*invalid */
        STATUS_TYPE_MAX,
    };

    Status(/* args */) {}
    ~Status() {}
};

/**
 * @brief Response
 *
 */
class Response
{
private:
    /* data */
public:
    /**
     * @brief Response Type
     *
     */
    enum ResponseType
    {
        RESPONSE_TYPE_NONE = 0, /* Resposnse none */
        RESPONSE_TYPE_SUCCESS, /* Response success */
        RESPONSE_TYPE_FAILED, /* Response failed */
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
    /**
     * @brief CommandType
     *
     */
    enum CommandType
    {
        COMMAND_TYPE_NONE = 0, /* Command type none */
        COMMAND_TYPE_STATUS, /* command type status */
        COMMAND_TYPE_CONTROL, /* command type control */
        COMMAND_TYPE_UPDATE, /* command type update */
        COMMAND_TYPE_CONFIG, /* command type config */
        COMMAND_TYPE_DECONFIGURE,/* command type deconfigure */
        COMMAND_TYPE_DELETE,/* command type delete */
        COMMAND_TYPE_ACTIVATION,/* command type activation */
        COMMAND_TYPE_CONNECTED,/* command type connected */
        COMMAND_TYPE_DISCONNECTED,/* command type disconnected */
        COMMAND_TYPE_ACKNOWLEDGEMENT,/* command type acknowledgement */
        COMMAND_TYPE_INVALID,/* command type invalid */
        COMMAND_TYPE_MAX,
    };

    /**
     * @brief CommandSubType
     *
     */
    enum CommandSubType
    {
        COMMAND_SUBTYPE_NONE = 0, /* Command sub type none */
        COMMAND_SUBTYPE_ADDITION_CONFIRMATION, /* Command sub type addition confirmation */
        COMMAND_SUBTYPE_DECONFIGURE,/* Command sub type deconfigure */
        COMMAND_SUBTYPE_INVALID,/* Command sub type invalid */
        COMMAND_SUBTYPE_MAX,
    };
};

/**
 * @brief Data
 *
 */
class Data
{
private:
    /* data */
public:
    /**
     * @brief DataType
     *
     */
    enum DataType
    {
        DATA_TYPE_NONE = 0, /* Data type none */
        DATA_TYPE_INTEGER, /* Data type integer */
        DATA_TYPE_STRING, /* Data type string */
        DATA_TYPE_INVALID, /* invalid */
        DATA_TYPE_MAX,
    };

};

/**
 * @brief Message class
 *
 */
class Message : public Status, public Response, public Command, public Data
{
public:
    static bool mIsEncryptionEnable; /* is encryotion enable or not */

    /**
     * @brief MessageType
     *
     */
    enum MessageType
    {
        MESSAGE_TYPE_NONE = 0,  /* Meessage Type None */
        MESSAGE_TYPE_CONNECTED, /* Message Type connected */
        MESSAGE_TYPE_DISCONNECTED,/* Message Type disconnected */
        MESSAGE_TYPE_DATA,/* Message Type data */
        MESSAGE_TYPE_INVALID,/* Message Type invalid */
        MESSAGE_TYPE_MAX,
    };

    /**
     * @brief Construct a new Message object
     *
     */
    Message();
    /**
     * @brief Convert the message to string
     *
     * @return std::string string converted message
     */
    std::string ToString();
    /**
     * @brief Convert the message to string with data parameter key
     *
     * @param key assign this key to data parameter
     * @return std::string
     */
    std::string ToStringWithKey(std::string key);
    /**
     * @brief Convert message object from the string
     *
     * @param data data string to parse
     * @return Message* return the object
     */
    void FromString(std::string data);
    /**
     * @brief Convert the message to object and fetch the data with key
     *
     * @param data data to convert into object
     * @param key to fetch from the object
     * @return Message* return the object
     */
    Message * FromStringWithKey(std::string data, std::string key);
    /**
     * @brief Destroy the Message object
     *
     */
    ~Message();

    /**
     * @brief Encryption is enable or not
     *
     * @return true if enabled
     * @return false if disabled
     */
    static bool IsEncryptionEnable()
    {
        return mIsEncryptionEnable;
    }

    /**
     * @brief Set encryotion
     *
     * @param encryptionFlag set the encryption
     */
    static void EncryptionEnable(bool encryptionFlag)
    {
        mIsEncryptionEnable = encryptionFlag;
    }
    /**
     * @brief Set the Message Type object
     *
     * @param messageType Message type to set
     * @return Message* return the object
     */
    Message * SetMessageType(MessageType messageType)
    {
        mMessageType = messageType;
        return this;
    }
    /**
     * @brief Set the Command Type object
     *
     * @param commandType Command type to set
     * @return Message* return the object
     */
    Message * SetCommandType(CommandType commandType)
    {
        mCommandType = commandType;
        return this;
    }
    /**
     * @brief Set the Command Sub Type object
     *
     * @param commandSubType Command type to set
     * @return Message* return the object
     */
    Message * SetCommandSubType(CommandSubType commandSubType)
    {
        mCommandSubType = commandSubType;
        return this;
    }
    /**
     * @brief Set the Response Type object
     *
     * @param responseType Response type to set
     * @return Message* return the object
     */
    Message * SetResponseType(ResponseType responseType)
    {
        // mCommandType = CommandType::COMMAND_TYPE_ACKNOWLEDGEMENT;
        mResponseType = responseType;
        return this;
    }
    /**
     * @brief Set the Status Type object
     *
     * @param statusType Status type to set
     * @return Message* return the object
     */
    Message * SetStatusType(StatusType statusType)
    {
        mStatusType = statusType;
        return this;
    }
    /**
     * @brief Set the Data Type object
     *
     * @param dataType data type to set
     * @return Message* return the object
     */
    Message * SetDataType(DataType dataType)
    {
        mDataType = dataType;
        return this;
    }
    /**
     * @brief Set the Data object
     *
     * @param data data to set
     * @return Message* return the object
     */
    Message * SetData(double data)
    {
        mIntegerData = data;
        return SetDataType(DataType::DATA_TYPE_INTEGER);
    }
    /**
     * @brief Set the Data object
     *
     * @param data data to set
     * @return Message* return the object
     */
    Message * SetData(std::string data)
    {
        mStringData = data;
        return SetDataType(DataType::DATA_TYPE_STRING);
    }

    /* data */
    MessageType mMessageType; /* message type */
    CommandType mCommandType; /* command type */
    CommandSubType mCommandSubType; /* command sub type */
    ResponseType mResponseType; /* response type */
    StatusType mStatusType; /* status type */
    double mServerTimestamp; /* server timestamp */
    double mDeviceTimestamp;/* device timestamp */
    double mIntegerData; /* integet data */
    std::string mStringData; /* string data */
    DataType mDataType; /* data type */
    std::string mMessageId; /* message id */
    std::string mSlot; /* data slot */

private:

};

/**
 * @brief Message Create class
 *
 */
class MessageCreate : public Message
{
private:
    /* data */
public:
    /**
     * @brief Construct the Message Create object
     *
     */
    MessageCreate(/* args */) :Message()
    {

    }
    /**
     * @brief Destroy the Message Create object
     *
     */
    ~MessageCreate();
};

class MessageParse : public Message
{
private:
    /* data */
public:
    /**
     * @brief Construct the Message Parse object
     *
     */
    MessageParse(/* args */) :Message()
    {

    }
    /**
     * @brief Destroy the Message Parse object
     *
     */
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
