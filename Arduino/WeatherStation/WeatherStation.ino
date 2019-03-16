#include "config.h"
#include "server.h"
#include "data.h"
#include "debug.h"
#include "net.h"
#include "ntp.h"
#include "temp.h"
#include "clock.h"
#include "weather.h"
#include "timer.h"
#include "button.h"
#include "leds.h"

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include "TM1637.h";
#include <DHT.h>

ESP8266WebServer server(80);
TM1637 tm1637(16, 14); //CLK, DIO
DHT dht(12, DHT11); //pin, type

void load_data() {
  temp_load();
  weather_load();
  clock_increment();
}

void setup() {
  Serial.begin(115200);
  config_init();
  net_init();
  server_register_handlers();
  server.begin();
  Serial.println("Server started");

  ntp_init();

  // Removed while testing:
  tm1637.init();
  tm1637.set(7); // Set brightness to 7
  dht.begin();
  button_init();
  leds_init();

  data_load_weather_data();
  data_load_time();
}

void loop() {
  server.handleClient();
  debug_write_dots_while_handling();
  timer_display();
  button_handle_press();
  load_data();
  delay(1);
}
