#include "stm32f4xx_hal.h"
#include "OV7670.h"


DCMI_HandleTypeDef hdcmi;
DMA_HandleTypeDef hdma_dcmi;
I2C_HandleTypeDef hi2c1;
uint8_t SCCBTxBuffer[5];
uint8_t SCCBRxBuffer[5];


void MX_DCMI_Init(void);
void MX_I2C1_Init(void);
uint8_t ov7670_init(void);
uint8_t ov7670_read(uint8_t reg);
void ov7670_write(uint8_t reg,uint8_t val);
void ov7670_capturesnapshot(uint8_t *buff);
void ov7670_capture(uint8_t *buff);

/* DCMI init function */
void MX_DCMI_Init(void)
{

  hdcmi.Instance = DCMI;
  hdcmi.Init.SynchroMode = DCMI_SYNCHRO_HARDWARE;
  hdcmi.Init.PCKPolarity = DCMI_PCKPOLARITY_RISING;
  hdcmi.Init.VSPolarity = DCMI_VSPOLARITY_HIGH;
  hdcmi.Init.HSPolarity = DCMI_HSPOLARITY_LOW;
  hdcmi.Init.CaptureRate = DCMI_CR_ALL_FRAME;
  hdcmi.Init.ExtendedDataMode = DCMI_EXTEND_DATA_8B;
  hdcmi.Init.JPEGMode = DCMI_JPEG_DISABLE;
  HAL_DCMI_Init(&hdcmi);

}

/* I2C1 init function */
void MX_I2C1_Init(void)
{

  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLED;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLED;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_ENABLED;
  HAL_I2C_Init(&hi2c1);

}

uint8_t ov7670_init(void){
	MX_DCMI_Init();
	MX_I2C1_Init();
	//Camera setup code
	HAL_DCMI_MspInit(&hdcmi);
  //I2C Setup code
	HAL_I2C_MspInit(&hi2c1);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, GPIO_PIN_RESET);
	HAL_Delay(100);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, GPIO_PIN_SET);
	if((ov7670_read(0x0A)==0x76)){
		////////////////////////////////////Working code
	//ov7670_write(0x11,0x01);//20FPS
ov7670_write(0x3A,0x04);//TSLB register
//ov7670_write(0x12,0x14);QVGA resolution
ov7670_write(0x12,0x0C);//QCIF Resolution
ov7670_write(0x8C,0x00);//RGB444 register
ov7670_write(0x04,0x00);//COM1 register
ov7670_write(0x40,0xD0);//COM15 register(RGB565)
ov7670_write(0x72,0x11);
ov7670_write(0x73,0xF1);
		////////////////////////////////////
/*
ov7670_write(0x3A,0x04);//TSLB
ov7670_write(0x3B,0x0A);//COM11 50Hz autodetection
ov7670_write(0x12,0x04);//COM7 RGB output
ov7670_write(0x8C,0x00);//RGB444 disable
ov7670_write(0x40,0xD0);//COM15  RGB565 output
ov7670_write(0x0C,0x0C);//COM3
ov7670_write(0x3E,0x11);//COM14
ov7670_write(0x70,0x3A);//Scaling XSC
ov7670_write(0x71,0x35);//Scaling YSC
ov7670_write(0x72,0x11);//Scaling DCWCTR
ov7670_write(0x73,0xF1);//Scaling PCLK_DIV
ov7670_write(0xA2,0x52);//Scaling PCLK_DELAY
*/
/*
ov7670_write(0x32,0x80);
ov7670_write(0x17,0x3A);
ov7670_write(0x18,0x03);
ov7670_write(0x19,0x02);
ov7670_write(0x1A,0x7B);
ov7670_write(0x03,0x06);
ov7670_write(0x13,0xE0);
ov7670_write(0x00,0x00);
ov7670_write(0x14,0x48);
ov7670_write(0x13,0xE5);
ov7670_write(0xA4,0x89);
*/
////////// Color settings
ov7670_write(0x4f,0x80);
ov7670_write(0x50,0x80);
ov7670_write(0x51,0x00);
ov7670_write(0x52,0x22);
ov7670_write(0x53,0x5e);
ov7670_write(0x54,0x80);
ov7670_write(0x56,0x40);
ov7670_write(0x58,0x9e);
ov7670_write(0x59,0x88);
ov7670_write(0x5a,0x88);
ov7670_write(0x5b,0x44);
ov7670_write(0x5c,0x67);
ov7670_write(0x5d,0x49);
ov7670_write(0x5e,0x0e);
ov7670_write(0x69,0x00);
ov7670_write(0x6a,0x40);
ov7670_write(0x6b,0x0a);
ov7670_write(0x6c,0x0a);
ov7670_write(0x6d,0x55);
ov7670_write(0x6e,0x11);
ov7670_write(0x6f,0x9f);
ov7670_write(0xb0,0x84);

return 1;
	}
else{
	return 0;
}
}

uint8_t ov7670_read(uint8_t reg){
	SCCBTxBuffer[0]=reg;
	while(HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)0x42, (uint8_t*)SCCBTxBuffer, 1, 10000)!= HAL_OK){};
	while(HAL_I2C_Master_Receive(&hi2c1, (uint16_t)0x42, (uint8_t *)SCCBRxBuffer, 1, 10000) != HAL_OK){};
	return SCCBRxBuffer[0];
	}

void ov7670_write(uint8_t reg,uint8_t val){
	SCCBTxBuffer[0]=reg;
	SCCBTxBuffer[1]=val;
	while(HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)0x42, (uint8_t*)SCCBTxBuffer, 2, 10000)!= HAL_OK){};
	}

void ov7670_capturesnapshot(uint8_t *buff){
HAL_DCMI_Start_DMA(&hdcmi, DCMI_MODE_SNAPSHOT, (uint32_t)buff,11088);  
}
void ov7670_capture(uint8_t *buff){
HAL_DCMI_Start_DMA(&hdcmi, DCMI_MODE_CONTINUOUS,(uint32_t)buff,11088);  
}
