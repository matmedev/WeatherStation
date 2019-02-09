#include "server.h"
#include "debug.h"
#include "config.h"
#include <ESP8266WebServer.h>

extern ESP8266WebServer server;

void handleRoot() {
  debugln("\n\"/\" opened");
  char html[2000]; // When response html modified, check size
  char* t = html;
  t += sprintf(t, "<html>");
   t += sprintf(t, "<head>");
    t += sprintf(t, "<title>Weather Station config</title>");
    t += sprintf(t, "<style>");
     t += sprintf(t, "td { padding-left: 5px; padding-right: 5px; text-align: center; }");
     t += sprintf(t, "button { padding-top: 5px; padding-bottom: 5px; padding-left: 20px; padding-right: 20px; }");
    t += sprintf(t, "</style>");
   t += sprintf(t, "</head>");
   t += sprintf(t, "<body>");
    t += sprintf(t, "<table border=\"0\" style=\"border: 1px solid black; border-collapse: collapse; text-align:center\" align=\"center\">");
     t += sprintf(t, "<tr>");
      t += sprintf(t, "<td colspan=\"2\" style=\"padding-top: 15px; padding-bottom: 15px; border-bottom: 1px solid black;\">Weather Station config<br></td>");
     t += sprintf(t, "</tr>");
     t += sprintf(t, "<form action=\"/wifi-set\" method=\"POST\">");
      t += sprintf(t, "<tr>");
       t += sprintf(t, "<td style=\"text-align: right; padding-top: 8px; padding-bottom: 8px;\">SSID:</td>");
       t += sprintf(t, "<td style=\"text-align: left;\"><input type=\"text\" name=\"ssid\" value=\"%s\" style=\"border: 0px; border-bottom: 1px solid black; outline: 0px;\"></td>", config_get_has_wifi()?config_get_ssid():"kutya");
      t += sprintf(t, "</tr>");
      t += sprintf(t, "<tr>");
       t += sprintf(t, "<td style=\"text-align: right; padding-top: 8px; padding-bottom: 8px;\">Password:</td>");
       t += sprintf(t, "<td style=\"text-align: left;\"><input type=\"text\" name=\"password\" style=\"border: 0px; border-bottom: 1px solid black; outline: 0px;\"></td>");
      t += sprintf(t, "</tr>");
      t += sprintf(t, "<tr>");
       t += sprintf(t, "<td colspan=\"2\" style=\"text-align: center; padding-top: 8px; padding-bottom: 8px;\"><input type=\"submit\" name=\"wifi-set\" value=\"Set\" style=\"width:100%\"></td>");
      t += sprintf(t, "</tr>");
     t += sprintf(t, "</form>");
    t += sprintf(t, "</table>");
   t += sprintf(t, "</body>");
  t += sprintf(t, "</html>");
  server.send(200, "text/html", html);
}

void handleWifiSet() {
  debugln("\n\"/wifi-set\" opened");
  config_store_wifi(server.arg("ssid").c_str(), server.arg("password").c_str());
  server.send(200, "text/plain", "Wi-Fi credentials saved successfully");
  delay(100);
  ESP.restart();
}

void server_register_handlers() {
  server.on("/", handleRoot);
  server.on("/wifi-set", handleWifiSet);
}
