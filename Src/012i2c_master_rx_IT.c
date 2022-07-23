/*
 * 020i2c_master_rx.c
 *
 *  Created on: ٢٨‏/٠٢‏/٢٠٢٢
 *      Author: ans
 */

#include <stdio.h>
#include <string.h>
#include "stm32f407xx.h"

uint8_t rxComplt = RESET;

#define SLAVE_ADDR 0x65

extern void initialise_monitor_handles(void);

I2C_Handle_t I2C1Handle;
uint8_t rcv[32];

void delay(void)
{
	for(uint32_t i=0;i<500000/2;i++);
}

void I2C1_GPIOInits(void)
{
	GPIO_Handle_t I2CPins;

	I2CPins.pGPIOx = GPIOB;
	I2CPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	I2CPins.GPIO_PinConfig.GPIO_PinAltFunMode = 4;
	I2CPins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_OD;
	I2CPins.GPIO_PinConfig.GPIO_PinPupdControl = GPIO_PIN_PU;
	I2CPins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

	// SCL
	I2CPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_6;
	GPIO_Inti(&I2CPins);

	// SDA
	I2CPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_9;
	GPIO_Inti(&I2CPins);
}

void I2C_Inits(void)
{
	I2C1Handle.pI2Cx = I2C1;
	I2C1Handle.I2C_Config.I2C_ACKControl = I2C_ACK_ENABLE;
	I2C1Handle.I2C_Config.I2C_DeviceAddress = 0x61;
	I2C1Handle.I2C_Config.I2C_FMDutyCycle = I2C_FM_DUTY_2;
	I2C1Handle.I2C_Config.I2C_SCLSpeed = I2C_SCL_SPEED_SM;

	I2C_Inti(&I2C1Handle);
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

int main(void)
{


	uint8_t command_code;
	uint8_t len;

	initialise_monitor_handles();

	I2C1_GPIOInits();

	I2C_Inits();

	I2C_IRQInterruptConfig(IRQ_NO_I2C1_EV, ENABLE);
	I2C_IRQInterruptConfig(IRQ_NO_I2C1_ER, ENABLE);

	Button_Inits();

	I2C_PeripheralControl(I2C1, ENABLE);

	I2C_MangeAck(I2C1, ENABLE);

	while(1)
	{
		while(!GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0));
		delay();
		//printf("hello \n");
		command_code = 0x51;
		while(I2C_MasterSendDataIT(&I2C1Handle,&command_code, 1, SLAVE_ADDR,I2C_ENABLE_SR)!= I2C_READY);
		while(I2C_MasterReceiveDataIT(&I2C1Handle, &len, 1, SLAVE_ADDR,I2C_ENABLE_SR) != I2C_READY);
		command_code = 0x52;
		while(I2C_MasterSendDataIT(&I2C1Handle,&command_code, 1, SLAVE_ADDR,I2C_ENABLE_SR)!= I2C_READY);
		while(I2C_MasterReceiveDataIT(&I2C1Handle, rcv, len , SLAVE_ADDR,I2C_DIABLE_SR) != I2C_READY);
		rxComplt = RESET;
		while(rxComplt != SET);
		rcv[len+1] = '\0';
		printf("data : %s \n",rcv);
		rxComplt = RESET;
	}


	return 0;
}
void I2C1_EV_IRQHandler(void)
{
	I2C_EV_IRQHandling(&I2C1Handle);
}
void I2C1_ER_IRQHandler(void)
{
	I2C_ER_IRQHandling(&I2C1Handle);
}


void I2C_ApllicationEventCallback(I2C_Handle_t *pI2CHandle,uint8_t AppEv)
{
	if(AppEv == I2C_EV_TX_CMPLT)
	{
		printf("Tx is complete \n");

	}else if(AppEv == I2C_EV_RX_CMPLT)
	{
		printf("Rx is complete \n");
		rxComplt = SET;

	}else if(AppEv == I2C_ERROR_AF)
	{
		printf("Error ACK Failure \n");
		I2C_CloseSendData(pI2CHandle);
		I2C_GenerateStopCondition(I2C1);
		while(1);
	}
}
