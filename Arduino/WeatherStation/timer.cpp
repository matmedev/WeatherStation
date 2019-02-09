#include "timer.h"
#include "clock.h"
#include "temp.h"
#include "weather.h"
#include "leds.h"

#include "Arduino.h"

#define TIMER_MODE_TIME 0
#define TIMER_MODE_TEMP 1
#define TIMER_MODE_WEATHER 2

int timer_mode = 2;

long timer_last_switched = 0;

void timer_switch_mode() {
  if(timer_mode==2) {
    timer_mode=0;
  } else {
    timer_mode++;
  }
}

void timer_display() {
  long now = millis();
  if(now>timer_last_switched+TIMER_MODE_DELAY) {
    timer_switch_mode();
    timer_last_switched = now;
  }
  switch(timer_mode) {
    case TIMER_MODE_TIME:
      clock_display();
      leds_change_state(1, 1);
      break;
    case TIMER_MODE_TEMP:
      temp_display();
      leds_change_state(1, 2);
      break;
    case TIMER_MODE_WEATHER:
      weather_display();
      leds_change_state(1, 3);
      break;
  }
}

void timer_jump_next() {
  timer_switch_mode();
  timer_last_switched = millis();
}

