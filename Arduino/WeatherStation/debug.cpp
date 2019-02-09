#include "debug.h"
#include "Arduino.h"

void debug(const char* msg) {
  Serial.print(msg);
}

void debug(int msg) {
  Serial.print(msg);
}

void debug(float msg) {
  Serial.print(msg);
}

void debug(long msg) {
  Serial.println(msg);
}

void debug(signed char msg) {
  Serial.print(msg);
}

void debugln(const char* msg) {
  Serial.println(msg);
}

void debugln(int msg) {
  Serial.println(msg);
}

void debugln(float msg) {
  Serial.println(msg);
}

void debugln(long msg) {
  Serial.println(msg);
}

void debugln(signed char msg) {
  Serial.println(msg);
}

long debug_last_dot = 0;
char debug_dot_count = 0;

void debug_write_dots_while_handling() {
  if(millis()>debug_last_dot+100) {
    Serial.print(".");
    debug_last_dot=millis();
    debug_dot_count++;
  }
  if(debug_dot_count==80) {
    Serial.println();
    debug_dot_count=0;
  }
}
