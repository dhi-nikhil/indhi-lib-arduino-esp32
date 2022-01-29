#ifndef HTTP_CLIENT_H_
#define HTTP_CLIENT_H_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_http_client.h"
#include "string"
#include "vector"
#include "HTTPClientSSLCert.h"
#include <map>

class HTTPClient;

class HTTPResponse
{
    friend HTTPClient;
private:
    bool success;
    std::vector<std::string> mHeaders;  //  For Headers this is ok
    std::string mData; // formatted data
public:
    HTTPResponse();
    ~HTTPResponse();
};

class HTTPRequest
{
    friend HTTPClient;
protected:
    std::string mURL;
    esp_http_client_method_t mMethod;
    std::vector<std::string> mHeaders;  //  For Headers this is ok
    std::string mData; // formatted data
public:
    HTTPRequest();
    ~HTTPRequest();
};


class HTTPClient :public HTTPClientSSLCert
{
private:
    /* required to initialize here */
    esp_http_client_handle_t mHTTPClientHandle = nullptr;
    esp_http_client_config_t mHTTPClientConfig = { 0 };
    // char * mResponseData;
    std::string mResponseData;
    std::string mAPIKey;
    char mResponseBuffer[2048] = { 0 };

public:
    HTTPClient();
    ~HTTPClient();
    // std::string Request(std::string * request, esp_http_client_method_t method);
    static esp_err_t HTTPEventHandler(esp_http_client_event_t * event);
    void HTTPClientInit();
    esp_err_t HTTPClientPerform();
    void HTTPClientSetURL(std::string url);
    std::string HTTPClientGetURL();
    esp_err_t HTTPClientSetMethod(esp_http_client_method_t method);
    int HTTPClientGetStatusCode();
    int HTTPClientGetContentLength();
    int HTTPClientSetPostField(std::string * data);
    int HTTPClientGetPostField();
    esp_err_t HTTPClientSetHeader(std::string key, std::string value);
    std::string HTTPClientGetHeaderValueByKey(std::string key);
    std::string HTTPClientGetUsername();
    esp_err_t HTTPClientSetUsername(std::string username);
    esp_err_t HTTPClientSetPassword(std::string password);
    std::string HTTPClientGetPassword();
    esp_err_t HTTPClientSetAuthType(esp_http_client_auth_type_t auth);
    esp_err_t HTTPClientSetTimeout(int milliSeconds);
    esp_err_t HTTPClientDeleteHeader(std::string key);
    esp_err_t HTTPClientOpenConnection(int writeLength);
    int HTTPClientFetchHeaders();
    bool HTTPClientIsChunkedResponse();
    int HTTPClientRead(std::string data);
    int HTTPGetContentLength();
    esp_err_t HTTPClientClose();
    esp_err_t HTTPClientCleanup();
    esp_http_client_transport_t HTTPClientGetTransportType();
    esp_err_t HTTPClientSetRedirection();
    void HTTPClientAddAuth();
    bool HTTPClientIsCompleteDataReceived();
    int HTTPClientReadResponse(std::string * data);
    esp_err_t HTTPClientFlushResponse();
    int HTTPClientGetChunkedLength();
};


#endif
