#ifndef AIOT_H
  #define AIOT_H
  #include "home_spiffs.h"
  #include "home_dht.h"
  void ArduinoCloud_setup();
  void ArduinoCloud_loop();

  // Code generated by Arduino IoT Cloud, DO NOT EDIT.

  #include <ArduinoIoTCloud.h>
  #include <Arduino_ConnectionHandler.h>

  const char DEVICE_LOGIN_NAME[]  = "";

  const char SSID[]               = "";    // Network SSID (name)
  const char PASS[]               = " ";    // Network password (use for WPA, or use as key for WEP)
  const char DEVICE_KEY[]         = "";    // Secret device password

  void pinToggle();
  void initProperties();
#endif