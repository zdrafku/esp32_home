#ifndef HOME_DHT_H
    #define HOME_DHT_H

    #include <DHT.h>
    #include "home_spiffs.h"

    extern int t;
    extern int h;

    void Sensor_loop();
#endif
