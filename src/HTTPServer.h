#ifndef HTTP_SERVER_H_
#define HTTP_SERVER_H_

#include "esp_http_server.h"
#include "string.h"
#include "string"

typedef esp_err_t (*mhandler)( httpd_req * request );

class HTTPServer
{
private:
    httpd_handle_t mHTTPHandle;
    httpd_config_t mHTTPConfig;

public:
    HTTPServer();
    esp_err_t Start();
    esp_err_t Stop();
    esp_err_t RegisterErrorHandler(httpd_err_code_t error, httpd_err_handler_func_t handler_fn);
    esp_err_t Register(std::string uri, mhandler handler, httpd_method_t method = HTTP_GET, void * context = nullptr);
    esp_err_t Unregister(std::string uri);
    ~HTTPServer();
};

// intutive ai
#endif
