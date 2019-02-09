#include "button.h"
#include "timer.h"

#include "Arduino.h"

char button_button_pressed = 0;

void button_init() {
  pinMode(BUTTON_PIN, INPUT);
}

void button_handle_press() {
  if(digitalRead(BUTTON_PIN)) {
    if(button_button_pressed!=0) {
      timer_jump_next();
      button_button_pressed = 0;
    }
  } else {
    button_button_pressed = 1;
  }
}

