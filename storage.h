#ifndef STORAGE_H
#define STORAGE_H

#include "main.h"

void storage_init(void);
int storage_check_pin(const char *pin);
void storage_log_event(const char *event, int id);

#endif
