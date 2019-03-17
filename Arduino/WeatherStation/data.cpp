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
#include <TimeLib.h>

void printString(String str) {
  char arr[str.length()];
  str.toCharArray(arr, str.length());
  debugln(arr);
}

int mapToWeatherCode(int condition_code) {
  switch(condition_code) {
    case 1000: // Sunny
      // Sunny
      return 1;    
    case 1003: // Partly cloudy
      // Partly
      return 2;
    case 1006: // Cloudy
    case 1009: // Overcast
    case 1030: // Mist
    case 1063: // Patchy rain possible
    case 1066: // Patchy snow possible
    case 1069: // Patchy sleet possible
    case 1072: // Patchy freezing drizzle possible
    case 1087: // Thundery outbreaks possible
    case 1135: // Fog
    case 1147: // Freezing fog
      // Cloudy
      return 3;
    case 1150: // Patchy light drizzle
    case 1153: // Light drizzle
    case 1168: // Freezing drizzle
    case 1171: // Heavy freezing drizzle
    case 1180: // Patchy light rain
    case 1183: // Light rain
    case 1186: // Moderate rain at times
    case 1189: // Moderate rain
    case 1192: // Heavy rain at times
    case 1195: // Heavy rain
    case 1198: // Light freezing rain
    case 1201: // Moderate or heavy freezing rain
    case 1240: // Light rain shower
    case 1243: // Moderate or heavy rain shower
    case 1246: // Torrential rain shower
      // Rain
      return 4;
    case 1114: // Blowing snow
    case 1117: // Blizzard
    case 1204: // Light sleet
    case 1207: // Moderate or heavy sleet
    case 1210: // Patchy light snow
    case 1213: // Light snow
    case 1216: // Patchy moderate snow
    case 1219: // Moderate snow
    case 1222: // Patchy heavy snow
    case 1225: // Heavy snow
    case 1237: // Ice pellets
    case 1249: // Light sleet showers
    case 1252: // Moderate or heavy sleet showers
    case 1255: // Light snow showers
    case 1258: // Moderate or heavy snow showers
    case 1261: // Light showers of ice pellets
    case 1264: // Moderate or heavy showers of ice pellets
      // Snow
      return 5;
    case 1273: // Patchy light rain with thunder
    case 1276: // Moderate or heavy rain with thunder
    case 1279: // Patchy light snow with thunder
    case 1282: //  Moderate or heavy snow with thunder
      // Thunder
      return 6;
    default:
      // Unknown error
      return 0;
  }
}

int getDSTCorrection() {
//  debugln("getDSTCorrection");
//  debug("current month=");
//  debugln(month());
  if(3 <= month() && month() <= 10) {
//    debugln("between march and october");
//    debug("current year=");
//    debugln(year());
    if(3 == month()) {
//      debugln("in march");
      int stpMarchDay = (31 - ((int) floor(year()*5.0/4) + 4) % 7);
//      debug("stpMarchDay");
//      debugln(stpMarchDay);
      if(day() < stpMarchDay) { // in March, before the STP start day
        return 0;
      } else if(stpMarchDay < day()) { // in March, after the STP start day
        return 1;
      } else { // in March, on the STP start day
        // STP starts at 1:00 am, before that no adjustment is needed
        if(hour() < 1) {
            return 0;
        } else {
            return 1;
        }
      }
    } else if(10 == month()) {
      int stpOctoberDay = (31 - ((int) floor(year()*5.0/4) + 1) % 7);
      if(day() < stpOctoberDay) { // in October, before the STP end day
        return 1;
      } else if(stpOctoberDay < day()) { // in October, after the STP end day
        return 0;
      } else { // in October, on the STP end day
        // STP ends at 1:00 am, after that no adjustment is needed
        if(hour() < 1) {
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

  String url = String("/v1/current.json?key=") + WEATHER_API_KEY + "&q=" + CITY_ID;
  debug("Connecting to URL: ");
  Serial.println(url);
  
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
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
 
  StaticJsonBuffer<1158> json_buf;
  JsonObject &root = json_buf.parseObject(jsonArray);
  if(!root.success()) {
    debugln("parseObject() failed");
  }
  
  float temperature = root["current"]["temp_c"];
  int condition = root["current"]["condition"]["code"];
  
  weather_set_temp(temperature);
  leds_change_state(2, (char) mapToWeatherCode(condition));
}

void data_load_time() {
//  debug(hour());
//  debug(":");
//  debug(minute());
//  debug(":");
//  debug(second());
//  debug(" (");
//  debug(getDSTCorrection());
//  debugln(")");
  
  clock_set_time(
    hour() + getDSTCorrection(),
    minute(),
    second());
}
