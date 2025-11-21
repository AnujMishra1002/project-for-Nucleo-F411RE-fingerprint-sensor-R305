#include "keypad.h"
#include <string.h>

#define ROW_PORT GPIOB
static const uint16_t ROW_PINS[4] = {GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_2, GPIO_PIN_3};
#define COL_PORT GPIOB
static const uint16_t COL_PINS[4] = {GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_6, GPIO_PIN_7};

static char keymap[4][4] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

static char pin_buffer[8];

void keypad_init(void) {
  for (int c = 0; c < 4; c++) HAL_GPIO_WritePin(COL_PORT, COL_PINS[c], GPIO_PIN_SET);
  memset(pin_buffer, 0, sizeof(pin_buffer));
}

static char map_key(int r, int c) { return keymap[r][c]; }

char keypad_get_key(void) {
  for (int c = 0; c < 4; c++) {
    HAL_GPIO_WritePin(COL_PORT, COL_PINS[c], GPIO_PIN_RESET);
    HAL_Delay(1);
    for (int r = 0; r < 4; r++) {
      if (HAL_GPIO_ReadPin(ROW_PORT, ROW_PINS[r]) == GPIO_PIN_RESET) {
        HAL_Delay(20); // debounce
        if (HAL_GPIO_ReadPin(ROW_PORT, ROW_PINS[r]) == GPIO_PIN_RESET) {
          char k = map_key(r,c);
          while (HAL_GPIO_ReadPin(ROW_PORT, ROW_PINS[r]) == GPIO_PIN_RESET) HAL_Delay(10);
          HAL_GPIO_WritePin(COL_PORT, COL_PINS[c], GPIO_PIN_SET);
          return k;
        }
      }
    }
    HAL_GPIO_WritePin(COL_PORT, COL_PINS[c], GPIO_PIN_SET);
  }
  return '\0';
}

char* keypad_get_pin(void) {
  static int idx = 0;
  char k = keypad_get_key();
  if (k == '\0') return NULL;

  if (k == '#') {
    if (idx > 0) {
      pin_buffer[idx] = '\0';
      idx = 0;
      return pin_buffer;
    }
    return NULL;
  }

  if (k == '*') {
    idx = 0;
    pin_buffer[0] = '\0';
    return NULL;
  }

  if (idx < (int)(sizeof(pin_buffer)-1)) {
    pin_buffer[idx++] = k;
  }
  return NULL;
}
