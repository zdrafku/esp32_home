/* WiFi setup */
#include "home_wifi.h"

int WIFI_TIMEOUT = 30; // seconds

void WiFi_setup() {
  int WiFiCurrentTime = millis();

  WiFi.mode(WIFI_STA); // OTA / WiFi Upload

  if (config.ap_enabled && config.ap_ssid != "") {
    WiFi.softAP(config.ap_ssid.c_str(), config.ap_pass.c_str());
    //IPAddress ap_ip(192, 168, 1, 111);      // fix IP of the server
    //IPAddress ap_subnet(255, 255, 255, 0);
    //IPAddress ap_gateway(0, 0, 0, 0);
    //WiFi.softAPConfig(ap_ip, ap_gateway, ap_subnet);
    Serial.println("AP Mode");
    Serial.printf("WiFI SSID: %s\n", config.ap_ssid.c_str());
    Serial.printf("WiFi pass: %s\n", config.ap_pass.c_str());
    return;
  }

  if (config.wifi_ssid == "") return;
  // Connect to WiFi network
  Serial.printf("\nConnecting to %s", config.wifi_ssid.c_str());
  WiFi.begin(config.wifi_ssid.c_str(), config.wifi_pass.c_str());

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if ( WiFiCurrentTime*1000 > (millis() - WIFI_TIMEOUT*1000) ) {
      Serial.printf("\nFailed to connect to %s\n", config.wifi_ssid.c_str());
      return;
    }
  }
  Serial.println("connected");
  // Print the IP address
  delay(1000);
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}
