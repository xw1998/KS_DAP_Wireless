/*
 * @Descripttion: 
 * @version: 
 * @Author: Kevincoooool
 * @Date: 2020-08-04 20:32:30
 * @LastEditors  : Kevincoooool
 * @LastEditTime : 2020-11-12 19:26:09
 * @FilePath     : \Simple_TeenyUSB_TX\BSP\bsp_spi.c
 */
#include "bsp_spi.h"
#include "spi.h"
#include "gpio.h"
#include "dma.h"
void DAP_SPI_Init(void)
{
	MX_GPIO_Init();
	MX_DMA_Init();
	MX_SPI1_Init();
	MX_SPI2_Init();
}

uint8_t SPI_RW_1(uint8_t dat)
{
	// uint8_t d_read, d_send = dat;
	// if (HAL_SPI_TransmitReceive(&hspi1, &d_send, &d_read, 1, 10) != HAL_OK)
	// {
	// 	d_read = 0xFF;
	// }
	// return d_read;
	uint16_t retry = 0;
	while ((SPI1->SR & 1 << 1) == 0) //等待发送区空
	{
		retry++;
		if (retry >= 0XFFFE)
			return 0; //超时退出
	}
	SPI1->DR = dat; //发送一个byte
	retry = 0;
	while ((SPI1->SR & 1 << 0) == 0) //等待接收完一个byte
	{
		retry++;
		if (retry >= 0XFFFE)
			return 0; //超时退出
	}
	return SPI1->DR; //返回收到的数据
}

uint8_t SPI_RW_2(uint8_t dat)
{
	uint8_t d_read, d_send = dat;
	if (HAL_SPI_TransmitReceive(&hspi2, &d_send, &d_read, 1, 0xFF) != HAL_OK)
	{
		d_read = 0xFF;
	}
	return d_read;
	//	uint8_t d_read, d_send = dat;
	//	if (HAL_SPI_TransmitReceive_DMA(&hspi2, &d_send, &d_read, 1) != HAL_OK)
	//	{
	//		d_read = 0xFF;
	//	}
	//	return d_read;
}
static void DeInit_SPI(void)
{
	
	SPI1->CR1 |= 0 << 6;
	//这里只针对SPI口初始化
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	GPIO_InitStruct.Pin = GPIO_PIN_7;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	GPIOA->BSRR = GPIO_PIN_5;
	GPIOA->BSRR = GPIO_PIN_7;
}

static void SPI_ON(void)
{

	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = GPIO_PIN_5;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_7;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = GPIO_PIN_6;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	SPI1->CR1 |= 1 << 6; //EN SPI
}
void SPI_Switch(uint8_t ONOFF)
{
	if (ONOFF == 1)
		SPI_ON();
	else
		DeInit_SPI();
}
uint8_t SPI_TXRX(uint8_t *TXBuff, uint8_t *RXBuff, uint8_t cnt)
{
	uint8_t cnt_buff;

	for (cnt_buff = 0; cnt_buff < cnt; cnt_buff++)
	{
		RXBuff[cnt_buff] = SPI_RW_1(TXBuff[cnt_buff]);
	}
	return 1;
}

uint8_t SPI_TX(uint8_t *TXBuff, uint8_t cnt)
{
	uint8_t cnt_buff;
	for (cnt_buff = 0; cnt_buff < cnt; cnt_buff++)
	{
		SPI_RW_1(TXBuff[cnt_buff]);
	}
	return 1;
}

uint8_t SPI_TX7(uint8_t *TXBuff)
{
	SPI_RW_1(TXBuff[0]);
	SPI_RW_1(TXBuff[1]);
	SPI_RW_1(TXBuff[2]);
	SPI_RW_1(TXBuff[3]);
	SPI_RW_1(TXBuff[4]);
	SPI_RW_1(TXBuff[5]);
	SPI_RW_1(TXBuff[6]);
	return 1;
}

uint8_t SPI_TX4(uint8_t *TXBuff)
{
	SPI_RW_1(TXBuff[0]);
	SPI_RW_1(TXBuff[1]);
	SPI_RW_1(TXBuff[2]);
	SPI_RW_1(TXBuff[3]);
	return 1;
}

uint8_t SPI_TX2(uint8_t *TXBuff)
{
	SPI_RW_1(TXBuff[0]);
	SPI_RW_1(TXBuff[1]);
	return 1;
}
