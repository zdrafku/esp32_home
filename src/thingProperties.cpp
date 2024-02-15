#include "thingProperties.h"
int hum;
bool relay_1;
bool relay_2;
bool relay_3;
CloudTemperature temp;

void pinToggle() {
  int pin = 2;
  digitalWrite(config.relays[pin], !digitalRead(config.relays[pin]));
  relay_1 = digitalRead(config.relays[pin]);
}

void initProperties(){
  ArduinoCloud.setBoardId(DEVICE_LOGIN_NAME);
  ArduinoCloud.setSecretDeviceKey(DEVICE_KEY);

  ArduinoCloud.addProperty(hum, READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(relay_1, READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(relay_2, READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(relay_3, READWRITE, ON_CHANGE, pinToggle);
  ArduinoCloud.addProperty(temp, READ, ON_CHANGE, NULL);
}

WiFiConnectionHandler ArduinoIoTPreferredConnection(SSID, PASS);

void ArduinoCloud_setup(){
  // Defined in thingProperties.h
  initProperties();

  // Connect to Arduino IoT Cloud
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);

  /*
     The following function allows you to obtain more information
     related to the state of network and IoT Cloud connection and errors
     the higher number the more granular information you’ll get.
     The default is 0 (only errors).
     Maximum is 4
 */
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();
}

void ArduinoCloud_loop(){
  hum = h;
  temp = t;
  relay_1 = digitalRead(config.relays[0]);
  relay_2 = digitalRead(config.relays[1]);
  relay_3 = digitalRead(config.relays[2]);

  ArduinoCloud.update();
}