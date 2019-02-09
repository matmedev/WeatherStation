#include "clock.h"
#include "display.h"
#include "debug.h"

#include "Arduino.h"

int clock_hour = 0;
int clock_minute = 0;
int clock_second = 0;
char clock_isPointOn = 1;
long clock_last_read = 0;
long clock_last_changed = 0;

void clock_set_hour(int h) {
  clock_hour = h;
}

void clock_set_minute(int m) {
  clock_minute = m;  
}

void clock_set_second(int s) {
  clock_second = s;
}

void clock_increment() {
  long now = millis();
  if(now>clock_last_read+500) {
    if(clock_isPointOn) {
      clock_isPointOn = 0;
    } else {
      clock_isPointOn = 1;
    }
    display_point(clock_isPointOn);
    clock_last_read = now;
  }
  if(now>=clock_last_changed+1000) {
    while(now-clock_last_changed>=1000) {
      clock_second++;
      if(clock_second==60) {
        clock_minute++;
        clock_second -= 60;
      }
      if(clock_minute==60) {
        clock_hour++;
        clock_minute=0;
        clock_second=0;
      }
      if(clock_hour==24) {
        clock_hour=0;
        clock_minute=0;
        clock_second=0;
      }      
      clock_last_changed += 1000;
    }
  }
}

void clock_display() {
  int d0 = clock_hour/10;
  int d1 = clock_hour%10;
  int d2 = clock_minute/10;
  int d3 = clock_minute%10;
  display_write(d0, d1, d2, d3);
}

