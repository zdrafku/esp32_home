#ifndef HOME_RELAYS_H
  #define HOME_RELAYS_H

  #include "home_dht.h"
  
  extern unsigned long relay_previousMillis[PIN_NUM]; // will store last time ralay was reset
  
  void pins_setup(String, int[], bool[], String[], int[]);
  void pins_loop();
  void door_cycle(int);
#endif
