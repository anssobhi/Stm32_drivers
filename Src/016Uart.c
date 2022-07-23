/*
 * 016Uart.c
 *
 *  Created on: ١٠‏/٠٣‏/٢٠٢٢
 *      Author: ans
 */


#include <stdio.h>
#include <string.h>
#include "stm32f407xx.h"

extern void initialise_monitor_handles(void);

char *msg[3] = {"hihihihi123" , "hello how are you " ,"today"};

char rx_buff[1024];

USART_Handle_t USART;

uint8_t rxCmplt = RESET;

uint8_t g_data = 0;

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
	USART.USART_Config.USART_Mode = USART_MODE_TXRX;
	USART.USART_Config.USART_NoOfStopBits = USART_STOPBITS_1;
	USART.USART_Config.USART_ParityControl = USART_PARITY_DISABLE;
	USART.USART_Config.USART_WordLength = USART_WORDLEN_8BITS;

	USART_Init(&USART);
}

int main(void)
{
	uint32_t cnt =0;

	initialise_monitor_handles();

	Button_Inits();

	USART2_GPIOInit();

	USART2_Init();

	USART_IRQInterruptConfig(IRQ_NO_USART2, ENABLE);

	USART_PeripheralControl(USART2,ENABLE);

	printf("Application is running \n");

	while(1)
	{
	while(!GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0));

	delay();

	cnt = cnt % 3;

	while(USART_ReceiveDataIT(&USART, rx_buff, strlen(msg[cnt])) != USART_READY);

	USART_SendData(&USART, (uint8_t*)msg[cnt], strlen(msg[cnt]));

	printf("Transmitted : %s\n",msg[cnt]);

	while(rxCmplt != SET);

	//just make sure that last byte should be null otherwise %s fails while printing
	rx_buff[strlen(msg[cnt])+ 1] = '\0';

	//Print what we received from the arduino
	printf("Received    : %s\n",rx_buff);

	//invalidate the flag
	rxCmplt = RESET;

	//move on to next message indexed in msg[]
	cnt ++;

	}
	return 0;
}


void USART2_IRQHandler(void)
{

	USART_IRQHandling(&USART);
}

void USART_ApplicationEventCallback(USART_Handle_t *pUSARTHandle,uint8_t AppEv)
{
	if(AppEv == USART_EVENT_RX_CMPLT)
	{
		rxCmplt = SET;
	}else if (AppEv == USART_EVENT_TX_CMPLT)
	{
		;
	}
}
