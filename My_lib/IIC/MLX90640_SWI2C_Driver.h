#ifndef __MLX90640_SWI2C_Driver_H
#define __MLX90640_SWI2C_Driver_H 

#include "MyType.h"

#define SCL_PORT   	 		GPIOB
#define SCL_Pin         GPIO_PIN_6
#define SCL_LOW         SCL_PORT->BSRR=(uint32_t)SCL_Pin<<16U
#define SCL_HIGH        SCL_PORT->BSRR=SCL_Pin 

#define SDA_PORT   	 		GPIOB
#define SDA_Pin         GPIO_PIN_7
#define SDA_LOW         SDA_PORT->BSRR=(uint32_t)SDA_Pin<<16U
#define SDA_HIGH        SDA_PORT->BSRR=SDA_Pin
#define sda             (SDA_PORT->IDR & SDA_Pin) 

#define IIC_SDA_OUT()     SDA_PORT->MODER |= 0X4000 //Output
#define IIC_SDA_IN()			SDA_PORT->MODER &=0XFFFF3FFF //Input

int I2CSendByte(int8_t);
void I2CReadBytes(int, char *);
void I2CStart(void);
void I2CStop(void);
void I2CRepeatedStart(void);
void I2CSendACK(void);
void I2CSendNack(void);
int I2CReceiveAck(void);
void Wait(int);
void IIC_GPIO_Init(void);
int MLX90640_I2CRead(uint8_t slaveAddr, uint16_t startAddress,uint16_t nMemAddressRead, uint16_t *data);
int MLX90640_I2CWrite(uint8_t slaveAddr, uint16_t writeAddress, uint16_t data);

#endif

