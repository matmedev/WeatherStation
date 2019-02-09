#include "config.h"
#include <EEPROM.h>
#include <stdio.h>

 /*
  * EEPROM contents:
  * addr=0 -> has wifi
  * addr=1-32 -> ssid
  * addr=33-96 -> password
  */

char config_has_wifi;
char config_ssid[33];
char config_password[65];

void config_init() {
  memset(config_ssid, '\0', 33);
  memset(config_password, '\0', 65);
  EEPROM.begin(512);
  
  char conf_0 = EEPROM.read(0);
  if(conf_0==0 || conf_0==255) {
    config_has_wifi = 0;
  } else {
    config_has_wifi = 1;
    config_load_wifi();
  }
}

void config_reset() {
  EEPROM.write(0, '\0');
  for (int i=1; i<=96; i++) {
    EEPROM.write(i, '\0');
  }
  EEPROM.commit();
}

int config_get_has_wifi() {
  return config_has_wifi;
}

void config_load_wifi() {
  int start = 1;
  for (int i=0;i<32;i++) {
    config_ssid[i] = EEPROM.read(start+i);
  }
  config_ssid[32] = '\0';
  for (int i=0;i<64;i++) {
    config_password[i] = EEPROM.read(start+32+i);
  }
  config_password[64] = '\0';
}

void config_store_wifi(const char* _ssid, const char* _password) {
  int start = 1;
  for (int i=0;i<strlen(_ssid);i++) {
    EEPROM.write(start+i, _ssid[i]);
  }
  for (int i=strlen(_ssid);i<32;i++) {
    EEPROM.write(start+i, '\0');
  }
  for (int i=0;i<strlen(_password);i++) {
    EEPROM.write(start+32+i, _password[i]);
  }
  for (int i=strlen(_password);i<64;i++) {
    EEPROM.write(start+32+i, '\0');
  }
  EEPROM.write(0, 1);
  EEPROM.commit();
}

char* config_get_ssid() {
  return config_ssid;
}

char* config_get_password() {
  return config_password;
}
