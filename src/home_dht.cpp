/* Get temperature and humidity from DHT sensor */
#include "home_dht.h"

// current temperature & humidity, updated in loop()
int t = 0;
int h = 0;

// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long previousMillis = 0; // will store last time DHT was updated

// Updates DHT readings every 10 seconds
const char interval = 10;

int temp_last_state;
int hum_last_state;

void Sensor_loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis <= interval * 1000) return;
  // save the last time you updated the DHT values
  previousMillis = currentMillis;

  DHT dht(*&config.dht_pin, *&config.dht_type);

  dht.begin();
  if (!dht.read()) return;

  // Read temperature as Celsius (the default)
  int newT = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  //float newT = dht.readTemperature(true);
  // if temperature read failed, don't change t value
  if (isnan(newT)) {
    Serial.println("Failed to read from DHT sensor!");
  } else {
    if (newT <= 100) t = newT;
    if (config.debug) Serial.printf("Temperature (°C): %d\n", t);
  }
  // Read Humidity
  int newH = dht.readHumidity();
  // if humidity read failed, don't change h value 
  if (isnan(newH)) {
    Serial.println("Failed to read from DHT sensor!");
  } else {
    if (newH < 100) h = newH;
    if (config.debug) Serial.printf("Humidity %%: %d\n", h);
  }
  if ( t != temp_last_state || h != hum_last_state ) {
    temp_last_state = t;
    hum_last_state = h;
//      SmHC_DHT_sensor(temp_last_state, hum_last_state);
  }
}
