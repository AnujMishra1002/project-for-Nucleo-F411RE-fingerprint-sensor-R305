#include "actuator.h"

extern TIM_HandleTypeDef htim3; // from main.c

#define RELAY_PORT GPIOA
#define RELAY_PIN  GPIO_PIN_5

void actuator_init(void) {
  HAL_GPIO_WritePin(RELAY_PORT, RELAY_PIN, GPIO_PIN_RESET);
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
}

void actuator_unlock(void) {
  HAL_GPIO_WritePin(RELAY_PORT, RELAY_PIN, GPIO_PIN_SET);
}

void actuator_lock(void) {
  HAL_GPIO_WritePin(RELAY_PORT, RELAY_PIN, GPIO_PIN_RESET);
}

void actuator_set_servo(int micros) {
  if (micros < 500) micros = 500;
  if (micros > 2500) micros = 2500;
  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, micros);
}
