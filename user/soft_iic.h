/************************************************************
* 软件IIC引脚配置
* GPIO:    见soft_iic.h宏定义 SOFT_IIC_PORT
* SCL引脚: 见soft_iic.h宏定义 SOFT_IIC_SCL_PIN
* SDA引脚: 见soft_iic.h宏定义 SOFT_IIC_SDA_PIN
*************************************************************/
#ifndef __SOFT_IIC_H__
#define __SOFT_IIC_H__

#include "stm32f1xx_hal.h"	//链接HAL库

#define SOFT_IIC_PORT                   GPIOA
#define SOFT_IIC_SCL_PIN                GPIO_PIN_6
#define SOFT_IIC_SDA_PIN                GPIO_PIN_5


void Soft_IIC_W_SCL(GPIO_PinState BitValue);//设置SCL引脚电平

void Soft_IIC_W_SDA(GPIO_PinState BitValue);//设置SDA引脚电平
	
GPIO_PinState Soft_IIC_R_SDA(void);//读取SDA引脚电平

void Soft_IIC_Start(void);//软件IIC起始条件

void Soft_IIC_Stop(void);//软件IIC终止条件

void Soft_IIC_SendByte(uint8_t Byte);//软件IIC发送一个字节数据

uint8_t Soft_IIC_ReceiveByte(void);//软件IIC接收一个字节数据

void Soft_IIC_SendACK(uint8_t AckBit);//软件IIC发送应答

uint8_t Soft_IIC_ReceiveACK(void);//软件IIC接收应答

void Soft_IIC_Init(void);   //软件IIC初始化


#endif /*__SOFT_IIC_H__*/
