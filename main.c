/* main.c - Nucleo F411RE final */

#include "main.h"
#include "keypad.h"
#include "fingerprint.h"
#include "actuator.h"
#include "storage.h"

UART_HandleTypeDef huart1;
TIM_HandleTypeDef htim3;

/* CubeMX will generate prototypes; define them here if replaced later */
void MX_GPIO_Init(void);
void MX_USART1_UART_Init(void);
void MX_TIM3_Init(void);

int main(void)
{
  HAL_Init();
  SystemClock_Config();

  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_TIM3_Init();

  fingerprint_init(&huart1);
  keypad_init();
  actuator_init();
  storage_init();

  while (1)
  {
    // 1) Fingerprint check
    int fp_id = fingerprint_search(); // non-blocking search, returns id or -1
    if (fp_id >= 0) {
      storage_log_event("FP_UNLOCK", fp_id);
      actuator_unlock();
      HAL_Delay(3000);
      actuator_lock();
      continue;
    }

    // 2) Keypad PIN entry
    char *pin = keypad_get_pin(); // returns pointer when '#' pressed
    if (pin) {
      if (storage_check_pin(pin)) {
        storage_log_event("PIN_UNLOCK", -1);
        actuator_unlock();
        HAL_Delay(3000);
        actuator_lock();
      }
    }

    HAL_Delay(50);
  }
}

/* Minimal peripheral init stubs (CubeMX will generate full code; include these only if needed) */
void MX_USART1_UART_Init(void)
{
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 57600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK) {
    Error_Handler();
  }
}

void MX_TIM3_Init(void)
{
  TIM_OC_InitTypeDef sConfigOC = {0};

  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 84-1; // 1 MHz timer (assuming 84 MHz system clock)
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 20000-1; // 20 ms => 50 Hz for servo
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK) Error_Handler();

  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 1500; // 1.5 ms center
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK) Error_Handler();
}

void MX_GPIO_Init(void)
{
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  // Actuator relay PA5
  GPIO_InitStruct.Pin = GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  // Keypad rows PB0-PB3 as input pull-up
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  // Keypad cols PB4-PB7 as outputs (set high)
  GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  // Set columns high
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_SET);
}

/* Minimal SystemClock stub - replace with CubeMX generated config */
void SystemClock_Config(void) {
  /* Use CubeMX generated code for full clock setup */
}

void Error_Handler(void)
{
  __disable_irq();
  while (1) {}
}
