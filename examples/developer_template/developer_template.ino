#include "esp_log.h"
#include "Device.h"
#include "Provision.h"
#include "HttpsOTAUpdate.h"

static const char * TAG = "Main";
static char * S1 = "S01";

int i = 0;

Device *device;

esp_err_t _callback(char * slot, double data, char * str)
{
  
  ESP_LOGI(TAG, "%s data : %f ---- ", slot, data);

  return ESP_OK;
}

esp_err_t configCallback( char * var, double value)
{
  ESP_LOGI(TAG, "%s config : %f ---- ", var, value);
  
  return ESP_OK;
}

void setup() {

  Serial.begin(115200);
  esp_log_level_set("Message", ESP_LOG_VERBOSE);
  esp_log_level_set("Provision", ESP_LOG_VERBOSE);
  esp_log_level_set("*", ESP_LOG_VERBOSE);
  esp_log_level_set("Main", ESP_LOG_VERBOSE);
  esp_log_level_set("BLEProvision", ESP_LOG_VERBOSE);

  // put your setup code here, to run once:
 Serial.setDebugOutput(true);
 
  DefaultEventLoop::DefaultEventLoopCreate();
  device = new Device("YOUR_TEMPLATE_ID",_callback,configCallback);

  esp_err_t err = ESP_OK;
  
  Provision * provision = new Provision(device);
  err = provision->StartProvisioningWiFi("DISPLAY_NAME");
  if (err != ESP_OK)
  {
    ESP_LOGV(TAG, "Provisioning failed due to %s", err);
    return;
  }

  provision->Connect();
}

void loop() {
  
  device->Send(S1,i,0);
  i++;
  delay(10000);
}