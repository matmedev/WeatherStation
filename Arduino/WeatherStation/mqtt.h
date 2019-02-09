#ifndef MQTT_H_
#define MQTT_H_

#define MQTT_TEMP_LOG_DELAY 1200000 //20 minutes

void mqtt_init();
void mqtt_handle();
void mqtt_request_weather();

#endif
