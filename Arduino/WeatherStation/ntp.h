#ifndef NTP_H_
#define NTP_H_

// NTP time is in the first 48 bytes of message
#define UDP_LOCAL_PORT 8888
#define NTP_PACKET_SIZE 48
#define NTP_SERVER "pool.ntp.org"
#define DEFAULT_TIME_ZONE 1 // Central European Time

void ntp_init();

#endif
