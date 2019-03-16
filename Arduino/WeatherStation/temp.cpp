#include "temp.h"
#include "debug.h"
#include "display.h"

#include <math.h>
#include <DHT.h>

extern DHT dht;

int last_average = 0;
long last_temp = 0;
signed char temps[TEMP_BUFFER_SIZE];
char temp_index = 0;

int temp_get_temp() {
  return last_average;
}

void temp_load() {
 if(millis()>last_temp+1000) {
    last_temp = millis();
    temps[temp_index++] = dht.readTemperature();
  }
  if(temp_index==TEMP_BUFFER_SIZE) {
    temp_index=0;
    float average_temp = 0;
    byte count_correction = 0;
    debugln("");
    for(char i=0; i<TEMP_BUFFER_SIZE; i++) {
      if(temps[i]>0) {
        debug(temps[i]);
        debug(", ");
        average_temp += temps[i]; 
      } else {
        count_correction++;
      }
    }
    average_temp /= (TEMP_BUFFER_SIZE-count_correction);
    debug("\nAverage=");
    debugln(average_temp);
    for(char i=0; i<TEMP_BUFFER_SIZE; i++) {
      if(average_temp-TEMP_THRESHOLD<temps[i] && temps[i]<average_temp+TEMP_THRESHOLD) {
        // value is between the thresholds
      } else {
        temps[i] = -1;
      }
    }
    average_temp = 0;
    count_correction = 0;
    for(char i=0; i<TEMP_BUFFER_SIZE; i++) {
      if(temps[i]>0) {
        average_temp += temps[i]; 
      } else {
        count_correction++;
      }
    }
    average_temp /= (TEMP_BUFFER_SIZE-count_correction);
    debug("Average corrected=");
    debugln(average_temp);
    if(isnan(average_temp)) {
      debugln("Measurement failed :(");
      debug("Temperature stays=");
      debugln(last_average);
    } else {
      last_average = round(average_temp);
      debug("Average rounded=");
      debugln(last_average);
    }
  } 
}

void temp_display() {
  int d0 = last_average/10;
  int d1 = last_average%10;
  display_point(0);
  display_write(d0, d1, 18, 12);
}
