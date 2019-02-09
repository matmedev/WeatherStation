#include "mqtt.h"
#include "clock.h"
#include "weather.h"
#include "debug.h"
#include "temp.h"
#include "leds.h"

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "Arduino.h"

const char* mqtt_server = "192.168.1.50";
int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient mqtt_client(espClient);

const char* timeTopic = "matme/service/time";
const char* timeRequestTopic = "matme/service/time/req";
const char* conditionsTopic = "matme/service/condition/Debrecen";
const char* conditionsRequestTopic = "matme/service/condition/Debrecen/req";
const char* weatherOutTopic = "matme/service/weather/Debrecen";
const char* weatherOutRequestTopic = "matme/service/weather/Debrecen/req";
const char* weatherInTopic = "matme/log/weather/kitchen";

long mqtt_last_logged_temp = 0;

void mqtt_callback(char* topic, byte* payload, unsigned int length) {
  if(strcmp(timeTopic,topic)==0) {
    int hour = (payload[0]-'0')*10 + (payload[1]-'0');
    int minute = (payload[3]-'0')*10 + (payload[4]-'0');
    int second = (payload[6]-'0')*10 + (payload[7]-'0');
    clock_set_hour(hour);
    clock_set_minute(minute);
    clock_set_second(second);
  } else if (strcmp(weatherOutTopic, topic)==0) {
    if(strcmp((const char*) payload, "na")==0) {
      weather_set_temp(9999);
    } else {
      float tempc = atof((char*) payload);
      weather_set_temp(tempc);
    }
  } else if (strcmp(conditionsTopic, topic)==0) {
    int cond = atoi((char*) payload);
    leds_change_state(2, (char) cond);
  }
}

void mqtt_reconnect() {
  if (mqtt_client.connect("WeatherStation")) {
    debug("Connected to MQTT server - subscribing to ");
    debugln(timeTopic);
    mqtt_client.subscribe(timeTopic);
    mqtt_client.publish(timeRequestTopic, "1");
    mqtt_client.subscribe(weatherOutTopic);
    mqtt_client.publish(weatherOutRequestTopic, "1");
    mqtt_client.subscribe(conditionsTopic);
    mqtt_client.publish(conditionsRequestTopic, "1");
  } else {
    debug("Failed to connect to MQTT server: ");
    debugln(mqtt_client.state());
  }
}

void mqtt_init() {
  debug("Initializing MQTT server as ");
  debug(mqtt_server);
  debug(":");
  debugln(mqtt_port);
  mqtt_client.setServer(mqtt_server, mqtt_port);
  mqtt_client.setCallback(mqtt_callback);
}

void mqtt_handle() {
  if (!mqtt_client.connected()) {
    mqtt_reconnect();
  }
  if(mqtt_client.connected()) {
    mqtt_client.loop();
  }
  long now = millis();
  int t = temp_get_temp();
  if(now>mqtt_last_logged_temp+MQTT_TEMP_LOG_DELAY || (mqtt_last_logged_temp==0 && t!=0)) {
    char msg[3] = {'\0', '\0', '\0'};
    msg[0] = t/10 + '0';
    msg[1] = t%10 + '0';
    mqtt_client.publish(weatherInTopic, msg);
    mqtt_last_logged_temp = now;
  }
}

void mqtt_request_weather() {
  mqtt_client.publish(weatherOutRequestTopic, "1");
  mqtt_client.publish(conditionsRequestTopic, "1");
}

