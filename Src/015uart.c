/*
 * 015uart.c
 *
 *  Created on: ١٠‏/٠٣‏/٢٠٢٢
 *      Author: ans
 */
#include <stdio.h>
#include <string.h>
#include "stm32f407xx.h"

char msg[1024] = "UART TX testing \n\r";

USART_Handle_t USART;

void delay(void)
{
	for(uint32_t i=0;i<500000/2;i++);
}

void Button_Inits(void)
{
	GPIO_Handle_t Button;

	Button.pGPIOx = GPIOA;

	Button.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_INPUT;
	Button.GPIO_PinConfig.GPIO_PinPupdControl = GPIO_NO_PUPD;
	Button.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_0;
	Button.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

	GPIO_Inti(&Button);

}

void USART2_GPIOInit(void)
{
	GPIO_Handle_t usart_gpios;
	usart_gpios.pGPIOx = GPIOA;
	usart_gpios.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	usart_gpios.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	usart_gpios.GPIO_PinConfig.GPIO_PinPupdControl = GPIO_PIN_PU;
	usart_gpios.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	usart_gpios.GPIO_PinConfig.GPIO_PinAltFunMode = 7;

	usart_gpios.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_2;
	GPIO_Inti(&usart_gpios);

	usart_gpios.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_3;
	GPIO_Inti(&usart_gpios);
}

void USART2_Init(void)
{
	USART.pUSARTx = USART2;
	USART.USART_Config.USART_Baud = USART_STD_BAUD_115200;
	USART.USART_Config.USART_HWFlowControl = USART_HW_FLOW_CTRL_NONE;
	USART.USART_Config.USART_Mode = USART_MODE_ONLY_TX;
	USART.USART_Config.USART_NoOfStopBits = USART_STOPBITS_1;
	USART.USART_Config.USART_ParityControl = USART_PARITY_DISABLE;
	USART.USART_Config.USART_WordLength = USART_WORDLEN_8BITS;

	USART_Init(&USART);
}

int main (void )
{

	Button_Inits();

	USART2_GPIOInit();

	USART2_Init();

	USART_PeripheralControl(USART2,ENABLE);

	while(1)
	{
	while(!GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0));

	delay();

	USART_SendData(&USART, (uint8_t*)msg, strlen(msg));

	}

	return 0;
}
