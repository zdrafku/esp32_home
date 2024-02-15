### What it does?
Trigger relays based on humidity level and manually via web interface. Also it can trigger the relays with external switches

### Purpose
* Control home bath fan :D
* Upload firmware/filesystem image via web
* Upload firmware via OTA with Arduino IDE

### IDE to compile and upload the source
[VS Code and PlatformIO for ESP32](https://randomnerdtutorials.com/vs-code-platformio-ide-esp32-esp8266-arduino/)

### Ardiono IDE
* You have to rename src/main.cpp to src/src.ino
* You have to modify src/home_web_spiffs.h

### Arduino IDE 1.8.x OTA Filesystem upload (no support for 2.x yet)
[Install ESP32 Filesystem Uploader in Arduino IDE](https://randomnerdtutorials.com/install-esp32-filesystem-uploader-arduino-ide/) 

### Needed libraries
* [WiFi](https://github.com/espressif/arduino-esp32/tree/master/libraries/WiFi)
* [Adafruit Unified Sensor Driver](https://github.com/adafruit/Adafruit_Sensor)
* [DHT sensor library](https://github.com/adafruit/DHT-sensor-library)
* [AsyncTCP](https://github.com/me-no-dev/AsyncTCP)
* [FreeRTOS](https://github.com/espressif/arduino-esp32/tree/master/tools/sdk/esp32c3/include/freertos/include/freertos)
* [ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer)
* [AsyncElegantOTA.h](https://github.com/ayushsharma82/AsyncElegantOTA)
* [ArduinoIoTCloud](https://github.com/arduino-libraries/ArduinoIoTCloud)
* [Arduino_ESP32_OTA](https://github.com/arduino-libraries/Arduino_ESP32_OTA)

### Hardware
* ESP32 + Development Board with 38 pins - [external link](https://www.aliexpress.com/item/32959541446.html)
* DHT11 - [external link](https://www.aliexpress.com/item/32769460765.html)
* Relays - [external link](https://www.aliexpress.com/item/1005001903120199.html)
* Magnetic Switch - [external link](https://www.aliexpress.com/item/1005001557491389.html)

### ArduinoIoTCloud
It requires manual work. You have to:
* You have to set the partitions to minimal SPIFFS - 1.9M with OTA/190K SPIFFS
* generate thingProperties.h at [Arduino IoT Cloud](https://create.arduino.cc/iot/things)
* separate thingProperties.h in [thingProperties.h](src/thingProperties.h) and [thingProperties.cpp](src/thingProperties.cpp) files like the examples