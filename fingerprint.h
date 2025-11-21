#ifndef FINGERPRINT_H
#define FINGERPRINT_H

#include "main.h"

void fingerprint_init(UART_HandleTypeDef *huart);
int fingerprint_search(void);      // returns template ID >= 0, or -1
int fingerprint_enroll(int id);    // enroll flow: returns 0 success, non-zero fail
int fingerprint_delete(int id);    // delete template id

#endif
