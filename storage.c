#include "storage.h"
#include "stm32f4xx_hal.h"
#include <string.h>

/* Simple demo store: keep one PIN in flash sector (example). 
   IMPORTANT: This is a minimal example. Flash addresses/sectors may vary per board.
   For production, implement wear-leveling and robust storage. */

#define FLASH_USER_START_ADDR   ((uint32_t)0x08060000) // example sector near end of flash
#define FLASH_USER_SIZE         ((uint32_t)0x1000)     // 4KB

static char stored_pin[8] = "1234"; // default

void storage_init(void)
{
  // In a full build you would read flash into stored_pin buffer.
  // For demo we'll keep default. Implement HAL flash read/erase/program if needed.
}

int storage_check_pin(const char *pin)
{
  if (!pin) return 0;
  return (strcmp(pin, stored_pin) == 0);
}

void storage_log_event(const char *event, int id)
{
  // Optional: append to SD card or send via debug UART.
  (void)event; (void)id;
}
