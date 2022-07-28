#define LOG_LOCAL_LEVEL ESP_LOG_NONE

#include "WiFi.h"
#include "WiFiEventGroup.h"
#include "WiFiSTAEventGroup.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>

#include <esp_err.h>
#include <esp_wifi.h>
#include <esp_event_loop.h>
#include "lwip/ip_addr.h"
#include "freertos/event_groups.h"
#include "lwip/opt.h"
#include "lwip/err.h"
#include "lwip/dns.h"
#include "esp_ipc.h"

#include "esp_log.h"
#include "esp_netif.h"
#include <vector>
#include "sdkconfig.h"

static const char * TAG = "WiFi";

/* There should be one call to wifi instance */
void WiFi::wifi_event_handler(void * handlerArgument, esp_event_base_t eventBase, int32_t eventId, void * eventData)
{

    WiFi * wifi = (WiFi *) handlerArgument;

    ESP_LOGI(TAG, "%s WITH ID %d", eventBase, eventId);
    switch (eventId)
    {
        case WIFI_EVENT_WIFI_READY:               /**< ESP32 WiFi ready */
        {
            wifi->WiFiScanEventGroup::EventGroupSetBits(WiFiScanEventGroup::EG_SCAN_IDLE);
            wifi->WiFiSTAEventGroup::EventGroupSetBits(WiFiSTAEventGroup::EG_STA_IDLE);
            wifi->WiFiAPEventGroup::EventGroupSetBits(WiFiAPEventGroup::EG_AP_IDLE);
            break;
        }
        case WIFI_EVENT_SCAN_DONE:                /**< ESP32 finish scanning AP */
        {
            wifi->WiFiSTAEventGroup::EventGroupClearBits(WiFiSTAEventGroup::EG_STA_IDLE);
            wifi->WiFiScanEventGroup::EventGroupClearBits(WiFiScanEventGroup::EG_SCAN_IDLE);
            wifi->WiFiScanEventGroup::EventGroupSetBits(WiFiScanEventGroup::EG_SCAN_COMPLETED);
            // wifi->WiFiSTAEventGroup::EventGroupWaitBits(WiFiSTAEventGroup::EG_STA_CONNECTED | WiFiSTAEventGroup::EG_STA_CONNECT_FAILED);
            break;
        }
        case WIFI_EVENT_STA_START:                /**< ESP32 station start */
        {
            wifi->WiFiSTAEventGroup::EventGroupClearBits(WiFiSTAEventGroup::EG_STA_IDLE);
            wifi->WiFiSTAEventGroup::EventGroupSetBits(WiFiSTAEventGroup::EG_STA_START);
            wifi->WiFiSTAEventGroup::EventGroupClearBits(WiFiSTAEventGroup::EG_STA_STOP);
            break;
        }
        case WIFI_EVENT_STA_STOP:                 /**< ESP32 station stop */
        {
            wifi->WiFiSTAEventGroup::EventGroupClearBits(WiFiSTAEventGroup::EG_STA_IDLE);
            wifi->WiFiSTAEventGroup::EventGroupClearBits(WiFiSTAEventGroup::EG_STA_START);
            wifi->WiFiSTAEventGroup::EventGroupSetBits(WiFiSTAEventGroup::EG_STA_STOP);
            break;
        }
        case WIFI_EVENT_STA_CONNECTED:            /**< ESP32 station connected to AP */
        {
            wifi->WiFiSTAEventGroup::EventGroupClearBits(WiFiSTAEventGroup::EG_STA_IDLE);
            wifi->WiFiSTAEventGroup::EventGroupSetBits(WiFiSTAEventGroup::EG_STA_CONNECTED);
            wifi->WiFiSTAEventGroup::EventGroupClearBits(WiFiSTAEventGroup::EG_STA_DISCONNECT);
            wifi->WiFiSTAEventGroup::EventGroupClearBits(WiFiSTAEventGroup::EG_STA_CONNECTING);
            break;
        }
        case WIFI_EVENT_STA_DISCONNECTED:         /**< ESP32 station disconnected from AP */
        {
            wifi->WiFiSTAEventGroup::EventGroupClearBits(WiFiSTAEventGroup::EG_STA_IDLE);
            wifi->WiFiSTAEventGroup::EventGroupClearBits(WiFiSTAEventGroup::EG_STA_CONNECTED);
            wifi->WiFiSTAEventGroup::EventGroupSetBits(WiFiSTAEventGroup::EG_STA_DISCONNECT);
            if (wifi->GetAutoReconnect())
            {
                wifi->Connect();
            }
            break;
        }
        case WIFI_EVENT_STA_AUTHMODE_CHANGE:      /**< the auth mode of AP connected by ESP32 station changed */
        {
            /* NOT NECESSARY */
            ESP_LOGE(TAG, "WIFI_EVENT_STA_AUTHMODE_CHANGE");
            wifi->WiFiSTAEventGroup::EventGroupClearBits(WiFiSTAEventGroup::EG_STA_IDLE);
            break;
        }

        case WIFI_EVENT_STA_WPS_ER_SUCCESS:       /**< ESP32 station wps succeeds in enrollee mode */
        {
            wifi->WiFiSTAEventGroup::EventGroupClearBits(WiFiSTAEventGroup::EG_STA_IDLE);
            break;
        }
        case WIFI_EVENT_STA_WPS_ER_FAILED:        /**< ESP32 station wps fails in enrollee mode */
        {
            ESP_LOGE(TAG, "WIFI_EVENT_STA_WPS_ER_FAILED");
            wifi->WiFiSTAEventGroup::EventGroupClearBits(WiFiSTAEventGroup::EG_STA_IDLE);
            break;
        }
        case WIFI_EVENT_STA_WPS_ER_TIMEOUT:       /**< ESP32 station wps timeout in enrollee mode */
        {
            ESP_LOGE(TAG, "WIFI_EVENT_STA_WPS_ER_TIMEOUT");
            wifi->WiFiSTAEventGroup::EventGroupClearBits(WiFiSTAEventGroup::EG_STA_IDLE);
            break;
        }
        case WIFI_EVENT_STA_WPS_ER_PIN:           /**< ESP32 station wps pin code in enrollee mode */
        {
            ESP_LOGE(TAG, "WIFI_EVENT_STA_WPS_ER_PIN");
            wifi->WiFiSTAEventGroup::EventGroupClearBits(WiFiSTAEventGroup::EG_STA_IDLE);
            break;
        }
        case WIFI_EVENT_STA_WPS_ER_PBC_OVERLAP:   /**< ESP32 station wps overlap in enrollee mode */
        {
            ESP_LOGE(TAG, "WIFI_EVENT_STA_WPS_ER_PBC_OVERLAP");
            wifi->WiFiSTAEventGroup::EventGroupClearBits(WiFiSTAEventGroup::EG_STA_IDLE);
            break;
        }

        case WIFI_EVENT_AP_START:                 /**< ESP32 soft-AP start */
        {
            wifi->WiFiAPEventGroup::EventGroupClearBits(WiFiAPEventGroup::EG_AP_IDLE);
            wifi->WiFiAPEventGroup::EventGroupSetBits(WiFiAPEventGroup::EG_AP_START);
            wifi->WiFiAPEventGroup::EventGroupClearBits(WiFiAPEventGroup::EG_AP_STOP);
            break;
        }
        case WIFI_EVENT_AP_STOP:                  /**< ESP32 soft-AP stop */
        {
            wifi->WiFiAPEventGroup::EventGroupClearBits(WiFiAPEventGroup::EG_AP_IDLE);
            wifi->WiFiAPEventGroup::EventGroupClearBits(WiFiAPEventGroup::EG_AP_START);
            wifi->WiFiAPEventGroup::EventGroupSetBits(WiFiAPEventGroup::EG_AP_STOP);
            break;
        }
        case WIFI_EVENT_AP_STACONNECTED:          /**< a station connected to ESP32 soft-AP */
        {
            wifi->WiFiAPEventGroup::EventGroupClearBits(WiFiAPEventGroup::EG_AP_IDLE);
            wifi->WiFiAPEventGroup::EventGroupSetBits(WiFiAPEventGroup::EG_AP_STA_CONNECTED);
            wifi->WiFiAPEventGroup::EventGroupClearBits(WiFiAPEventGroup::EG_AP_STA_DISCONNECTED);
            break;
        }
        case WIFI_EVENT_AP_STADISCONNECTED:       /**< a station disconnected from ESP32 soft-AP */
        {
            wifi->WiFiAPEventGroup::EventGroupClearBits(WiFiAPEventGroup::EG_AP_IDLE);
            wifi->WiFiAPEventGroup::EventGroupClearBits(WiFiAPEventGroup::EG_AP_STA_CONNECTED);
            wifi->WiFiAPEventGroup::EventGroupSetBits(WiFiAPEventGroup::EG_AP_STA_DISCONNECTED);
            break;
        }
        case WIFI_EVENT_AP_PROBEREQRECVED:        /**< Receive probe request packet in soft-AP interface */
        {
            wifi->WiFiAPEventGroup::EventGroupClearBits(WiFiAPEventGroup::EG_AP_IDLE);
            break;
        }

        case WIFI_EVENT_FTM_REPORT:               /**< Receive report of FTM procedure */
        {
            ESP_LOGE(TAG, "WIFI_EVENT_FTM_REPORT");
            break;
        }

        /* Add next events after this only */
        case WIFI_EVENT_STA_BSS_RSSI_LOW:         /**< AP's RSSI crossed configured threshold */
        {
            ESP_LOGE(TAG, "WIFI_EVENT_STA_BSS_RSSI_LOW");
            wifi->WiFiSTAEventGroup::EventGroupClearBits(WiFiSTAEventGroup::EG_STA_IDLE);
            break;
        }
        case WIFI_EVENT_ACTION_TX_STATUS:         /**< Status indication of Action Tx operation */
        {
            ESP_LOGE(TAG, "WIFI_EVENT_ACTION_TX_STATUS");
            break;
        }
        case WIFI_EVENT_ROC_DONE:                 /**< Remain-on-Channel operation complete */
        {
            ESP_LOGE(TAG, "WIFI_EVENT_ROC_DONE");
            break;
        }

        case WIFI_EVENT_STA_BEACON_TIMEOUT:       /**< ESP32 station beacon timeout */
        {
            ESP_LOGE(TAG, "WIFI_EVENT_STA_BEACON_TIMEOUT");
            break;
        }

        case WIFI_EVENT_MAX:                      /**< Invalid WiFi event ID */
        {
            ESP_LOGE(TAG, "WIFI_EVENT_MAX");
            break;
        }

        default:
        {
            break;
        }
    }
}

esp_err_t WiFi::Init()
{
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    return ::esp_wifi_init(&cfg);
}

esp_err_t WiFi::Deinit()
{
    ESP_LOGI(TAG, "%s", __func__);
    return ::esp_wifi_deinit();
}

esp_err_t WiFi::Start()
{
    ESP_LOGI(TAG, "%s", __func__);
    return ::esp_wifi_start();
}

esp_err_t WiFi::Stop()
{
    ESP_LOGI(TAG, "%s", __func__);
    return ::esp_wifi_stop();
}

esp_event_handler_instance_t WiFi::mEventHandlerInstance = nullptr;

esp_err_t WiFi::RegisterHandler()
{
    ESP_LOGI(TAG, "%s", __func__);
    esp_err_t err = esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, wifi_event_handler, this, &mEventHandlerInstance);
    if (err)
    {
        ESP_LOGE(TAG, "REGISTERED NOT HANDLER %d (0x%X)", err, err);
    }
    return err;
}

esp_err_t WiFi::UnregisterHandler()
{
    ESP_LOGI(TAG, "%s", __func__);
    esp_err_t err = esp_event_handler_instance_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID, mEventHandlerInstance);
    if (err)
    {
        ESP_LOGE(TAG, "HANDLER NOT REGISTERED  %d (0x%X)", err, err);
    }
    return err;
}

wifi_mode_t WiFi::GetMode()
{
    wifi_mode_t mode;
    esp_wifi_get_mode(&mode);
    return mode;
}

WiFi::WiFi() : NVStorage(), NetworkInterface(), DefaultEventLoop(), WiFiAccessPoint(), WiFiStation(), WiFiEventGroup(), WiFiStationScan()
{
    ESP_ERROR_CHECK(Init());
    ESP_ERROR_CHECK(RegisterHandler());
    ESP_ERROR_CHECK(SetDefaultStorage());
}

esp_err_t WiFi::SetDefaultStorage(wifi_storage_t wifiStorage)
{
    return ::esp_wifi_set_storage(wifiStorage);
}

WiFi::~WiFi()
{
    ESP_LOGI(TAG, "WiFi deinitialized");
    UnregisterHandler();
    Stop();
    Deinit();
}

int32_t WiFi::GetChannel(void)
{
    uint8_t primaryChan = 0;
    esp_err_t err = ESP_OK;
    wifi_second_chan_t secondChan = WIFI_SECOND_CHAN_NONE;
    err = ::esp_wifi_get_channel(&primaryChan, &secondChan);
    ESP_ERROR_CHECK_WITHOUT_ABORT(err);
    return primaryChan;
}

/* Passed */
esp_err_t WiFi::SetMode(wifi_mode_t wifiMode)
{
    esp_err_t err;
    err = ::esp_wifi_set_mode(wifiMode);
    ESP_ERROR_CHECK_WITHOUT_ABORT(err);
    return err;
}
/* Passed */
esp_err_t WiFi::EnableSTA(bool enable)
{
    ESP_LOGI(TAG, "%s", __func__);
    // WiFiStation::Create();

    wifi_mode_t currentMode = GetMode();
    bool isEnabled = ( ( currentMode & WIFI_MODE_STA ) != 0 );

    if (isEnabled != enable)
    {
        if (enable)
        {
            return SetMode((wifi_mode_t) ( currentMode | WIFI_MODE_STA ));
        }
        return SetMode((wifi_mode_t) ( currentMode & ( ~WIFI_MODE_STA ) ));
    }
    return ESP_OK;
}

/* Passed */
esp_err_t WiFi::EnableAP(bool enable)
{
    ESP_LOGI(TAG, "%s", __func__);

    wifi_mode_t currentMode = GetMode();
    bool isEnabled = ( ( currentMode & WIFI_MODE_AP ) != 0 );

    if (isEnabled != enable)
    {
        if (enable)
        {
            return SetMode((wifi_mode_t) ( currentMode | WIFI_MODE_AP ));
        }
        return SetMode((wifi_mode_t) ( currentMode & ( ~WIFI_MODE_AP ) ));
    }
    return ESP_OK;
}
