/*
 * lcd1602_text.h
 *
 *  Created on: Apr 22, 2025
 *      Author: Maksym Sadovnychyy (MAKS-IT)
 */

#ifndef INC_LCD1602_TEXT_H_
#define INC_LCD1602_TEXT_H_

#include "lcd1602_i2c.h"

void lcd_scroll_text(LCD1602_I2C_HandleTypeDef *lcd, const char *text, uint8_t row, uint16_t delay_ms);

#endif /* INC_LCD1602_TEXT_H_ */
