#include "weather.h"
#include "mqtt.h"
#include "display.h"

#include "Arduino.h"

#define WEATHER_REFRESH_DELAY 1200000 //20 minutes

int weather_temp_c = 9999;

long weather_last_refreshed = 0;

void weather_set_temp(float t) {
  if(t>0) {
    t += 0.5;
  } else if(t<0) {
    t -= 0.5;
  }
  weather_temp_c = (int) t;
}

void weather_load() {
  long now = millis();
  if(now>weather_last_refreshed+WEATHER_REFRESH_DELAY) {
    mqtt_request_weather();
    weather_last_refreshed = now;
  }
}

void weather_display() {
  int d0;
  int d1;
  int d2;
  int d3;
  if(weather_temp_c<0) {
    if(weather_temp_c<-10) {
      d0 = 16; // 16 is for '-'
      d1 = (-1*weather_temp_c)/10;
      d2 = (-1*weather_temp_c)%10;
      d3 = 18; //18 is for '°'
    } else {
      d0 = 16; // 16 is for '-'
      d1 = (-1*weather_temp_c);
      d2 = 18; //18 is for '°'
      d3 = 12; //12 is for 'C'
    }
  } else if(weather_temp_c == 9999) {
    d0 = 16; // 16 is for '-'
    d1 = 16; // 16 is for '-'
    d2 = 16; // 16 is for '-'
    d3 = 16; // 16 is for '-'
  } else {
    d0 = weather_temp_c/10;
    d1 = weather_temp_c%10;
    d2 = 18; //18 is for '°'
    d3 = 12; //12 is for 'C'
  }
  display_point(0);
  display_write(d0, d1, d2, d3);
}
