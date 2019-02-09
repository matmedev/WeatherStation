#ifndef LEDS_H_
#define LEDS_H_

#define LED_9 0
#define SR_REG 15   //74HC595 pin 12
#define SR_CLOCK 2  //74HC595 pin 11
#define SR_DATA 13  //74HC595 pin 14

/*
 * 74HC595 setup:
 * * pin 10(SRCLR) to POS
 * * pin 13(OE)    to GND 
 */

// Based on: https://gist.github.com/42Bots/54711c9536ef69afad7f3b9aacebb942

/*
 *  Row #1: 
 *   - #1 - P0 - Clock
 *   - #2 - S1 - Inner Temp
 *   - #3 - S2 - Outer Temp
 *  Row #2:
 *   - #1 - S3 - Sunny
 *   - #2 - S4 - Partly sunny
 *   - #3 - S5 - Cloudy
 *   - #4 - S6 - Rain
 *   - #5 - S7 - Snow
 *   - #6 - S8 - Thunder
 */

void leds_init();
void leds_change_state(char row, char coloumn);

#endif
