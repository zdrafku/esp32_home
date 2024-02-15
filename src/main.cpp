/*
 * - Controlls bath fan based on humidity
 * - Controlls garage door and lights
 * - Monitor view web
 * - Sends data to Arduino IoT Cloud - https://create.arduino.cc/iot/dashboards
 *
 * GitHub project: https://github.com/zdrafku/esp32_home_PlatformIO
 * Author: Mr. Psycho
 */
#include <Arduino.h>
#include "home_spiffs.h"
#include "home_dyn_vars.h"
#include "home_wifi.h"
#include "home_ota.h"
#include "home_dht.h"
#include "home_pins.h"
#include "home_web_spiffs.h"
#include "thingProperties.h"

void setup() {
  Serial.begin(115200);
  config_gen();
  WiFi_setup();
  OTA_Setup();
  pins_setup("Relays", config.relays, config.relays_defaults, config.info, config.intervals);
  pins_setup("Switches", config.switches, config.switches_defaults, config.info, config.intervals);
  dyn_vars_setup();
  web_spiffs_setup();
  ArduinoCloud_setup();
}

void loop() {
  dyn_vars_loop();
  ArduinoOTA.handle();
  if (*&config.dht_pin && *&config.dht_type) Sensor_loop();
  /* Reconnect to WiFi if the connection has dropped */
  if ( ( WiFi.status() != WL_CONNECTED || WiFi.status() == WL_NO_SSID_AVAIL ) && !config.ap_enabled ) {
    WiFi_setup();
    delay(5000);
  }
  pins_loop();
  ArduinoCloud_loop();
}
