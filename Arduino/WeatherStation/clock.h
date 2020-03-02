#ifndef CLOCK_H_
#define CLOCK_H_

#define NIGHT_START 20
#define DAY_START 5

void clock_set_time(int h, int m, int s);
void clock_increment();
void clock_display();

#endif
