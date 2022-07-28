#include "esp_log.h"
#include "Device.h"
#include "Provision.h"

static const char * TAG = "Main";
static char * S7 = "S07";
static char * S6 = "S06";
  
Device *device;

esp_err_t _callback(char * slot, double data, char * str)
{
  ESP_LOGI(TAG, "%s data : %f ---- ", slot, data);
  if(!strcmp(slot,S6)){
    ESP_LOGI(TAG, "%s data - if : %f ---- ", slot, data);
    }
  //Serial.print(data);
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
  device = new Device("YOUR_DEVICE_NAME","TEMPLATE_ID",_callback);

  esp_err_t err = ESP_OK;
  
  Provision * provision = new Provision(device);
  err = provision->StartProvisioningWiFi();
  if (err != ESP_OK)
  {
    ESP_LOGV(TAG, "Provisioning failed due to %s", err);
    return;
  }
  //provision->StartProvisioningBLE();
  //ESP_LOGE(TAG, "errr %s", err);
  provision->Connect();
}

void loop() {
  //put your main code here, to run repeatedly:

  char * Text = (char*)"Test";
  
  device->Send(S7,Text);
  delay(10000);
}