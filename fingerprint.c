#include "fingerprint.h"
#include <string.h>

static UART_HandleTypeDef *fp_uart = NULL;
static uint8_t txbuf[64];
static uint8_t rxbuf[64];

static void fp_send_packet(uint8_t pid, uint8_t *data, uint16_t len)
{
  uint16_t idx = 0;
  txbuf[idx++] = 0xEF; txbuf[idx++] = 0x01; // header
  txbuf[idx++] = 0xFF; txbuf[idx++] = 0xFF; txbuf[idx++] = 0xFF; txbuf[idx++] = 0xFF; // addr
  txbuf[idx++] = pid;
  uint16_t plen = len + 2;
  txbuf[idx++] = (plen >> 8) & 0xFF;
  txbuf[idx++] = (plen) & 0xFF;
  uint16_t sum = pid + (plen >> 8) + (plen & 0xFF);
  for (uint16_t i = 0; i < len; ++i) {
    txbuf[idx++] = data[i];
    sum += data[i];
  }
  txbuf[idx++] = (sum >> 8) & 0xFF;
  txbuf[idx++] = sum & 0xFF;
  HAL_UART_Transmit(fp_uart, txbuf, idx, 300);
}

static int fp_receive(uint8_t *out, int maxlen, uint32_t timeout)
{
  memset(rxbuf,0,sizeof(rxbuf));
  if (HAL_UART_Receive(fp_uart, rxbuf, maxlen, timeout) > 0) {
    if (out) memcpy(out, rxbuf, maxlen);
    return 1;
  }
  return 0;
}

/* R305 basic commands */
static int fp_gen_image(void)
{
  uint8_t cmd[] = {0x01}; // GenImage
  fp_send_packet(0x01, cmd, 1);
  if (!fp_receive(NULL, 12, 300)) return -1;
  return rxbuf[9]; // confirmation code, 0x00 = OK
}

static int fp_img2tz(int buffer)
{
  uint8_t cmd[] = {0x02, (uint8_t)buffer}; // Img2Tz
  fp_send_packet(0x01, cmd, 2);
  if (!fp_receive(NULL, 12, 300)) return -1;
  return rxbuf[9];
}

static int fp_create_model(void)
{
  uint8_t cmd[] = {0x05}; // RegModel
  fp_send_packet(0x01, cmd, 1);
  if (!fp_receive(NULL, 12, 300)) return -1;
  return rxbuf[9];
}

static int fp_store_model(int id)
{
  uint8_t cmd[] = {0x06, 0x01, (uint8_t)((id>>8)&0xFF), (uint8_t)(id & 0xFF)};
  fp_send_packet(0x01, cmd, 4);
  if (!fp_receive(NULL, 12, 300)) return -1;
  return rxbuf[9];
}

static int fp_search(int *out_id)
{
  // Search from page 0x0000 to 0x03E8 (0..1000)
  uint8_t cmd[] = {0x04, 0x01, 0x00, 0x00, 0x03, 0xE8};
  fp_send_packet(0x01, cmd, 6);
  if (!fp_receive(NULL, 16, 400)) return -1;
  if (rxbuf[9] != 0x00) return -1;
  int id = (rxbuf[10] << 8) | rxbuf[11];
  if (out_id) *out_id = id;
  return 0;
}

static int fp_delete(int id)
{
  uint8_t cmd[] = {0x0C, 0x00, (uint8_t)((id>>8)&0xFF), (uint8_t)(id & 0xFF), 0x00, 0x01};
  fp_send_packet(0x01, cmd, 6);
  if (!fp_receive(NULL, 12, 300)) return -1;
  return rxbuf[9];
}

/* Public API */

void fingerprint_init(UART_HandleTypeDef *huart)
{
  fp_uart = huart;
}

int fingerprint_search(void)
{
  if (!fp_uart) return -1;
  // 1) Generate image
  if (fp_gen_image() != 0x00) return -1;
  // 2) Convert to template in buffer 1
  if (fp_img2tz(0x01) != 0x00) return -1;
  // 3) Search
  int id;
  if (fp_search(&id) == 0) return id;
  return -1;
}

int fingerprint_enroll(int id)
{
  if (!fp_uart) return -1;
  // Step 1: capture first
  int r;
  // Wait until a finger is placed
  r = fp_gen_image();
  if (r != 0x00) return -1;
  r = fp_img2tz(0x01);
  if (r != 0x00) return -1;

  // Ask user to remove finger then place again (host side delay)
  HAL_Delay(700);

  r = fp_gen_image();
  if (r != 0x00) return -1;
  r = fp_img2tz(0x02);
  if (r != 0x00) return -1;

  r = fp_create_model();
  if (r != 0x00) return -1;

  r = fp_store_model(id);
  if (r != 0x00) return -1;

  return 0;
}

int fingerprint_delete(int id)
{
  if (!fp_uart) return -1;
  return fp_delete(id);
}
