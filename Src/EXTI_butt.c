/*
 * EXTI_butt.c
 *
 *  Created on: ٢٤‏/٠١‏/٢٠٢٢
 *      Author: ans
 */

#include "stm32f407xx_gpio.h"
#include "stm32f407xx.h"
#include <string.h>
int main(void)
{

	    GPIO_Handle_t GPIO_Button;
	    memset(&GPIO_Button,0,sizeof(GPIO_Button));
		GPIO_Button.pGPIOx = GPIOA;
		GPIO_Button.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_0;
		GPIO_Button.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IT_FT;
		GPIO_Button.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
		GPIO_Button.GPIO_PinConfig.GPIO_PinPupdControl = GPIO_NO_PUPD;
		GPIO_Handle_t GPIO_led;
		GPIO_led.pGPIOx = GPIOD;
		GPIO_led.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
		GPIO_led.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUTPUT;
		GPIO_led.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
		GPIO_led.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
		GPIO_led.GPIO_PinConfig.GPIO_PinPupdControl = GPIO_NO_PUPD;
		GPIO_PerClockControl(GPIOD, ENABLE);
		GPIO_PerClockControl(GPIOA, ENABLE);
		GPIO_Inti(&GPIO_Button);
		GPIO_Inti(&GPIO_led);
		GPIO_IRQInterruptConfig(IRQ_NO_EXTI0,ENABLE);
		GPIO_IRQPriorityConfig(IRQ_NO_EXTI0, 15);
		while(1);

}
void EXTI0_IRQHandler(void)
{
	GPIO_IRQHandling(GPIO_PIN_NO_0);
	GPIO_ToggleOutPutPin(GPIOD,GPIO_PIN_NO_12);
}
