/* Dynamic variables used in web template ../data/index.html */
#include "home_dyn_vars.h"
char web_uptime[14]; // 000d 00:00:02
String web_ip;

void dyn_vars_setup(){
    web_ip = WiFi.localIP().toString();
}

void dyn_vars_loop(){
    sprintf(web_uptime, "%dd %02u:%02u:%02u",
                    (int)(millis()/1000/60/60/24), // days
                    (int)(millis()/1000/60/60)%24,	// hours
                    (int)(millis()/1000/60)%60,	   // minutes
                    (int)(millis()/1000)%60,		  // seconds
                    millis()/1000);				   // uptime in seconds);

}
