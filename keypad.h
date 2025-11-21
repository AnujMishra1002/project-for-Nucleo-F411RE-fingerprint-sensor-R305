#ifndef KEYPAD_H
#define KEYPAD_H

#include "main.h"

void keypad_init(void);
char keypad_get_key(void);
char* keypad_get_pin(void); // returns pointer to static buffer when '#' pressed

#endif
