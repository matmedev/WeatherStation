#include "Arduino.h"
#include <math.h>

#include "leds.h"

void leds_init() {
  pinMode(LED_9, OUTPUT);
  pinMode(SR_REG, OUTPUT);
  pinMode(SR_CLOCK, OUTPUT);
  pinMode(SR_DATA, OUTPUT);
}

char leds_shifted_value[8] = {0, 0, 0, 0, 0, 0, 0, 0};

void leds_shift_register() {
  char val_to_shift = 0;
  for(char i=0; i<8; i++) {
    if(leds_shifted_value[i]) {
      char new_val = 1;
      new_val = new_val << i;
      val_to_shift = val_to_shift | new_val;
    }
  }
  digitalWrite(SR_REG, LOW);
  shiftOut(SR_DATA, SR_CLOCK, MSBFIRST, val_to_shift);
  digitalWrite(SR_REG, HIGH);
}

void leds_change_state(char row, char coloumn) {
  if(row==1) {
    switch(coloumn) {
      case 1:
        digitalWrite(LED_9, HIGH);
        leds_shifted_value[0] = 0;
        leds_shifted_value[1] = 0;
        break;
      case 2:
        digitalWrite(LED_9, LOW);
        leds_shifted_value[0] = 1;
        leds_shifted_value[1] = 0;
        break;
      case 3:
        digitalWrite(LED_9, LOW);
        leds_shifted_value[0] = 0;
        leds_shifted_value[1] = 1;
        break;
    }
  } else if(row==2) {
    for(char i=2; i<8; i++) {
      leds_shifted_value[i] = 0;
    }
    if(coloumn!=0) {
      leds_shifted_value[coloumn+1] = 1;
    }
  }
  leds_shift_register();
}
