#ifndef ACTUATOR_H
#define ACTUATOR_H

#include "main.h"

void actuator_init(void);
void actuator_unlock(void);
void actuator_lock(void);
void actuator_set_servo(int micros);

#endif
