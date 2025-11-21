/* main.h */
#ifndef __MAIN_H
#define __MAIN_H

#include "stm32f4xx_hal.h"

extern UART_HandleTypeDef huart1; // fingerprint on USART1 (PA9/PA10)
extern TIM_HandleTypeDef htim3;   // actuator PWM if used

void SystemClock_Config(void);
void Error_Handler(void);

#endif // __MAIN_H
