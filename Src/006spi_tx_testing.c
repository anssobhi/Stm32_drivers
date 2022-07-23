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
	// this is the CONFIG of SCLK
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;
	GPIO_Inti(&SPIPins);
	// this is the CONFIG of MISO
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_15;
	GPIO_Inti(&SPIPins);

}
void SPI2_Intis(void)
{
	SPI_Handle_t SPI2handle;
	SPI2handle.pSPIx = SPI2;
	SPI2handle.SPIConfig.SPI_BusConfig = SPI_BUS_CONFIG_FD;
	SPI2handle.SPIConfig.SPI_DeviceMode = SPI_DEVICE_MODE_MASTER;
	SPI2handle.SPIConfig.SPI_SclkSpeed = SPI_SCLK_SPEED_DIV2;
	SPI2handle.SPIConfig.SPI_DFF  = SPI_DFF_8BITS;
	SPI2handle.SPIConfig.SPI_CPOL = SPI_CPOL_LOW;
	SPI2handle.SPIConfig.SPI_CPHA = SPI_CPHA_LOW;
	SPI2handle.SPIConfig.SPI_SSM  = SPI_SSM_EN;
	SPI2handle.pSPIx->CR1 |= (1<<6);
	SPI_Inti(&SPI2handle);
}

int main ()
{
	char User_data [] = "Hello World";

	SPI2_GPIOIntis();

	SPI2_Intis();

	SPI_SSIConfig(SPI2,ENABLE);

	SPI_PeripheralControl(SPI2,ENABLE);


	SPI_SendData(SPI2, (uint8_t*)User_data,strlen(User_data));

	SPI_PeripheralControl(SPI2, DISABLE);

	while(1);

	return 0;
}
