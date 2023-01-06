#include "soft_iic.h"


void Soft_IIC_W_SCL(GPIO_PinState BitValue)//设置SCL引脚电平
{
	HAL_GPIO_WritePin(SOFT_IIC_PORT, SOFT_IIC_SCL_PIN, BitValue);
}


void Soft_IIC_W_SDA(GPIO_PinState BitValue)//设置SDA引脚电平
{
	HAL_GPIO_WritePin(SOFT_IIC_PORT, SOFT_IIC_SDA_PIN, BitValue);
}


GPIO_PinState Soft_IIC_R_SDA()//读取SDA引脚电平
{
	return HAL_GPIO_ReadPin(SOFT_IIC_PORT, SOFT_IIC_SDA_PIN);
}


void Soft_IIC_Start()//软件IIC起始条件
{
	Soft_IIC_W_SDA(GPIO_PIN_SET);
	Soft_IIC_W_SCL(GPIO_PIN_SET);
	Soft_IIC_W_SDA(GPIO_PIN_RESET);
	Soft_IIC_W_SCL(GPIO_PIN_RESET);
}


void Soft_IIC_Stop()//软件IIC终止条件
{
	Soft_IIC_W_SDA(GPIO_PIN_RESET);
	Soft_IIC_W_SCL(GPIO_PIN_SET);
	Soft_IIC_W_SDA(GPIO_PIN_SET);
}


void Soft_IIC_SendByte(uint8_t Byte)//软件IIC发送一个字节数据
{
	uint8_t i;
	for(i = 0; i < 8; i ++)
	{
		if((Byte & (0x80 >> i)) == (0x80 >> i)) Soft_IIC_W_SDA(GPIO_PIN_SET);
		else Soft_IIC_W_SDA(GPIO_PIN_RESET);
		Soft_IIC_W_SCL(GPIO_PIN_SET);
		Soft_IIC_W_SCL(GPIO_PIN_RESET);
	}
}


uint8_t Soft_IIC_ReceiveByte()//软件IIC接收一个字节数据
{
	uint8_t i, Byte = 0x00;
	Soft_IIC_W_SDA(GPIO_PIN_SET);
	for(i = 0; i < 8; i ++)
	{
		Soft_IIC_W_SCL(GPIO_PIN_SET);
		if(Soft_IIC_R_SDA() == GPIO_PIN_SET) Byte |= (0x80 >> i);
		Soft_IIC_W_SCL(GPIO_PIN_RESET);
	}
	return Byte;
}


void Soft_IIC_SendACK(uint8_t AckBit)//软件IIC发送应答
{
	if(AckBit == 1) Soft_IIC_W_SDA(GPIO_PIN_SET);
	else Soft_IIC_W_SDA(GPIO_PIN_RESET);
	Soft_IIC_W_SCL(GPIO_PIN_SET);
	Soft_IIC_W_SCL(GPIO_PIN_RESET);
}


uint8_t Soft_IIC_ReceiveACK()//软件IIC接收应答
{
	uint8_t AckBit;
	Soft_IIC_W_SDA(GPIO_PIN_SET);
	Soft_IIC_W_SCL(GPIO_PIN_SET);
	if(Soft_IIC_R_SDA() == GPIO_PIN_SET) AckBit = 1;
	else AckBit = 0;
	Soft_IIC_W_SCL(GPIO_PIN_RESET);
	return AckBit;
}


void Soft_IIC_Init()//软件IIC初始化
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(SOFT_IIC_PORT, SOFT_IIC_SDA_PIN|SOFT_IIC_SCL_PIN, GPIO_PIN_SET);

  /*Configure GPIO pins : PA5 PA6 */
  GPIO_InitStruct.Pin = SOFT_IIC_SDA_PIN|SOFT_IIC_SCL_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(SOFT_IIC_PORT, &GPIO_InitStruct);
}
