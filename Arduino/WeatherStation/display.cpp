#include "display.h"
#include "TM1637.h";

extern TM1637 tm1637;

void display_point(char stat) {
  tm1637.point(stat==0?POINT_OFF:POINT_ON);
}

void display_write(int d0, int d1, int d2, int d3) {
  tm1637.display(0, d0);
  tm1637.display(1, d1);
  tm1637.display(2, d2);
  tm1637.display(3, d3);
}

void display_set_brightness(char brightness) {
  tm1637.set(brightness);
}
