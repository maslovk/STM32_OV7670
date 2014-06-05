#include "stm32f4xx_hal.h"

void MX_DCMI_Init(void);
void MX_I2C1_Init(void);
uint8_t ov7670_init(void);
uint8_t ov7670_read(uint8_t reg);
void ov7670_write(uint8_t reg,uint8_t val);
void ov7670_capturesnapshot(uint8_t *buff);
void ov7670_capture(uint8_t *buff);
