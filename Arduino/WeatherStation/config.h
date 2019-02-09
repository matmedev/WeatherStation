#ifndef CONFIG_H_
#define CONFIG_H_

void config_init();
void config_reset();
int config_get_has_wifi();
void config_load_wifi();
void config_store_wifi(const char* _ssid, const char* _password);
char* config_get_ssid();
char* config_get_password();

#endif
