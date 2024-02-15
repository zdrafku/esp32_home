/* Uppload firmware via OAT (Over The Air) */
#include "home_ota.h"

// OTA / WiFi Upload
void OTA_Setup() {
  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);
  if (config.ota_host == "") return;

  // Hostname defaults to esp8266-[ChipID]
  // Hostname defaults to esp32-[MAC]
  ArduinoOTA.setHostname(config.ota_host.c_str());

  // No authentication by default
  ArduinoOTA.setPassword(config.ota_pass.c_str());

  ArduinoOTA.onStart([]() {
    Serial.println("OTA Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nOTA End");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("OTA Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("OTA Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("OTA Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("OTA Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("OTA Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("OTA End Failed");
  });
  ArduinoOTA.begin();
  Serial.println("OTA Ready");
  Serial.printf("OTA Host: %s\n", config.ota_host.c_str());
  Serial.printf("OTA pass: %s\n", config.ota_pass.c_str());
}
