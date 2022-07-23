/*
 * lcd.c
 *
 *  Created on: ١٣‏/٠٣‏/٢٠٢٢
 *      Author: ans
 */

#include "lcd.h"

static void lcd_enable(void);
static void write_4_bit(uint8_t value);
static void mdelay(uint32_t delay);
static void udelay(uint32_t delay);

void lcd_send_command(uint8_t cmd)
{
	GPIO_WriteToOutputPin(LCD_PORT, LCD_RS, GPIO_PIN_RESET);
	GPIO_WriteToOutputPin(LCD_PORT, LCD_RW, GPIO_PIN_RESET);

	write_4_bit(cmd >> 4);
	write_4_bit(cmd & 0x0F);
}
void lcd_send_char(uint8_t data)
{
	GPIO_WriteToOutputPin(LCD_PORT, LCD_RS, GPIO_PIN_SET);
	GPIO_WriteToOutputPin(LCD_PORT, LCD_RW, GPIO_PIN_RESET);

	write_4_bit(data >> 4);
	write_4_bit(data & 0x0F);
}

void lcd_send_string(char *message)
{
	do
	{
		lcd_send_char((uint8_t)*message++);
	}while(*message != '\0');
}

void Lcd_Init(void)
{
	GPIO_Handle_t lcd_signal;
	lcd_signal.pGPIOx = GPIOD;
	lcd_signal.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUTPUT;
	lcd_signal.GPIO_PinConfig.GPIO_PinNumber = LCD_RS;
	lcd_signal.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	lcd_signal.GPIO_PinConfig.GPIO_PinPupdControl = GPIO_NO_PUPD;
	lcd_signal.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

	GPIO_Inti(&lcd_signal);

	lcd_signal.GPIO_PinConfig.GPIO_PinNumber = LCD_RW;
	GPIO_Inti(&lcd_signal);

	lcd_signal.GPIO_PinConfig.GPIO_PinNumber = LCD_EN;
	GPIO_Inti(&lcd_signal);

	lcd_signal.GPIO_PinConfig.GPIO_PinNumber = LCD_D4;
	GPIO_Inti(&lcd_signal);

	lcd_signal.GPIO_PinConfig.GPIO_PinNumber = LCD_D5;
	GPIO_Inti(&lcd_signal);

	lcd_signal.GPIO_PinConfig.GPIO_PinNumber = LCD_D6;
	GPIO_Inti(&lcd_signal);

	lcd_signal.GPIO_PinConfig.GPIO_PinNumber = LCD_D7;
	GPIO_Inti(&lcd_signal);

	GPIO_WriteToOutputPin(LCD_PORT, LCD_RS, GPIO_PIN_RESET);
	GPIO_WriteToOutputPin(LCD_PORT, LCD_RW, GPIO_PIN_RESET);
	GPIO_WriteToOutputPin(LCD_PORT, LCD_EN, GPIO_PIN_RESET);
	GPIO_WriteToOutputPin(LCD_PORT, LCD_D4, GPIO_PIN_RESET);
	GPIO_WriteToOutputPin(LCD_PORT, LCD_D5, GPIO_PIN_RESET);
	GPIO_WriteToOutputPin(LCD_PORT, LCD_D6, GPIO_PIN_RESET);
	GPIO_WriteToOutputPin(LCD_PORT, LCD_D7, GPIO_PIN_RESET);
	//2. DO the INitialization
	mdelay(40);
	GPIO_WriteToOutputPin(LCD_PORT, LCD_RS, GPIO_PIN_RESET);
	GPIO_WriteToOutputPin(LCD_PORT, LCD_RW, GPIO_PIN_RESET);

	write_4_bit(0x3);

	mdelay(5);

	write_4_bit(0x3);

	udelay(150);

	write_4_bit(0x3);
	write_4_bit(0x2);

	lcd_send_command(LCD_CMD_4DL_2N_5X8F);
	lcd_send_command(LCD_CMD_DON_CURON);
	lcd_display_clear();
	lcd_send_command(LCD_CMD_INCADD);
}

static void write_4_bit(uint8_t value)
{
	GPIO_WriteToOutputPin(LCD_PORT, LCD_D4, ((value>>0) & 0x01));
	GPIO_WriteToOutputPin(LCD_PORT, LCD_D5, ((value>>1) & 0x01));
	GPIO_WriteToOutputPin(LCD_PORT, LCD_D6, ((value>>2) & 0x01));
	GPIO_WriteToOutputPin(LCD_PORT, LCD_D7, ((value>>3) & 0x01));

	lcd_enable();

}

void lcd_set_cursor(uint8_t row, uint8_t column)
{
	column--;
	switch(row)
	{
	case 1:
		lcd_send_command((column|= 0x80));
		break;
	case 2:
		lcd_send_command((column |= 0xc0));
		break;
	default:
		break;
	}
}

void lcd_display_clear(void)
{
	lcd_send_command(LCD_CMD_DIS_CLEAR);
	mdelay(2);
}

void lcd_display_return_home(void)
{
	lcd_send_command(LCD_CMD_DIS_RETURN_HOME);
	mdelay(2);
}
static void lcd_enable(void)
{
	GPIO_WriteToOutputPin(LCD_PORT, LCD_EN, GPIO_PIN_SET);
	udelay(10);
	GPIO_WriteToOutputPin(LCD_PORT, LCD_EN, GPIO_PIN_RESET);
	udelay(100);
}
static void mdelay(uint32_t delay)
{
	for(uint32_t i =0;i<(delay*1000);i++);
}
static void udelay(uint32_t delay)
{
	for(uint32_t i =0;i<(delay*1);i++);
}
