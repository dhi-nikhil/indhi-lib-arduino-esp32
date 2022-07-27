#ifndef HTTP_SERVER_H_
#define HTTP_SERVER_H_

#include "esp_http_server.h"
#include "string.h"
#include "string"

/**
 * @brief http request handler
 *
 */
typedef esp_err_t (*mhandler)( httpd_req * request );

class HTTPServer
{
private:
    /**
     * @brief HTTP server handler
     *
     */
    httpd_handle_t mHTTPHandle;
    /**
     * @brief HTTP server config
     *
     */
    httpd_config_t mHTTPConfig;

public:
    /**
     * @brief Construct a new HTTPServer object
     *
     */
    HTTPServer();
    /**
     * @brief Start the HTTP server
     *
     * @return esp_err_t success or failed
     */
    esp_err_t Start();
    /**
     * @brief Stop the HTTP server
     *
     * @return esp_err_t success or failed
     */
    esp_err_t Stop();
    /**
     * @brief Event handler to register
     *
     * @param error error code
     * @param handler_fn error handler
     * @return esp_err_t success or failed
     */
    esp_err_t RegisterErrorHandler(httpd_err_code_t error, httpd_err_handler_func_t handler_fn);
    /**
     * @brief Register the encpoint paramters
     *
     * @param uri uri to call for endpoint
     * @param handler event handler to handle the request
     * @param method HTTP Method to set for the call
     * @param context Context to pass to the handler
     * @return esp_err_t success or failed
     */
    esp_err_t Register(std::string uri, mhandler handler, httpd_method_t method = HTTP_GET, void * context = nullptr);
    /**
     * @brief Unregister the URI
     *
     * @param uri URI to unregister
     * @return esp_err_t success or failed
     */
    esp_err_t Unregister(std::string uri);
    /**
     * @brief Destroy the HTTPServer object
     *
     */
    ~HTTPServer();
};

// intutive ai
#endif
