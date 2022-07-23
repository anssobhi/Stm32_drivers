/*
 * stm32f407xx.h
 *
 *  Created on: Jan 13, 2022
 *      Author: ans
 */

#ifndef INC_STM32F407XX_H_
#define INC_STM32F407XX_H_
// base addresses of flash and SRAM memory


#include <stdint.h>
#include <stddef.h>

#define NVIC_ISER0       			((volatile uint32_t*)0xE000E100)
#define NVIC_ISER1       			((volatile uint32_t*)0xE000E104)
#define NVIC_ISER2       			((volatile uint32_t*)0xE000E108)
#define NVIC_ISER3       			((volatile uint32_t*)0xE000E10C)

#define NVIC_ICER0       			((volatile uint32_t*)0xE000E180)
#define NVIC_ICER1       			((volatile uint32_t*)0xE000E184)
#define NVIC_ICER2       			((volatile uint32_t*)0xE000E188)
#define NVIC_ICER3       			((volatile uint32_t*)0xE000E18C)

#define NVIC_PR_BASEADDR       		((volatile uint32_t*)0xE000E400)
#define NO_PR_BITS_IMPLEMENTED     	4


#define FLASH_BASEADDR        		0x08000000U
#define SRAM1_BASEADDR        		0x20000000U
#define SRAM2_BASEADDR        		0x2001C000U
#define ROM                   		0x1FFF0000U
#define SRAM                  		SRAM1_BASEADDR

// base addresses of AHBx and APBx

#define PERIPH_BASE            		0x40000000U
#define APB1PERIPH_BASE        		PERIPH_BASE
#define APB2PERIPH_BASE        		0x40010000U
#define AHB1PERIPH_BASE        		0x40020000U
#define AHB2PERIPH_BASE        		0x50000000U

// base addresses of all peripheral hanging on AHB1

#define GPIOA_BASEADDRE      	  	(AHB1PERIPH_BASE + 0x0000)
#define GPIOB_BASEADDRE        		(AHB1PERIPH_BASE + 0x0400)
#define GPIOC_BASEADDRE        		(AHB1PERIPH_BASE + 0x0800)
#define GPIOD_BASEADDRE        		(AHB1PERIPH_BASE + 0x0C00)
#define GPIOE_BASEADDRE        		(AHB1PERIPH_BASE + 0x1000)
#define GPIOF_BASEADDRE        		(AHB1PERIPH_BASE + 0x1400)
#define GPIOG_BASEADDRE        		(AHB1PERIPH_BASE + 0x1800)
#define GPIOH_BASEADDRE        		(AHB1PERIPH_BASE + 0x1C00)
#define GPIOI_BASEADDRE        		(AHB1PERIPH_BASE + 0x2000)
#define RCC_BASEADDRE          		(AHB1PERIPH_BASE + 0x3800)
// base addresses of all peripheral hanging on APB1

/*
 *
 *
#define TIM2_BASEADDRE         (APB1PERIPH_BASE + 0x0000)
#define TIM3_BASEADDRE         (APB1PERIPH_BASE + 0x0400)
#define TIM4_BASEADDRE         (APB1PERIPH_BASE + 0x0800)
#define TIM5_BASEADDRE         (APB1PERIPH_BASE + 0x0C00)
#define TIM6_BASEADDRE         (APB1PERIPH_BASE + 0x1000)
#define TIM7_BASEADDRE         (APB1PERIPH_BASE + 0x1400)
#define TIM12_BASEADDRE        (APB1PERIPH_BASE + 0x1800)
#define TIM13_BASEADDRE        (APB1PERIPH_BASE + 0x1C00)
#define TIM14_BASEADDRE        (APB1PERIPH_BASE + 0x2000)
 */

#define I2C1_BASEADDRE         (APB1PERIPH_BASE + 0x5400)
#define I2C2_BASEADDRE         (APB1PERIPH_BASE + 0x5800)
#define I2C3_BASEADDRE         (APB1PERIPH_BASE + 0x5C00)
#define SPI2_BASEADDRE         (APB1PERIPH_BASE + 0x3800)
#define SPI3_BASEADDRE         (APB1PERIPH_BASE + 0x3C00)
#define USART2_BASEADDRE       (APB1PERIPH_BASE + 0x4400)
#define USART3_BASEADDRE       (APB1PERIPH_BASE + 0x4800)
#define UART4_BASEADDRE        (APB1PERIPH_BASE + 0x4C00)
#define UART5_BASEADDRE        (APB1PERIPH_BASE + 0x5000)

// base addresses of all peripheral hanging on APB2

#define SPI1_BASEADDRE           (APB2PERIPH_BASE + 0x3000)
#define USART1_BASEADDRE         (APB2PERIPH_BASE + 0x1000)
#define USART6_BASEADDRE         (APB2PERIPH_BASE + 0x1400)
#define EXTI_BASEADDRE           (APB2PERIPH_BASE + 0x3C00)
#define SYSCFG_BASEADDRE         (APB2PERIPH_BASE + 0x3800)

typedef struct
{
	volatile uint32_t MODER;
	volatile uint32_t OTYPER;
	volatile uint32_t OSPEEDR;
	volatile uint32_t PUPDR;
	volatile uint32_t IDR;
	volatile uint32_t ODR;
	volatile uint32_t BSRR;
	volatile uint32_t LCKR;
	volatile uint32_t AFR[2];


}GPIO_RegDef_t;

typedef struct
{
	volatile uint32_t CR;
	volatile uint32_t PLLCFG;
	volatile uint32_t CFGR;
	volatile uint32_t CIR;
	volatile uint32_t AHB1RSTR;
	volatile uint32_t AHB2RSTR;
	volatile uint32_t AHB3RSTR;
	uint32_t Reserved0;
	volatile uint32_t APB1RSTR;
	volatile uint32_t APB2RSTR;
	uint32_t Reserved1;
	uint32_t Reserved2;
	volatile uint32_t AHB1ENR;
	volatile uint32_t AHB2ENR;
	volatile uint32_t AHB3ENR;
	uint32_t Reserved3;
	volatile uint32_t APB1ENR;
	volatile uint32_t APB2ENR;
	uint32_t Reserved4;
	uint32_t Reserved5;
	volatile uint32_t AHB1LPENR;
	volatile uint32_t AHB2LPENR;
	volatile uint32_t AHB3LPENR;
	uint32_t Reserved6;
	volatile uint32_t APB1LPENR;
	volatile uint32_t APB2LPENR;
	uint32_t Reserved7;
	uint32_t Reserved8;
	volatile uint32_t BDCR;
	volatile uint32_t CSR;
	uint32_t Reserved9;
	uint32_t Reserved10;
	volatile uint32_t SSCGR;
	volatile uint32_t PLLI2SCFGR;
	volatile uint32_t PLLSAICFGR;
	volatile uint32_t DCKCFGR;

}RCC_RegDef_t;

typedef struct
{

	volatile uint32_t IMR;
	volatile uint32_t EMR;
	volatile uint32_t RTSR;
	volatile uint32_t FTSR;
	volatile uint32_t SWIER;
	volatile uint32_t PR;


}EXTI_RegDef_t;

typedef struct
{

	volatile uint32_t MEMRMP;
	volatile uint32_t PMC;
	volatile uint32_t EXTICR[4];
	uint32_t RESERVED[2];
    volatile uint32_t CMPCR;

}SYSCFG_RegDef_t;

typedef struct
{
	volatile uint32_t CR1;
	volatile uint32_t CR2;
	volatile uint32_t SR;
	volatile uint32_t DR;
	volatile uint32_t CRCPR;
	volatile uint32_t RXCRCR;
	volatile uint32_t TXCRCR;
	volatile uint32_t I2SCFGR;
	volatile uint32_t I2SPR;

}SPI_RegDef_t;

typedef struct
{

	volatile uint32_t CR1;
	volatile uint32_t CR2;
	volatile uint32_t OAR1;
	volatile uint32_t OAR2;
	volatile uint32_t DR;
	volatile uint32_t SR1;
	volatile uint32_t SR2;
	volatile uint32_t CCR;
	volatile uint32_t TRISE;
	volatile uint32_t FLTR;

}I2C_RegDef_t;

typedef struct
{
	volatile uint32_t SR;
	volatile uint32_t DR;
	volatile uint32_t BRR;
	volatile uint32_t CR1;
	volatile uint32_t CR2;
	volatile uint32_t CR3;
	volatile uint32_t GTPR;

}USART_RegDef_t;

#define GPIOA  ((GPIO_RegDef_t*)GPIOA_BASEADDRE)
#define GPIOB  ((GPIO_RegDef_t*)GPIOB_BASEADDRE)
#define GPIOC  ((GPIO_RegDef_t*)GPIOC_BASEADDRE)
#define GPIOD  ((GPIO_RegDef_t*)GPIOD_BASEADDRE)
#define GPIOE  ((GPIO_RegDef_t*)GPIOE_BASEADDRE)
#define GPIOF  ((GPIO_RegDef_t*)GPIOF_BASEADDRE)
#define GPIOG  ((GPIO_RegDef_t*)GPIOG_BASEADDRE)
#define GPIOH  ((GPIO_RegDef_t*)GPIOH_BASEADDRE)
#define GPIOI  ((GPIO_RegDef_t*)GPIOI_BASEADDRE)

#define RCC       ((RCC_RegDef_t*)RCC_BASEADDRE)
#define SYSCFG    ((SYSCFG_RegDef_t*)SYSCFG_BASEADDRE)
#define EXTI      ((EXTI_RegDef_t*)EXTI_BASEADDRE)

#define SPI1   ((SPI_RegDef_t*)SPI1_BASEADDRE)
#define SPI2   ((SPI_RegDef_t*)SPI2_BASEADDRE)
#define SPI3   ((SPI_RegDef_t*)SPI3_BASEADDRE)

#define I2C1   ((I2C_RegDef_t*)I2C1_BASEADDRE)
#define I2C2   ((I2C_RegDef_t*)I2C2_BASEADDRE)
#define I2C3   ((I2C_RegDef_t*)I2C3_BASEADDRE)

#define USART1 ((USART_RegDef_t*)USART1_BASEADDRE)
#define USART2 ((USART_RegDef_t*)USART2_BASEADDRE)
#define USART3 ((USART_RegDef_t*)USART3_BASEADDRE)
#define UART4  ((USART_RegDef_t*)UART4_BASEADDRE)
#define UART5  ((USART_RegDef_t*)UART5_BASEADDRE)
#define USART6 ((USART_RegDef_t*)USART6_BASEADDRE)

/*
 * clock enable macros for GPIOx peripheral
 */
#define GPIOA_PCLK_EN()   (RCC->AHB1ENR |= (1<<0))
#define GPIOB_PCLK_EN()   (RCC->AHB1ENR |= (1<<1))
#define GPIOC_PCLK_EN()   (RCC->AHB1ENR |= (1<<2))
#define GPIOD_PCLK_EN()   (RCC->AHB1ENR |= (1<<3))
#define GPIOE_PCLK_EN()   (RCC->AHB1ENR |= (1<<4))
#define GPIOF_PCLK_EN()   (RCC->AHB1ENR |= (1<<5))
#define GPIOG_PCLK_EN()   (RCC->AHB1ENR |= (1<<6))
#define GPIOH_PCLK_EN()   (RCC->AHB1ENR |= (1<<7))
#define GPIOI_PCLK_EN()   (RCC->AHB1ENR |= (1<<8))

#define SYSCFG_PCLK_EN()   (RCC->APB2ENR |= (1<<14))

#define SPI1_PCLK_EN()     (RCC->APB2ENR |= (1<<12))
#define SPI2_PCLK_EN()     (RCC->APB1ENR |= (1<<14))
#define SPI3_PCLK_EN()     (RCC->APB1ENR |= (1<<15))

#define I2C1_PCLK_EN()      (RCC->APB1ENR |= (1<<21))
#define I2C2_PCLK_EN()      (RCC->APB1ENR |= (1<<22))
#define I2C3_PCLK_EN()      (RCC->APB1ENR |= (1<<23))

#define USART1_PCLK_EN()    (RCC->APB2ENR |= (1<<4))
#define USART2_PCLK_EN()    (RCC->APB1ENR |= (1<<17))
#define USART3_PCLK_EN()    (RCC->APB1ENR |= (1<<18))
#define UART4_PCLK_EN()     (RCC->APB1ENR |= (1<<19))
#define UART5_PCLK_EN()     (RCC->APB1ENR |= (1<<20))
#define USART6_PCLK_EN()    (RCC->APB2ENR |= (1<<5))

/*
 * clock disable macros for GPIOx peripheral
 */

#define GPIOA_PCLK_DI()   (RCC->AHB1ENR &= ~(1<<0))
#define GPIOB_PCLK_DI()   (RCC->AHB1ENR &= ~(1<<1))
#define GPIOC_PCLK_DI()   (RCC->AHB1ENR &= ~(1<<2))
#define GPIOD_PCLK_DI()   (RCC->AHB1ENR &= ~(1<<3))
#define GPIOE_PCLK_DI()   (RCC->AHB1ENR &= ~(1<<4))
#define GPIOF_PCLK_DI()   (RCC->AHB1ENR &= ~(1<<5))
#define GPIOG_PCLK_DI()   (RCC->AHB1ENR &= ~(1<<6))
#define GPIOH_PCLK_DI()   (RCC->AHB1ENR &= ~(1<<7))
#define GPIOI_PCLK_DI()   (RCC->AHB1ENR &= ~(1<<8))

#define SPI1_PCLK_DI()   (RCC->APB2ENR &= ~(1<<12))
#define SPI2_PCLK_DI()   (RCC->APB1ENR &= ~(1<<14))
#define SPI3_PCLK_DI()   (RCC->APB1ENR &= ~(1<<15))

#define I2C1_PCLK_DI()      (RCC->APB1ENR &= ~(1<<21))
#define I2C2_PCLK_DI()      (RCC->APB1ENR &= ~(1<<22))
#define I2C3_PCLK_DI()      (RCC->APB1ENR &= ~(1<<23))

#define USART1_PCLK_DI()    (RCC->APB2ENR &= ~(1<<4))
#define USART2_PCLK_DI()    (RCC->APB1ENR &= ~(1<<17))
#define USART3_PCLK_DI()    (RCC->APB1ENR &= ~(1<<18))
#define UART4_PCLK_DI()     (RCC->APB1ENR &= ~(1<<19))
#define UART5_PCLK_DI()     (RCC->APB1ENR &= ~(1<<20))
#define USART6_PCLK_DI()    (RCC->APB2ENR &= ~(1<<5))

#define GPIOA_REG_RESET()     do{(RCC->AHB1RSTR |= (1 << 0)); (RCC->AHB1RSTR &= ~(1 << 0)); }while(0)
#define GPIOB_REG_RESET()     do{(RCC->AHB1RSTR |= (1 << 1)); (RCC->AHB1RSTR &= ~(1 << 1)); }while(0)
#define GPIOC_REG_RESET()     do{(RCC->AHB1RSTR |= (1 << 2)); (RCC->AHB1RSTR &= ~(1 << 2)); }while(0)
#define GPIOD_REG_RESET()     do{(RCC->AHB1RSTR |= (1 << 3)); (RCC->AHB1RSTR &= ~(1 << 3)); }while(0)
#define GPIOE_REG_RESET()     do{(RCC->AHB1RSTR |= (1 << 4)); (RCC->AHB1RSTR &= ~(1 << 4)); }while(0)
#define GPIOF_REG_RESET()     do{(RCC->AHB1RSTR |= (1 << 5)); (RCC->AHB1RSTR &= ~(1 << 5)); }while(0)
#define GPIOG_REG_RESET()     do{(RCC->AHB1RSTR |= (1 << 6)); (RCC->AHB1RSTR &= ~(1 << 6)); }while(0)
#define GPIOH_REG_RESET()     do{(RCC->AHB1RSTR |= (1 << 7)); (RCC->AHB1RSTR &= ~(1 << 7)); }while(0)
#define GPIOI_REG_RESET()     do{(RCC->AHB1RSTR |= (1 << 8)); (RCC->AHB1RSTR &= ~(1 << 8)); }while(0)

#define GPIO_BASEADDR_TO_CODE(x)      ( (x == GPIOA)? 0 :\
		                              (x == GPIOB)? 1 :\
		                              (x == GPIOC)? 2 :\
		                              (x == GPIOD)? 3 :\
		                              (x == GPIOE)? 4 :\
		                              (x == GPIOF)? 5 :\
		                              (x == GPIOG)? 6 :\
		                              (x == GPIOH)? 7 :0 )

#define IRQ_NO_EXTI0              6
#define IRQ_NO_EXTI1              7
#define IRQ_NO_EXTI2              8
#define IRQ_NO_EXTI3              9
#define IRQ_NO_EXTI4              10
#define IRQ_NO_EXTI9_5            23
#define IRQ_NO_EXTI15_10          40

#define IRQ_NO_SPI1                35
#define IRQ_NO_SPI2                36
#define IRQ_NO_SPI3                51

#define IRQ_NO_I2C1_EV             31
#define IRQ_NO_I2C1_ER             32
#define IRQ_NO_I2C2_EV             33
#define IRQ_NO_I2C2_ER             34
#define IRQ_NO_I2C3_EV             72
#define IRQ_NO_I2C3_ER             73

#define IRQ_NO_USART1              37
#define IRQ_NO_USART2              38
#define IRQ_NO_USART3              39
#define IRQ_NO_UART4               52
#define IRQ_NO_UART5               53
#define IRQ_NO_USART6              71

#define ENABLE                 1
#define DISABLE                0
#define SET                    ENABLE
#define RESET                  DISABLE
#define GPIO_PIN_SET           SET
#define GPIO_PIN_RESET         RESET

#define FLAG_SET               SET
#define FLAG_RESET             RESET

#define SPI_CR1_CPHA                 0
#define SPI_CR1_CPOL                 1
#define SPI_CR1_MSTR                 2
#define SPI_CR1_BAUDRATE             3
#define SPI_CR1_SPE                  6
#define SPI_CR1_LSBFIRST             7
#define SPI_CR1_SSI                  8
#define SPI_CR1_SSM                  9
#define SPI_CR1_RXONLY               10
#define SPI_CR1_DFF                  11
#define SPI_CR1_CRCNEXT              12
#define SPI_CR1_CRCEN                13
#define SPI_CR1_BIDIOE               14
#define SPI_CR1_BIDIMODE             15


#define SPI_CR2_RXDMAEN              0
#define SPI_CR2_TXDMAEN              1
#define SPI_CR2_SSOE                 2
#define SPI_CR2_FRF                  4
#define SPI_CR2_ERRIE                5
#define SPI_CR2_RXNEIE               6
#define SPI_CR2_TXEIE                7

#define SPI_SR_RXNE                  0
#define SPI_SR_TXE                   1
#define SPI_SR_CHSIDE                2
#define SPI_SR_UDR                   3
#define SPI_SR_CRCERR                4
#define SPI_SR_MODF                  5
#define SPI_SR_OVR                   6
#define SPI_SR_BSY                   7
#define SPI_SR_FRE                   8

#define I2C_CR1_PE                      0
#define I2C_CR1_SMBUS                   1
#define I2C_CR1_SMBTYPE                 3
#define I2C_CR1_ENARP                   4
#define I2C_CR1_ENPEC                   5
#define I2C_CR1_ENGC                    6
#define I2C_CR1_NOSTRETCH               7
#define I2C_CR1_START                   8
#define I2C_CR1_STOP                    9
#define I2C_CR1_ACK                     10
#define I2C_CR1_POS                     11
#define I2C_CR1_PEC                     12
#define I2C_CR1_ALERT                   13
#define I2C_CR1_SWRST                   15

#define I2C_CR2_FREQ                    0
#define I2C_CR2_ITERREN                 8
#define I2C_CR2_ITEVTEN                 9
#define I2C_CR2_ITBUFEN                 10
#define I2C_CR2_DMAEN                   11
#define I2C_CR2_LAST                    12

#define I2C_SR1_SB                      0
#define I2C_SR1_ADDR                    1
#define I2C_SR1_BTF                     2
#define I2C_SR1_ADD10                   3
#define I2C_SR1_STOPF                   4
#define I2C_SR1_RxNE                    6
#define I2C_SR1_TxE                     7
#define I2C_SR1_BERR                    8
#define I2C_SR1_ARLO                    9
#define I2C_SR1_AF                      10
#define I2C_SR1_OVR                     11
#define I2C_SR1_PECERR                  12
#define I2C_SR1_TIMEOUT                 14
#define I2C_SR1_SMBALERT                15

#define I2C_SR2_MSL                     0
#define I2C_SR2_BUSY                    1
#define I2C_SR2_TRA                     2
#define I2C_SR2_GENCALL                 4
#define I2C_SR2_SMBDEFAULT              5
#define I2C_SR2_SMBHOST                 6
#define I2C_SR2_DUALF                   7

#define I2C_CCR_CCR                     0
#define I2C_CCR_DUTY                    14
#define I2C_CCR_FS                      15

#define USART_SR_PE                     0
#define USART_SR_FE                     1
#define USART_SR_NF                     2
#define USART_SR_ORE                    3
#define USART_SR_IDLE                   4
#define USART_SR_RXNE                   5
#define USART_SR_TC                     6
#define USART_SR_TXE                    7
#define USART_SR_LBD                    8
#define USART_SR_CTS                    9

#define USART_BRR_DIV_FRACTION          0
#define USART_BRR_DIV_MANTISSA          4

#define USART_CR1_SBK                   0
#define USART_CR1_RWU                   1
#define USART_CR1_RE                    2
#define USART_CR1_TE                    3
#define USART_CR1_IDLEIE                4
#define USART_CR1_RXNEIE                5
#define USART_CR1_TCIE                  6
#define USART_CR1_TXEIE                 7
#define USART_CR1_PEIE                  8
#define USART_CR1_PS                    9
#define USART_CR1_PCE                   10
#define USART_CR1_WAKE                  11
#define USART_CR1_M                     12
#define USART_CR1_UE                    13
#define USART_CR1_OVER8                 15

#define USART_CR2_ADD                    0
#define USART_CR2_LBDL                   5
#define USART_CR2_LBDIE                  6
#define USART_CR2_LBCL                   8
#define USART_CR2_CPHA                   9
#define USART_CR2_CPOL                   10
#define USART_CR2_CLKEN                  11
#define USART_CR2_STOP                   12
#define USART_CR2_LINEN                  14

#define USART_CR3_EIE                    0
#define USART_CR3_IREN                   1
#define USART_CR3_IRLP                   2
#define USART_CR3_HDSEL                  3
#define USART_CR3_NACK                   4
#define USART_CR3_SCEN                   5
#define USART_CR3_DMAR                   6
#define USART_CR3_DMAT                   7
#define USART_CR3_RTSE                   8
#define USART_CR3_CTSE                   9
#define USART_CR3_CTSIE                  10
#define USART_CR3_ONEBIT                 11

#define USART_GTPR_PSC                   0
#define USART_GTPR_GT                    8

#include "stm32f407xx_gpio.h"
#include "stm32f407xx_spi.h"
#include "stm32f407xx_I2c.h"
#include "stm32f407xx_usart.h"
#include "stm32f407xx_rcc.h"

#endif /* INC_STM32F407XX_H_ */
