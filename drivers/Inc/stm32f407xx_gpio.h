/*
 * stm32f407xx_gpio.h
 *
 *  Created on: ١٥‏/٠١‏/٢٠٢٢
 *      Author: ans
 */

#ifndef INC_STM32F407XX_GPIO_H_
#define INC_STM32F407XX_GPIO_H_

#include"stm32f407xx.h"

typedef struct
{
	uint8_t GPIO_PinNumber;
	uint8_t GPIO_PinMode;
	uint8_t GPIO_PinSpeed;
	uint8_t GPIO_PinPupdControl;
	uint8_t GPIO_PinOPType;
	uint8_t GPIO_PinAltFunMode;
}GPIO_PinConfig_t;



typedef struct
{
	// pointer to hold the base addresses of the gpio peripherals

	GPIO_RegDef_t *pGPIOx; //this hold base address of the gpio port which pin belong
	GPIO_PinConfig_t GPIO_PinConfig;
}GPIO_Handle_t;
/*
 * GPIO pin numbers
 */
#define GPIO_PIN_NO_0         0
#define GPIO_PIN_NO_1         1
#define GPIO_PIN_NO_2         2
#define GPIO_PIN_NO_3         3
#define GPIO_PIN_NO_4         4
#define GPIO_PIN_NO_5         5
#define GPIO_PIN_NO_6         6
#define GPIO_PIN_NO_7         7
#define GPIO_PIN_NO_8         8
#define GPIO_PIN_NO_9         9
#define GPIO_PIN_NO_10        10
#define GPIO_PIN_NO_11        11
#define GPIO_PIN_NO_12        12
#define GPIO_PIN_NO_13        13
#define GPIO_PIN_NO_14        14
#define GPIO_PIN_NO_15        15

/*
 * the GPIO possible modes
 */

#define GPIO_MODE_INPUT    0
#define GPIO_MODE_OUTPUT   1
#define GPIO_MODE_ALTFN    2
#define GPIO_MODE_ANALOG   3
#define GPIO_MODE_IT_FT    4
#define GPIO_MODE_IT_RT    5
#define GPIO_MODE_IT_RFT   6

/*
 * GPIO pin possible OUTput types
 */

#define GPIO_OP_TYPE_PP   0
#define GPIO_OP_TYPE_OD   1

/*
 * GPIO pin possible speed
 */

#define GPIO_SPEED_LOW       0
#define GPIO_SPEED_MEDIUM    1
#define GPIO_SPEED_FAST      2
#define GPIO_SPEED_HIGH      3

/*
 * GPIO PIN pull up and pull down
 */

#define GPIO_NO_PUPD             0
#define GPIO_PIN_PU              1
#define GPIO_PIN_PD              2


/*
 * API supported by this driver
 */

/*
 * this for enabling clock
 */
void GPIO_PerClockControl(GPIO_RegDef_t *pGPIOx, uint8_t EnorDi);

/*
 * this for intialization
 */
void GPIO_Inti(GPIO_Handle_t *pGPIOHandle);
void GPIO_DeInti(GPIO_RegDef_t *pGPIOx);
/*
 * data read and write
 */
uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx,uint8_t PinNumber);
uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx);
void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx,uint8_t PinNumber,uint8_t Value);
void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx,uint16_t Value);
void GPIO_ToggleOutPutPin(GPIO_RegDef_t *pGPIOx,uint8_t PinNumber);
/*
 * IRQ config and ISR handling
 */

void GPIO_IRQInterruptConfig(uint8_t IRQNumber,uint8_t EnorDi);
void GPIO_IRQPriorityConfig(uint8_t IRQNumber,uint8_t IRQPriority);
void GPIO_IRQHandling(uint8_t PinNumber);





#endif /* INC_STM32F407XX_GPIO_H_ */
