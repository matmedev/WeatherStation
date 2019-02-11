#include "data.h"

#include "credentials.h"
#include "clock.h"
#include "debug.h"
#include "leds.h"
#include "weather.h"

#include "Arduino.h"
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <time.h>

void printString(String str) {
  char arr[str.length()];
  str.toCharArray(arr, str.length());
  debugln(arr);
}

int mapToWeatherCode(String condition_str) {
  int condition = condition_str.toInt();
  switch(condition) {
    case 1:
      return 1;
    case 2:
      return 2;
    case 3:
    case 4:
    case 50:
      return 3;
    case 9:
    case 10:
      return 4;
    case 13:
      return 5;
    case 11:
      return 6;
    default:
      return 0;
  }
}

int getDSTCorrection(struct tm *currentTime) {
  byte currentMonth = currentTime->tm_mon + 1;
  if(3 <= currentMonth && currentMonth <= 10) {
    byte currentYear = currentTime->tm_year + 1900;
    if(3 == currentMonth) {
      int stpMarchDay = (31 - ((int) floor(currentYear*5.0/4) + 4) % 7);
      if(currentTime->tm_mday < stpMarchDay) { // in March, before the STP start day
        return DEFAULT_TIME_ZONE;
      } else if(stpMarchDay < currentTime->tm_mday) { // in March, after the STP start day
        return DEFAULT_TIME_ZONE + 1;
      } else { // in March, on the STP start day
        // STP starts at 1:00 am, before that no adjustment is needed
        if(currentTime->tm_hour < 1) {
            return 0;
        } else {
            return 1;
        }
      }
    } else if(10 == currentMonth) {
      int stpOctoberDay = (31 - ((int) floor(currentYear*5.0/4) + 1) % 7);
      if(currentTime->tm_mday < stpOctoberDay) { // in October, before the STP end day
        return DEFAULT_TIME_ZONE + 1;
      } else if(stpOctoberDay < currentTime->tm_mday) { // in October, after the STP end day
        return DEFAULT_TIME_ZONE;
      } else { // in October, on the STP end day
        // STP ends at 1:00 am, after that no adjustment is needed
        if(currentTime->tm_hour < 1) {
          return 1;
        } else {
          return 0;
        }
      }
    } else {
      return 1;
    }
  } else { 
    return 0;
  }
}

void data_load_weather_data() {
  String result = "";
  WiFiClient client;

  if(!client.connect(SERVER_NAME, 80)) {
    // Connection failed
    return;
  }

  String url = "/data/2.5/forecast?id=" + String(CITY_ID) + "&units=metric&cnt=1&APPID=" + String(WEATHER_API_KEY);
  debug("Connecting to URL: ");
  printString(url);

  client.print(String("GET ") + url + "HTTP/1.1\r\n" +
               "Host: " + SERVER_NAME + "\r\n" +
               "Connection: close\r\n" +
               "\r\n");
  unsigned long timeout = millis();
  while(client.available() == 0) {
    if(millis() - timeout > 5000) {
      client.stop();
      return;
    }
  }

  // Read all the lines of the reply
  while(client.available()) {
    result = client.readStringUntil('\r');
  }

  result.replace('[', ' ');
  result.replace(']', ' ');

  char jsonArray[result.length() + 1];
  result.toCharArray(jsonArray, sizeof(jsonArray));
  jsonArray[result.length() + 1] = '\0';

  StaticJsonBuffer<1024> json_buf;
  JsonObject &root = json_buf.parseObject(jsonArray);
  if(!root.success()) {
    debugln("parseObject() failed");
  }
  
  String temperature = root["list"]["main"]["temp"];
  String condition = root["list"]["weather"]["icon"];
  
  weather_set_temp(temperature.toFloat());
  leds_change_state(2, (char) mapToWeatherCode(condition));
}

void data_load_time() {
  time_t current_time = 0;
  
  configTime(DEFAULT_TIME_ZONE * 3600, 0, NTP_SERVER_1, NTP_SERVER_2);
  while(!current_time) {
    time(&current_time);
    delay(250);
  }

  time_t now = time(&current_time);
  struct tm *rtime = localtime(&now);
  
  clock_set_time(
    rtime->tm_hour + getDSTCorrection(rtime),
    rtime->tm_min,
    rtime->tm_sec);
}
