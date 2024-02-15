/* Setting up pins based on ../data/home.conf and web requests */
#include "home_pins.h"

unsigned long relay_previousMillis[PIN_NUM];
bool laser_trigger = true;

void pins_setup(String type, int pins[], bool status[], String info[], int interval[]){
  // prepare GPIOs
  for (int pin = 0; pins[pin]; pin++) {
    pinMode(pins[pin], OUTPUT);
   if ( pin == config.door_opener_position -1 && type == "Relays" ) {
     digitalWrite(pins[pin], HIGH);
   } else {
      // set to OFF / NO
      digitalWrite(pins[pin], status[pin]);
      if (config.debug) {
        Serial.printf("%s pin: %d\n", type, pins[pin]);
        Serial.printf("%s info: %s\n", type, info[pin]);
        Serial.printf("%s pin default: %d\n", type, status[pin]);
        Serial.printf("%s active time: %d\n", type, interval[pin]);
        if ( type == "Relays" ) Serial.printf("%s humidity auto %%: %d\n", type, config.hum_auto[pin]);
     }
    }
  }
}

void pins_loop(){
  for (int pin = 0; pin < PIN_NUM; pin++) {
    if (!config.relays[pin]) continue;
    // Trigger relay if humidity threshold (../data/home.conf -> hum_auto) is reached and web auto_status (../data/home.conf -> auto_status) is On
    if (h >= config.hum_auto[pin] && !config.door_opener_position && config.auto_status[pin] && config.hum_auto[pin]) {
      if (config.switches[0] && !digitalRead(config.switches[pin])) continue;
      relay_previousMillis[pin] = millis();
      digitalWrite(config.relays[pin], true);
      if (config.debug) Serial.printf("Triggered humidity: %d\n", config.relays[pin]);
    }
    if ( pin == config.laser_pin_position -1 && config.intervals[pin] && config.door_opener_position) {
      if (digitalRead(config.switches[pin])) {
        // turn off laser after interval - config.intervals[config.laser_pin_position -1]
        if (millis() - relay_previousMillis[pin] >= (config.intervals[pin]*60*1000)) {
          laser_trigger = false;
          relay_previousMillis[pin] = 0;
          // pin HIGH to turn OFF laser
          digitalWrite(config.relays[pin], true);
        } else {
          laser_trigger = true;
        }
        // pin LOW to turn ON laser
        if (laser_trigger) digitalWrite(config.relays[pin], false);
      } else {
        // pin HIGH to turn OFF laser
        relay_previousMillis[pin] = millis();
        digitalWrite(config.relays[pin], true);
      }
    }
    if (config.auto_status[pin] && config.door_opener_position) {
      if (digitalRead(config.switches[pin])) {
        digitalWrite(config.relays[pin], false);
      } else {
        digitalWrite(config.relays[pin], true);
      }
    }
    if (config.auto_status[pin] && config.hum_auto[pin] && millis() - relay_previousMillis[pin] >= (config.intervals[pin]*60*1000)) {
      if (config.switches[0] && !digitalRead(config.switches[pin])) continue;
      // Turn off relay if humidity is under the threshold (../data/home.conf -> hum_auto) and the time has passed (../data/home.conf -> intervals) and web auto_status (../data/home.conf -> auto_status) is On
      digitalWrite(config.relays[pin], false);
      if (config.debug) Serial.printf("humidity stopped: %d\n", config.relays[pin]);
    }
    // door cycle hardware button/switch
    if ( pin == config.door_opener_position -1 && digitalRead(config.switches[config.door_opener_position -1]) != config.switches_defaults[pin]) {
      door_cycle(config.door_opener_position);
    }
    if (config.debug) {
      Serial.printf("Relay %d: %d\n",config.relays[pin], digitalRead(config.relays[pin]));
      Serial.printf("Switch %d: %d\n",config.switches[pin], digitalRead(config.switches[pin]));
    }
  }
  if (config.debug) {
    delay(1000);
    Serial.println();
  }
}

void door_cycle(int door_switch_pin) {
  if (door_switch_pin != config.door_opener_position) return;
  digitalWrite(config.relays[door_switch_pin -1], !digitalRead(config.relays[door_switch_pin -1]));
  delay(500);
  digitalWrite(config.relays[door_switch_pin -1], !digitalRead(config.relays[door_switch_pin -1]));
}