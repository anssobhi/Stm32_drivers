/*
 * 007spi_txonly_arduino.c
 *
 *  Created on: ٠٦‏/٠٢‏/٢٠٢٢
 *      Author: ans
 */


/*
 * 006spi_tx_testing.c
 *
 *  Created on: ٣١‏/٠١‏/٢٠٢٢
 *      Author: ans
 */

//PB14 -> MISO
//PB15 -> MOSI
//PB12 -> NSS
//PB13 -> SCLK
// ALT MODE IS 5


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "stm32f407xx.h"

//extern void initialise_monitor_handles();

#define COMMAND_LED_CTRL         0x50
#define COMMAND_SENSOR_READ      0x51
#define COMMAND_LED_READ         0x52
#define COMMAND_PRINT            0x53
#define COMMAND_ID_READ          0x54

#define LED_ON                   1
#define LED_OFF                  0

#define ANALOG_PIN0              0
#define ANALOG_PIN1              1
#define ANALOG_PIN2              2
#define ANALOG_PIN3              3
#define ANALOG_PIN4              4

#define LED_PIN                  9

void delay(void)
{
	for(uint32_t i =0;i<200000;i++);
}
void SPI2_GPIOIntis(void)
{
	GPIO_Handle_t SPIPins;
	SPIPins.pGPIOx = GPIOB;
	SPIPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	SPIPins.GPIO_PinConfig.GPIO_PinAltFunMode = 0x05;
	SPIPins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	SPIPins.GPIO_PinConfig.GPIO_PinPupdControl = GPIO_NO_PUPD;
	SPIPins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

	// configure the SCLK pin
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;
	GPIO_Inti(&SPIPins);

	// configure the MOSI pin
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_15;
	GPIO_Inti(&SPIPins);

	// configure the MISO pin
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_14;
	GPIO_Inti(&SPIPins);

	// configure the NSS pin
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
	GPIO_Inti(&SPIPins);
}
void SPI2_Intis(void)
{
	SPI_Handle_t SPI2handle;
	SPI2handle.pSPIx = SPI2;
	SPI2handle.SPIConfig.SPI_BusConfig = SPI_BUS_CONFIG_FD;
	SPI2handle.SPIConfig.SPI_DeviceMode = SPI_DEVICE_MODE_MASTER;
	SPI2handle.SPIConfig.SPI_SclkSpeed = SPI_SCLK_SPEED_DIV8;
	SPI2handle.SPIConfig.SPI_DFF  = SPI_DFF_8BITS;
	SPI2handle.SPIConfig.SPI_CPOL = SPI_CPOL_LOW;
	SPI2handle.SPIConfig.SPI_CPHA = SPI_CPHA_LOW;
	SPI2handle.SPIConfig.SPI_SSM  = SPI_SSM_DI;// hardware
	//SPI2handle.pSPIx->CR1 |= (1<<6);
	SPI_Inti(&SPI2handle);
}
void GPIO_ButtonInit(void)
{
	GPIO_Handle_t GPIO_Button;
	GPIO_Button.pGPIOx = GPIOA;
	GPIO_Button.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_0;
	GPIO_Button.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_INPUT;
	GPIO_Button.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GPIO_Button.GPIO_PinConfig.GPIO_PinPupdControl = GPIO_NO_PUPD;
	//GPIO_PerClockControl(GPIOA, ENABLE);
	GPIO_Inti(&GPIO_Button);
}
uint8_t SPI_VerifyResponse(uint8_t ackbyte)
{
	if(ackbyte == 0xF5)
	{
		return 1;
	}else
	{
		return 0;
	}
}
int main ()
{
	uint8_t dummy_byte = 0xff;
	uint8_t dummy_read;

	//initialise_monitor_handles();

	GPIO_ButtonInit();

	SPI2_GPIOIntis();

	SPI2_Intis();


	SPI_SSOEConfig(SPI2, ENABLE);

	while(1)
	{
	//wait till button is pressed
	while(!GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0));
	delay();
	// enable the spi2 peripheral
	SPI_PeripheralControl(SPI2,ENABLE);

	// 1. CMD_LED_CTRL  <pin no(1)>    <value(1)>
	uint8_t commandcode  = COMMAND_LED_CTRL;
	uint8_t ackbyte;
	uint8_t args[2];

	// send command
	SPI_SendData(SPI2,&commandcode, 1);
	// do dummy read to clear off the RXNE
	SPI_ReceiveData(SPI2, &dummy_read, 1);
	// send dummy data to fetch the response from the slave
	SPI_SendData(SPI2, &dummy_byte, 1);
	// read the ACK byte received
	SPI_ReceiveData(SPI2, &ackbyte, 1);
	//printf("the ack is %d \n",ackbyte);
	if ( SPI_VerifyResponse(ackbyte))
	{
		args[0] = LED_PIN;
		args[1] = LED_ON;
		SPI_SendData(SPI2, args, 2);
	}
	while(!GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0));
	delay();
	   commandcode = COMMAND_SENSOR_READ;
	   // send command
	    SPI_SendData(SPI2,&commandcode, 1);
		// do dummy read to clear off the RXNE
		SPI_ReceiveData(SPI2, &dummy_read, 1);
		// send dummy data to fetch the response from the slave
		SPI_SendData(SPI2, &dummy_byte, 1);
		// read the ack byte received
		SPI_ReceiveData(SPI2, &ackbyte, 1);

		if ( SPI_VerifyResponse(ackbyte))
		{
			args[0] = ANALOG_PIN0;
			SPI_SendData(SPI2, args, 1);

			SPI_ReceiveData(SPI2, &dummy_read, 1);

			delay();

			SPI_SendData(SPI2, &dummy_byte, 1);

 			uint8_t analog_read;

			SPI_ReceiveData(SPI2,&analog_read, 1);
			//printf("the value is %d\n",analog_read);
		}


 	while(SPI_GetFlagStatus(SPI2, SPI_BUSY_FLAG));

	SPI_PeripheralControl(SPI2, DISABLE);

	}

	return 0;
}
