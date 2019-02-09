#ifndef DEBUG_H_
#define DEBUG_H_

void debug(const char* msg);
void debug(int msg);
void debug(float msg);
void debug(long msg);
void debug(signed char msg);

void debugln(const char* msg);
void debugln(int msg);
void debugln(float msg);
void debugln(long msg);
void debugln(signed char msg);

void debug_write_dots_while_handling();

#endif


