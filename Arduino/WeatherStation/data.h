#ifndef DATA_H_
#define DATA_H_

#define SERVER_NAME "api.openweathermap.org"
#define CITY_ID "721472"
#define NTP_SERVER_1 "pool.ntp.org"
#define NTP_SERVER_2 "time.nist.gov"
#define DEFAULT_TIME_ZONE 1 // Central European Time

void data_load_weather_data();
void data_load_time();

#endif
