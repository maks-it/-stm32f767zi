/*
 * lcd1602_text.c
 *
 *  Created on: Apr 22, 2025
 *      Author: Maksym Sadovnychyy (MAKS-IT)
 */

#include "lcd1602_text.h"
#include "lcd1602_i2c.h"
#include <string.h>

void lcd_scroll_text(LCD1602_I2C_HandleTypeDef *lcd, const char *text, uint8_t row, uint16_t delay_ms)
{
    size_t len = strlen(text);

    if (len <= 16)
    {
        lcd_gotoxy(lcd, 0, row);
        lcd_puts(lcd, text);
        return;
    }

    for (size_t i = 0; i <= len - 16; ++i)
    {
        lcd_gotoxy(lcd, 0, row);
        for (uint8_t j = 0; j < 16; ++j)
        {
            lcd_putchar(lcd, text[i + j]);
        }
        HAL_Delay(delay_ms);
    }
}
