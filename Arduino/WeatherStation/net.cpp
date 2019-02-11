#include "net.h"
#include "config.h"
#include "debug.h"

#include <ESP8266WiFi.h>
#include <WiFiClient.h>

void net_setup_ap() {
  debugln("Creating AP...");
  WiFi.mode(WIFI_STA);
  if(WiFi.status() == WL_CONNECTED) {
    WiFi.disconnect();
  }
  delay(100);
  WiFi.softAP("WeatherStation", "w34th3rSt4t10n");
  debugln("WeatherStation AP created");
  debug("IP address: ");
  debugln(WiFi.softAPIP().toString().c_str());
}

void net_init() {
  if(config_get_has_wifi()) {
    WiFi.mode(WIFI_STA);
    debugln("WiFi Credentials available");
    char ssid[33];
    strcpy(ssid, config_get_ssid());
    char pass[65];
    strcpy(pass, config_get_password());
    debug("Connecting");
    WiFi.begin(ssid, pass);
    char t = 0;
    // Wait for connection
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      debug(">");
      t++;
      if(t==20) {
        debugln("Connection failed");
        net_setup_ap();
        break;
      }
    }
    if(WiFi.status() == WL_CONNECTED) {
      debug("Connected to ");
      debugln(ssid);
      debug("IP address: ");
      debugln(WiFi.localIP().toString().c_str());
    }
  } else {
    debugln("No WiFi Credentials available");
    net_setup_ap();
  }
}
