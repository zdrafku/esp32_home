/* Parse home.conf file from SPIFFS and load it to the ESP32's configuration */
#include "home_spiffs.h"
conf_t config;

bool spiffs_setup() {
  if(!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return false;
  }

  /* List files in SPIFFS' root where the config file is located */
  File root = SPIFFS.open("/");
  File file = root.openNextFile();
  Serial.println();
  Serial.println("SPIFFS content: ");
  while(file){
    Serial.print("FILE: ");
    Serial.println(file.name());

    file = root.openNextFile();
  }
  Serial.println();
  return true;
}

/* Parse the config file and assign the values to the conf structure (config) */
void config_gen() {
  if ( !spiffs_setup() ) return;
  File conf_file = SPIFFS.open(config_file);
  if ( !config_file ) {
    Serial.printf("Config file %s is not uploaded in SPIFFS", config_file);
    return;
  }

  while ( conf_file.available() ) {
    String line = conf_file.readStringUntil('\n');
    line.trim(); // remove white spaces if any
    if ( line.startsWith("#") || line.startsWith(";") ) continue; // skip lines with comment

    int conf_key_num = line.indexOf('=');
    String conf_key = line.substring(0, conf_key_num);
    conf_key.trim(); // remove white spaces if any
    String conf_value = line.substring(conf_key.length() + 1, line.length());
    conf_value.trim(); // remove white spaces if any

    if ( conf_key == "debug" ) config.debug = bool(conf_value.toInt());
    if ( conf_key == "relays" ) split_var(conf_value, ",", config.relays);
    if ( conf_key == "relays_defaults" ) split_var(conf_value, ",", config.relays_defaults);
    if ( conf_key == "switches" ) split_var(conf_value, ",", config.switches);
    if ( conf_key == "switches_defaults" ) split_var(conf_value, ",", config.switches_defaults);
    if ( conf_key == "info" ) split_var(conf_value, ",", config.info);
    if ( conf_key == "auto_status" ) split_var(conf_value, ",", config.auto_status);
    if ( conf_key == "intervals" ) split_var(conf_value, ",", config.intervals);
    if ( conf_key == "dht_pin" ) config.dht_pin = conf_value.toInt();
    if ( conf_key == "dht_type" ) {
      if ( conf_value == "DHT11" ) config.dht_type = 11;
      if ( conf_value == "DHT12" ) config.dht_type = 12;
      if ( conf_value == "DHT21" ) config.dht_type = 21;
      if ( conf_value == "DHT22" ) config.dht_type = 22;
      if ( conf_value == "AM2301" ) config.dht_type = 21;
    }
    if ( conf_key == "hum_auto" ) split_var(conf_value, ",", config.hum_auto);
    if ( conf_key == "reset_timer" ) config.reset_timer = conf_value.toInt();
    if ( conf_key == "door_opener_pos" ) config.door_opener_position = conf_value.toInt();
    if ( conf_key == "laser_pin_pos" ) config.laser_pin_position = conf_value.toInt();
    if ( conf_key == "wifi_ssid" ) config.wifi_ssid = *&conf_value;
    if ( conf_key == "wifi_pass" ) config.wifi_pass = *&conf_value;
    if ( conf_key == "ap_enabled" ) config.ap_enabled = bool(conf_value.toInt());
    if ( conf_key == "ap_ssid" ) config.ap_ssid = *&conf_value;
    if ( conf_key == "ap_pass" ) config.ap_pass = *&conf_value;
    if ( conf_key == "ota_host" ) config.ota_host = *&conf_value;
    if ( conf_key == "ota_pass" ) config.ota_pass = *&conf_value;
    if ( conf_key == "smcd_id" ) config.smcd_id = *&conf_value;
    if ( conf_key == "smcd_host" ) config.smcd_host = *&conf_value;
    if ( conf_key == "smcd_port" ) config.smcd_port = conf_value.toInt();
  }

  if (config.debug) {
    Serial.printf("config.relays: %d\n", config.relays[1]);
    Serial.printf("config.relays_defaults: %d\n", config.relays_defaults[1]);
    Serial.printf("config.switches: %d\n", config.switches[1]);
    Serial.printf("config.switches_defaults: %d\n", config.switches_defaults[1]);
    Serial.printf("config.info: %s\n", config.info[1]);
    Serial.printf("config.auto_status: %d\n", config.auto_status[1]);
    Serial.printf("config.intervals: %d\n", config.intervals[1]);
    Serial.printf("config.dht_pin: %d\n", config.dht_pin);
    Serial.printf("config.dht_type: %d\n", config.dht_type);
    Serial.printf("config.hum_auto: %d\n", config.hum_auto[1]);
    Serial.printf("config.reset_timer: %d\n", config.reset_timer);
    Serial.printf("config.door_opener_position: %d\n", config.door_opener_position);
    Serial.printf("config.laser_pin_position: %d\n", config.laser_pin_position);
    Serial.printf("config.wifi_ssid: %s\n", config.wifi_ssid);
    Serial.printf("config.wifi_pass: %s\n", config.wifi_pass);
    Serial.printf("config.ap_enabled: %d\n", config.ap_enabled);
    Serial.printf("config.ap_ssid: %s\n", *&config.ap_ssid);
    Serial.printf("config.ap_pass: %s\n", config.ap_pass);
    Serial.printf("config.ota_host: %s\n", config.ota_host);
    Serial.printf("config.ota_pass: %s\n", config.ota_pass);
    Serial.printf("config.smcd_id: %s\n", config.smcd_id);
    Serial.printf("config.smcd_host: %s\n", config.smcd_host);
    Serial.printf("config.smcd_port: %d\n", config.smcd_port);
  }
}

void split_var(String value, char *separator, String variable[]) { // String
  int i = 0;
  int startIndex = 0;
  int endIndex = value.indexOf(separator);

  while (endIndex != -1) {
    variable[i++] = value.substring(startIndex, endIndex);
    startIndex = endIndex + 1;
    endIndex = value.indexOf(separator, startIndex);
  }
  variable[i] = value.substring(startIndex, value.length());
}

void split_var(String value, char *separator, int variable[]) { // int
  int i = 0;
  int startIndex = 0;
  int endIndex = value.indexOf(separator);

  while (endIndex != -1) {
    variable[i++] = value.substring(startIndex, endIndex).toInt();
    startIndex = endIndex + 1;
    endIndex = value.indexOf(separator, startIndex);
  }
  variable[i] = value.substring(startIndex, value.length()).toInt();
}

void split_var(String value, char *separator, bool variable[]) { // bool
  int i = 0;
  int startIndex = 0;
  int endIndex = value.indexOf(separator);

  while (endIndex != -1) {
    variable[i++] = value.substring(startIndex, endIndex).toInt();
    startIndex = endIndex + 1;
    endIndex = value.indexOf(separator, startIndex);
  }
  variable[i] = value.substring(startIndex, value.length()).toInt();
}
