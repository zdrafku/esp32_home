#ifndef HOME_DYN_VARS_H
    #define HOME_DYN_VARS_H

    #include <Arduino.h>
    #include <WiFi.h> //ESP32

    extern char web_uptime[];
    extern String web_ip;
    void dyn_vars_setup();
    void dyn_vars_loop();
#endif