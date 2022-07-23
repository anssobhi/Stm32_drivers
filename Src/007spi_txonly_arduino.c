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

#include "stm32f407xx.h"
#include "stm32f407xx_gpio.h"
#include "stm32f407xx_spi.h"
#include <string.h>
void SPI2_GPIOIntis(void)
{
	GPIO_Handle_t SPIPins;
	SPIPins.pGPIOx = GPIOB;
	SPIPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	SPIPins.GPIO_PinConfig.GPIO_PinAltFunMode = 0x05;
	SPIPins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	SPIPins.GPIO_PinConfig.GPIO_PinPupdControl = GPIO_NO_PUPD;
	SPIPins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;
	GPIO_Inti(&SPIPins);

	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_15;
	GPIO_Inti(&SPIPins);

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
int main ()
{
 	char User_data [] = "Hello World";

	GPIO_ButtonInit();

	SPI2_GPIOIntis();

	SPI2_Intis();

	//SPI_SSIConfig(SPI2,ENABLE);

	SPI_SSOEConfig(SPI2, ENABLE);

	while(1)
	{

	while(!GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0));

	SPI_PeripheralControl(SPI2,ENABLE);

	uint8_t data_len = strlen(User_data);

	SPI_SendData(SPI2, &data_len,1);

	SPI_SendData(SPI2, (uint8_t*)User_data,strlen(User_data));

	while(SPI_GetFlagStatus(SPI2, SPI_BUSY_FLAG));

	SPI_PeripheralControl(SPI2, DISABLE);

	}

	return 0;
}
