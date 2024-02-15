#ifndef HOME_WEB_SPIFFS_H
    #define HOME_WEB_SPIFFS_H

    // PlatformIO
    #include "FreeRTOS.h"
    // Arduino IDE
    // #include "/Users/mrpsycho/Library/Arduino15/packages/esp32/hardware/esp32/2.0.6/tools/sdk/esp32/include/freertos/include/freertos/FreeRTOS.h"
    #include "AsyncTCP.h"
    #include "ESPAsyncWebServer.h"
    #include "SPIFFS.h"
    #include "home_dyn_vars.h"
    #include "home_spiffs.h"
    #include "home_dht.h"
    #include "home_pins.h"

    void web_spiffs_setup();
#endif
