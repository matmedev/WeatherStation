#include "ntp.h"
#include "debug.h"

#include <TimeLib.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#include "Arduino.h"

byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming & outgoing packets
WiFiUDP Udp;

// send an NTP request to the time server at the given address
void ntp_send_packet(IPAddress &address) {
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12] = 49;
  packetBuffer[13] = 0x4E;
  packetBuffer[14] = 49;
  packetBuffer[15] = 52;
  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  Udp.beginPacket(address, 123); //NTP requests are to port 123
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
}

time_t ntp_get_time() {
  IPAddress ntpServerIp;

  while(Udp.parsePacket() > 0);
  debugln("Transmit NTP request");
  // get a random server from the pool
  WiFi.hostByName(NTP_SERVER, ntpServerIp);
  debug(NTP_SERVER);
  debug(": ");
  Serial.println(ntpServerIp);
  ntp_send_packet(ntpServerIp);
  uint32_t beginWait = millis();
  while(millis() - beginWait < 1500) {
    int size = Udp.parsePacket();
    if(size >= NTP_PACKET_SIZE) {
      debugln("NTP response received");
      Udp.read(packetBuffer, NTP_PACKET_SIZE);
      unsigned long secsSince1900;
      // convert four bytes starting at location 40 to a long integer
      secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
      secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
      secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
      secsSince1900 |= (unsigned long)packetBuffer[43];
      return secsSince1900 - 2208988800UL + DEFAULT_TIME_ZONE * SECS_PER_HOUR;
    }
  }
  debugln("No NTP response :(");
  return 0;
}

void ntp_init() {
  debugln("Starting UDP");
  Udp.begin(UDP_LOCAL_PORT);
  debugln("Waiting for sync...");
  setSyncProvider(ntp_get_time);
  setSyncInterval(28800); // 8 hours in seconds
}
