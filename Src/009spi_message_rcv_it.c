/*
 * 009spi_message_rcv_it.c
 *
 *  Created on: ١٣‏/٠٢‏/٢٠٢٢
 *      Author: ans
 */

#include <stdio.h>
#include <string.h>
#include "stm32f407xx.h"

SPI_Handle_t SPI2_Handle;

#define MAX_LEN 500
char RcvBuff[MAX_LEN];
volatile uint8_t rcvStop = 0;
volatile uint8_t dataAvailable = 0;
volatile char ReadByte  ;


void GPIO_ConfigIt(void)
{
	GPIO_Handle_t GPIO_It;
	GPIO_It.pGPIOx = GPIOD;
	GPIO_It.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_6;
	GPIO_It.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IT_FT;
	GPIO_It.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_LOW;
	GPIO_It.GPIO_PinConfig.GPIO_PinPupdControl = GPIO_PIN_PU;
	GPIO_Inti(&GPIO_It);
	GPIO_IRQInterruptConfig(IRQ_NO_EXTI9_5, ENABLE);
	GPIO_IRQPriorityConfig(IRQ_NO_EXTI9_5, 30);
}
void SPI2_GPIOInti (void)
{
	GPIO_Handle_t SPIPins;
	SPIPins.pGPIOx = GPIOB;
	SPIPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	SPIPins.GPIO_PinConfig.GPIO_PinAltFunMode = 5;
	SPIPins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	SPIPins.GPIO_PinConfig.GPIO_PinPupdControl = GPIO_NO_PUPD;
	SPIPins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;
	GPIO_Inti(&SPIPins);
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_15;
	GPIO_Inti(&SPIPins);
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_14;
	GPIO_Inti(&SPIPins);
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
	GPIO_Inti(&SPIPins);

}

void SPI2_Inti(void)
{

	SPI2_Handle.pSPIx = SPI2;
	SPI2_Handle.SPIConfig.SPI_DeviceMode = SPI_DEVICE_MODE_MASTER;
	SPI2_Handle.SPIConfig.SPI_SclkSpeed = SPI_SCLK_SPEED_DIV32;
	SPI2_Handle.SPIConfig.SPI_BusConfig = SPI_BUS_CONFIG_FD;
	SPI2_Handle.SPIConfig.SPI_DFF = SPI_DFF_8BITS;
	SPI2_Handle.SPIConfig.SPI_CPHA = SPI_CPHA_LOW;
	SPI2_Handle.SPIConfig.SPI_CPOL = SPI_CPOL_LOW;
	SPI2_Handle.SPIConfig.SPI_SSM = SPI_SSM_DI;
	SPI_Inti(&SPI2_Handle);
}




int main()
{
	uint8_t dummy = 0xff;



	GPIO_ConfigIt();

	SPI2_GPIOInti();

	SPI2_Inti();

	SPI_SSOEConfig(SPI2, ENABLE);

	SPI_IRQInterruptConfig(IRQ_NO_SPI2, ENABLE);

	while(1)
	{
		rcvStop = 0;
		while(!dataAvailable);

		GPIO_IRQInterruptConfig(IRQ_NO_EXTI9_5, DISABLE);

		SPI_PeripheralControl(SPI2, ENABLE);

		while(!rcvStop)
		{
			while(SPI_SendDataIT(&SPI2_Handle, &dummy, 1)== SPI_BUSY_IN_TX);
			while(SPI_ReceiveDataIT(&SPI2_Handle, &ReadByte, 1)== SPI_BUSY_IN_RX);

		}
		while(SPI_GetFlagStatus(SPI2, SPI_BUSY_FLAG));

		SPI_PeripheralControl(SPI2, DISABLE);

		printf("RCVd data = %s\n",RcvBuff);

		dataAvailable =0;

		GPIO_IRQInterruptConfig(IRQ_NO_EXTI9_5, ENABLE);
	}

	return 0;
}

void SPI2_IRQHandler(void)
{
	SPI_IRQHandling(&SPI2_Handle);
}

void SPI_ApplicationEventCallback(SPI_Handle_t *pSPIHandle,uint8_t AppEv)
{
	static uint32_t i = 0;
	if(AppEv == SPI_EVENT_RX_CMPLT)
	{
		RcvBuff[i++] = ReadByte;
		if(ReadByte == '\0' || (i == MAX_LEN))
		{
			rcvStop = 1;
			RcvBuff[i-1] = '\0';
			i=0;
		}
	}
}
void EXTI9_5_IRQHandler(void)
{
	GPIO_IRQHandling(GPIO_PIN_NO_6);
	dataAvailable = 1;
}

