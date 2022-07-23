/*
 * 020i2c_master_rx.c
 *
 *  Created on: ٢٨‏/٠٢‏/٢٠٢٢
 *      Author: ans
 */

#include <stdio.h>
#include <string.h>
#include "stm32f407xx.h"


#define SLAVE_ADDR 0x69

I2C_Handle_t I2C1Handle;
uint8_t TX_BUFF[32] = "STM32 Slave mode testing";

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
	I2C1Handle.I2C_Config.I2C_DeviceAddress = SLAVE_ADDR;
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
	I2C1_GPIOInits();

	I2C_Inits();

	I2C_IRQInterruptConfig(IRQ_NO_I2C1_EV, ENABLE);
	I2C_IRQInterruptConfig(IRQ_NO_I2C1_ER, ENABLE);

	I2C_SlaveEnableDisableCallbackEvents(I2C1,ENABLE);

	Button_Inits();

	I2C_PeripheralControl(I2C1, ENABLE);

	I2C_MangeAck(I2C1, ENABLE);

	while(1);


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
	static uint8_t commandCode = 0;
	static uint8_t cnt = 0;
	if(AppEv == I2C_EV_DATA_REQ)
	{
		if(commandCode == 0x51)
		{
			I2C_SlaveSendData(pI2CHandle->pI2Cx, strlen((char*)TX_BUFF));

		}else if(commandCode == 0x52)
		{
			I2C_SlaveSendData(pI2CHandle->pI2Cx,TX_BUFF[cnt++]);
		}


	}else if(AppEv == I2C_EV_DATA_RCV)
	{
		commandCode = I2C_SlaveReceiveData(pI2CHandle->pI2Cx);


	}else if(AppEv == I2C_ERROR_AF)
	{

		commandCode = 0xff;
		cnt = 0;

	}else if(AppEv == I2C_EV_STOP)
	{



	}
}
