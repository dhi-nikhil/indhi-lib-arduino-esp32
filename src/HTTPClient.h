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

/**
 * @brief Class to handle HTTPClient
 *
 */
class HTTPClient :public HTTPClientSSLCert
{
private:
    /**
     * @brief handle to manage the http client
     *
     */
    esp_http_client_handle_t mHTTPClientHandle = nullptr;
    /**
     * @brief http client configration
     *
     */
    esp_http_client_config_t mHTTPClientConfig = { 0 };
    /**
     * @brief Save the response data
     *
     */
    std::string mResponseData;
    /**
     * @brief Store the API key
     *
     */
    std::string mAPIKey;
    /**
     * @brief Store the response data in buffer
     *
     */
    char mResponseBuffer[2048] = { 0 };

public:
    /**
     * @brief Construct a new HTTPClient object
     *
     */
    HTTPClient();
    /**
     * @brief Destroy the HTTPClient object
     *
     */
    ~HTTPClient();
    // std::string Request(std::string * request, esp_http_client_method_t method);
    /**
     * @brief HTTP event handler
     *
     * @param event event with event data to pass
     * @return esp_err_t
     */
    static esp_err_t HTTPEventHandler(esp_http_client_event_t * event);
    /**
     * @brief HTTP client initialize
     *
     */
    void HTTPClientInit();
    /**
     * @brief Perform the http client request
     *
     * @return esp_err_t
     */
    esp_err_t HTTPClientPerform();
    /**
     * @brief Set the http client endpoint url
     *
     * @param url
     */
    void HTTPClientSetURL(std::string url);
    /**
     * @brief Get the endpoint url
     *
     * @return std::string
     */
    std::string HTTPClientGetURL();
    /**
     * @brief Set the http client method
     *
     * @param method HTTP METHOD
     * @return esp_err_t return is success or fail
     */
    esp_err_t HTTPClientSetMethod(esp_http_client_method_t method);
    /**
     * @brief Get the status code of the http call
     *
     * @return int
     */
    int HTTPClientGetStatusCode();
    /**
     * @brief Get the content length of the http call response
     *
     * @return int
     */
    int HTTPClientGetContentLength();
    /**
     * @brief Set the Post field
     *
     * @param data set the data
     * @return int reutn if true or false
     */
    int HTTPClientSetPostField(std::string * data);
    /**
     * @brief Get the post field
     *
     */
    int HTTPClientGetPostField();
    /**
     * @brief Set the HTTP header
     *
     * @param key key of the header
     * @param value value of the header
     * @return esp_err_t return if success or fail
     */
    esp_err_t HTTPClientSetHeader(std::string key, std::string value);
    /**
     * @brief Get the http header value by key
     *
     * @param key key of the header
     * @return std::string value of the header
     */
    std::string HTTPClientGetHeaderValueByKey(std::string key);
    /**
     * @brief Get the username
     *
     * @return std::string return the username
     */
    std::string HTTPClientGetUsername();
    /**
     * @brief Set the username
     *
     * @param username username to set
     * @return esp_err_t return success or failed
     */
    esp_err_t HTTPClientSetUsername(std::string username);
    /**
     * @brief Set the password
     *
     * @param password password to set
     * @return esp_err_t return success or failed
     */
    esp_err_t HTTPClientSetPassword(std::string password);
    /**
     * @brief Get the password
     *
     * @return std::string return password
     */
    std::string HTTPClientGetPassword();
    /**
     * @brief Set auth type
     *
     * @param auth auth type to set
     * @return esp_err_t return success or failed
     */
    esp_err_t HTTPClientSetAuthType(esp_http_client_auth_type_t auth);
    /**
     * @brief Set the http call timeout
     *
     * @param milliSeconds wait for duration
     * @return esp_err_t reuturn success or failed
     */
    esp_err_t HTTPClientSetTimeout(int milliSeconds);
    /**
     * @brief Delete the http header
     *
     * @param key key to delete
     * @return esp_err_t return success or failed
     */
    esp_err_t HTTPClientDeleteHeader(std::string key);
    /**
     * @brief Open HTTP connection
     *
     * @param writeLength length of the content need to write to server
     * @return esp_err_t success or failed
     */
    esp_err_t HTTPClientOpenConnection(int writeLength);
    /**
     * @brief Fetch the headers from the response
     *
     * @return int number of header values
     */
    int HTTPClientFetchHeaders();
    /**
     * @brief Is the response is chunked or not
     *
     * @return true
     * @return false
     */
    bool HTTPClientIsChunkedResponse();

    // int HTTPClientRead(std::string data);

    /**
     * @brief Get the content length of the response
     *
     * @return int
     */
    int HTTPGetContentLength();
    /**
     * @brief Close the client connection
     *
     * @return esp_err_t
     */
    esp_err_t HTTPClientClose();
    /**
     * @brief Cleanup the client from memory
     *
     * @return esp_err_t succes or failed
     */
    esp_err_t HTTPClientCleanup();
    /**
     * @brief Get the transport type for the request
     *
     * @return esp_http_client_transport_t return transport type
     */
    esp_http_client_transport_t HTTPClientGetTransportType();
    /**
     * @brief Set redirection
     *
     * @return esp_err_t success or not
     */
    esp_err_t HTTPClientSetRedirection();
    /**
     * @brief Add auth
     *
     */
    void HTTPClientAddAuth();
    /**
     * @brief Check if complete data received or not
     *
     * @return true
     * @return false
     */
    bool HTTPClientIsCompleteDataReceived();
    /**
     * @brief Read the response
     *
     * @param data save the read data here
     * @return int length of the read data
     */
    int HTTPClientReadResponse(std::string * data);
    /**
     * @brief Flush the response
     *
     * @return esp_err_t if success or failed
     */
    esp_err_t HTTPClientFlushResponse();
    /**
     * @brief Get the chunked data response length
     *
     * @return int length of the data read
     */
    int HTTPClientGetChunkedLength();
};


#endif
