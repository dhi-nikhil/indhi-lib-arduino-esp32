#define LOG_LOCAL_LEVEL ESP_LOG_NONE

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "sdkconfig.h"
#include "HTTPClient.h"

static const char * TAG = "HTTPClient";

// namespace Idf
// {
//     namespace HTTPClient
//     {

// extern const uint8_t ca_cert_pem_start [] asm("_binary_ca_cert_pem_start");
// extern const uint8_t ca_cert_pem_end [] asm("_binary_ca_cert_pem_end");

HTTPClient::HTTPClient() :HTTPClientSSLCert()
{
    // mHTTPClientConfig.host = "httpbin.org";
    // mHTTPClientConfig.path = "/get";
    // mHTTPClientConfig.query = "esp";
    /* Above parameters are just for  */
    // mHTTPClientConfig
    // mHTTPClientConfig.host = NULL;
    mHTTPClientConfig.url = "https://devws.indhi.io";
    // mHTTPClientConfig.host = "https://devws.indhi.io";
    // mHTTPClientConfig.path = "/device/validateDevice";
    // mHTTPClientConfig.port = 80;
    mHTTPClientConfig.buffer_size = 2048;
    mHTTPClientConfig.event_handler = HTTPEventHandler;
    mHTTPClientConfig.buffer_size_tx = 2048;
    mHTTPClientConfig.skip_cert_common_name_check = false;
    mHTTPClientConfig.user_data = mResponseBuffer;// Pass address of local buffer to get response
    mHTTPClientConfig.disable_auto_redirect = false;
    mHTTPClientConfig.timeout_ms = 10000;
    // mHTTPClientConfig.auth_type = esp_http_client_auth_type_t::HTTP_AUTH_TYPE_NONE;
    mHTTPClientConfig.transport_type = HTTP_TRANSPORT_OVER_SSL;
    mHTTPClientConfig.user_data = this;    //  local event handler
    mHTTPClientConfig.cert_pem = HTTPClientSSLCert::GetCertificate();
    // mHTTPClientConfig.cert_pem = (char *) ca_cert_pem_start;
    // ESP_LOGI(TAG, "Certificate \n %s", HTTPClientSSLCert::GetCertificate());
}

HTTPClient::~HTTPClient() {}

// std::string HTTPClient::Request(std::string * request, esp_http_client_method_t method)
// {
//     response->mData = mResponseData;
//     return *response;
// }

esp_err_t HTTPClient::HTTPClientSetAuthType(esp_http_client_auth_type_t auth)
{
    return ::esp_http_client_set_authtype(mHTTPClientHandle, auth);
}

esp_err_t HTTPClient::HTTPClientSetTimeout(int milliSeconds)
{
    return ::esp_http_client_set_timeout_ms(mHTTPClientHandle, milliSeconds);
}

esp_err_t HTTPClient::HTTPClientDeleteHeader(std::string key)
{
    return ::esp_http_client_delete_header(mHTTPClientHandle, key.c_str());
}

esp_err_t HTTPClient::HTTPClientOpenConnection(int writeLength)
{
    return ::esp_http_client_open(mHTTPClientHandle, writeLength);
}

esp_err_t HTTPClient::HTTPClientClose()
{
    return ::esp_http_client_close(mHTTPClientHandle);
}

esp_err_t HTTPClient::HTTPClientCleanup()
{
    return ::esp_http_client_cleanup(mHTTPClientHandle);
}

esp_http_client_transport_t HTTPClient::HTTPClientGetTransportType()
{
    return ::esp_http_client_get_transport_type(mHTTPClientHandle);
}

esp_err_t HTTPClient::HTTPClientSetRedirection()
{
    return ::esp_http_client_set_redirection(mHTTPClientHandle);
}

esp_err_t HTTPClient::HTTPClientFlushResponse()
{
    // return ::esp_http_client_flush_response(mHTTPClientHandle, )
    return ESP_OK;
}

esp_err_t HTTPClient::HTTPEventHandler(esp_http_client_event_t * event)
{
    ESP_LOGI(TAG, "Event Occured With Id %d", event->event_id);
    HTTPClient * httpClient = (HTTPClient *) event->user_data;
    switch (event->event_id)
    {
        case HTTP_EVENT_ON_DATA:
            if (!httpClient->HTTPClientIsChunkedResponse())
            {
                std::string responseData ((char *) event->data, event->data_len);
                httpClient->mResponseData = responseData;
                ESP_LOGI(TAG, "Response received : %s", httpClient->mResponseData.c_str());
            }
            break;

        default:
            break;
    }
    return ESP_OK;
}

void HTTPClient::HTTPClientInit()
{
    mHTTPClientHandle = esp_http_client_init(&mHTTPClientConfig);
    if (mHTTPClientHandle == NULL)
    {
        ESP_LOGE(TAG, "Error initializing");
    }
}

esp_err_t HTTPClient::HTTPClientPerform()
{
    return ::esp_http_client_perform(mHTTPClientHandle);
}

void HTTPClient::HTTPClientSetURL(std::string url)
{
    ESP_LOGI(TAG, "URL set to %s", url.c_str());
    ESP_ERROR_CHECK_WITHOUT_ABORT(::esp_http_client_set_url(mHTTPClientHandle, url.c_str()));
}

std::string HTTPClient::HTTPClientGetURL()
{
    char url[50] = { 0 };
    int length = 0;
    ::esp_http_client_get_url(mHTTPClientHandle, &url[0], length);
    return std::string(url);
}

esp_err_t HTTPClient::HTTPClientSetMethod(esp_http_client_method_t method)
{
    return ::esp_http_client_set_method(mHTTPClientHandle, method);
}

int HTTPClient::HTTPClientGetStatusCode()
{
    return ::esp_http_client_get_status_code(mHTTPClientHandle);
}

int HTTPClient::HTTPClientGetContentLength()
{
    return ::esp_http_client_get_content_length(mHTTPClientHandle);
}

esp_err_t HTTPClient::HTTPClientSetPostField(std::string * data)
{
    ESP_LOGI(TAG, "Request %s", ( *data ).c_str());
    return ::esp_http_client_set_post_field(mHTTPClientHandle, ( *data ).c_str(), ( *data ).length());
}

int HTTPClient::HTTPClientGetPostField()
{
    return 0;
}

esp_err_t HTTPClient::HTTPClientSetHeader(std::string key, std::string value)
{
    return ::esp_http_client_set_header(mHTTPClientHandle, key.c_str(), value.c_str());
}

std::string HTTPClient::HTTPClientGetHeaderValueByKey(std::string key)
{
    char * valueBuffer;
    ::esp_http_client_get_header(mHTTPClientHandle, key.c_str(), & valueBuffer);
    std::string valueString(valueBuffer);
    return valueString;
}

std::string HTTPClient::HTTPClientGetUsername()
{
    char * usernameBuffer;
    ::esp_http_client_get_username(mHTTPClientHandle, &usernameBuffer);
    std::string usernameString(usernameBuffer);
    return usernameString;
}

esp_err_t HTTPClient::HTTPClientSetUsername(std::string username)
{
    return ::esp_http_client_set_username(mHTTPClientHandle, username.c_str());
}

esp_err_t HTTPClient::HTTPClientSetPassword(std::string password)
{
    return ::esp_http_client_set_password(mHTTPClientHandle, password.c_str());
}

std::string HTTPClient::HTTPClientGetPassword()
{
    char * passwordBuffer;
    ::esp_http_client_get_password(mHTTPClientHandle, &passwordBuffer);
    std::string passwordString(passwordBuffer);
    return passwordString;
}

int HTTPClient::HTTPClientFetchHeaders()
{
    return ::esp_http_client_fetch_headers(mHTTPClientHandle);
}

bool HTTPClient::HTTPClientIsChunkedResponse()
{
    return ::esp_http_client_is_chunked_response(mHTTPClientHandle);
}

// int HTTPClient::HTTPClientRead(std::string data)
// {

// }

int HTTPClient::HTTPGetContentLength()
{
    return ::esp_http_client_get_content_length(mHTTPClientHandle);
}

void HTTPClient::HTTPClientAddAuth()
{
    ::esp_http_client_add_auth(mHTTPClientHandle);
}

bool HTTPClient::HTTPClientIsCompleteDataReceived()
{
    return ::esp_http_client_is_complete_data_received(mHTTPClientHandle);
}

int HTTPClient::HTTPClientReadResponse(std::string * data)
{
    int length = 0;
    // char responseBuffer[2048] = { 0 };
    // length = esp_http_client_read_response(mHTTPClientHandle, &responseBuffer[0], 2048);
    // if (length == 0)
    // {
    //     ESP_LOGE(TAG, "Response Data Length Error %d", length);
    //     *data = std::string(mResponseBuffer);
    //     length = ( *data ).length();
    //     ESP_LOGI(TAG, "DATA : %s", ( *data ).c_str());
    // }
    // else
    // {
    //     ESP_LOGI(TAG, "Response Data Length Error %d", length);
    //     *data = std::string(responseBuffer);
    // }
    // return length;
    * data = mResponseData;
    return mResponseData.length();
}

int HTTPClient::HTTPClientGetChunkedLength()
{
    return ::esp_http_client_get_content_length(mHTTPClientHandle);
}


// } // HTTPClient
// } // Idf

