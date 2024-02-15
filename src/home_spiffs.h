#ifndef HOME_SPIFFS_H
    #define HOME_SPIFFS_H

    #include <FS.h>
    #include <SPIFFS.h>

    #define FORMAT_SPIFFS_IF_FAILED true
    static const char * config_file = "/home.conf";
    static const int PIN_NUM = 6; // hypothetically up to 5 pins (0-4 + 2 for teminate bytes) pins

    typedef struct conf {
        bool debug;
        int relays[PIN_NUM];
        bool relays_defaults[PIN_NUM];
        int switches[PIN_NUM];
        bool switches_defaults[PIN_NUM];
        String info[PIN_NUM];
        bool auto_status[PIN_NUM];
        int intervals[PIN_NUM];
        int dht_pin;
        int dht_type;
        int hum_auto[PIN_NUM];
        int reset_timer;
        int door_opener_position;
        int laser_pin_position;
        String wifi_ssid;
        String wifi_pass;
        bool ap_enabled;
        String ap_ssid;
        String ap_pass;
        String ota_host;
        String ota_pass;
        String smcd_id;
        String smcd_host;
        int smcd_port;
    } conf_t;
    extern conf_t config;

    void config_gen();
    void split_var(String, char *, String[]);
    void split_var(String, char *, int[]);
    void split_var(String, char *, bool[]);
#endif