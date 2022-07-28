#define LOG_LOCAL_LEVEL ESP_LOG_NONE

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "HTTPServer.h"
#include "esp_log.h"
#include "sdkconfig.h"

static const char * TAG = "HTTPServer";

HTTPServer::HTTPServer()
{
    ESP_LOGI(TAG, "Init");
    mHTTPConfig.task_priority = tskIDLE_PRIORITY + 5;
    mHTTPConfig.stack_size = 4096;
    mHTTPConfig.core_id = tskNO_AFFINITY;
    mHTTPConfig.server_port = 80;
    mHTTPConfig.ctrl_port = 32768;
    mHTTPConfig.max_open_sockets = 7;
    mHTTPConfig.max_uri_handlers = 8;
    mHTTPConfig.max_resp_headers = 8;
    mHTTPConfig.backlog_conn = 5;
    mHTTPConfig.lru_purge_enable = false;
    mHTTPConfig.recv_wait_timeout = 5;
    mHTTPConfig.send_wait_timeout = 5;
    mHTTPConfig.global_user_ctx = NULL;
    mHTTPConfig.global_user_ctx_free_fn = NULL;
    mHTTPConfig.global_transport_ctx = NULL;
    mHTTPConfig.global_transport_ctx_free_fn = NULL;
    mHTTPConfig.open_fn = NULL;
    mHTTPConfig.close_fn = NULL;
    mHTTPConfig.uri_match_fn = NULL;
}

esp_err_t HTTPServer::Start()
{
    ESP_LOGI(TAG, "Started");
    return ::httpd_start(&mHTTPHandle, &mHTTPConfig);
}

esp_err_t HTTPServer::Stop()
{
    ESP_LOGI(TAG, "Stopped");
    return ::httpd_stop(&mHTTPHandle);
}

esp_err_t HTTPServer::RegisterErrorHandler(httpd_err_code_t error, httpd_err_handler_func_t handler_fn)
{
    return ::httpd_register_err_handler(mHTTPHandle, error, handler_fn);
}

esp_err_t HTTPServer::Register(std::string uri, mhandler handler, httpd_method_t method, void * context)
{
    struct httpd_uri httpuri;
    httpuri.method = method;
    httpuri.handler = handler;
    httpuri.uri = uri.c_str();
    httpuri.user_ctx = context;

    return ::httpd_register_uri_handler(mHTTPHandle, &httpuri);
}

esp_err_t HTTPServer::Unregister(std::string uri)
{
    return ::httpd_unregister_uri(mHTTPHandle, uri.c_str());
}

HTTPServer::~HTTPServer()
{
    ESP_LOGI(TAG, "deleted");
    Stop();
}
