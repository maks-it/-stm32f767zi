/* lcd1602_i2c.h
 *
 *  Created on: Apr 15, 2025
 *      Author: Maksym Sadovnychyy (MAKS-IT)
 *  Improved on: Apr 16, 2025
 *
 *  Most generic 1602 LCD modules with an I2C interface are built around the HD44780 controller (or a compatible clone).
 *  The HD44780 datasheet is the primary reference for the full command set and bit definitions,
 *  including how to set up function parameters, control display on/off, clear display, and move the cursor.
 */

#ifndef LCD1602_I2C_H
#define LCD1602_I2C_H

#include "stm32f7xx_hal.h"
#include <stdint.h>

/**
 * @brief Structure to hold LCD instance information.
 */
typedef struct {
    I2C_HandleTypeDef *hi2c;  // I2C handler for communication
    uint8_t address;          // I2C address of the LCD
} LCD1602_I2C_HandleTypeDef;

/**
 * @brief  Initializes the LCD in 4-bit mode.
 * @param  lcd: Pointer to the LCD handle.
 */
void lcd_init(LCD1602_I2C_HandleTypeDef *lcd);

/**
 * @brief  Sends a command byte to the LCD.
 * @param  lcd: Pointer to the LCD handle.
 * @param  cmd: Command byte to send.
 */
void lcd_send_cmd(LCD1602_I2C_HandleTypeDef *lcd, uint8_t cmd);

/**
 * @brief  Sends a data byte (character) to the LCD.
 * @param  lcd: Pointer to the LCD handle.
 * @param  data: Data byte to send.
 */
void lcd_send_data(LCD1602_I2C_HandleTypeDef *lcd, uint8_t data);

/**
 * @brief  Outputs a single character to the LCD.
 * @param  lcd: Pointer to the LCD handle.
 * @param  ch: Character to send.
 */
void lcd_putchar(LCD1602_I2C_HandleTypeDef *lcd, char ch);

/**
 * @brief  Outputs a null-terminated string to the LCD.
 * @param  lcd: Pointer to the LCD handle.
 * @param  str: String to send.
 */
void lcd_puts(LCD1602_I2C_HandleTypeDef *lcd, const char *str);

/**
 * @brief  Moves the cursor to a specified position on the LCD.
 * @param  lcd: Pointer to the LCD handle.
 * @param  col: Column number (0 based, typically 0-15).
 * @param  row: Row number (0 based, typically 0-3).
 */
void lcd_gotoxy(LCD1602_I2C_HandleTypeDef *lcd, uint8_t col, uint8_t row);

/**
 * @brief  Clears the LCD display.
 * @param  lcd: Pointer to the LCD handle.
 */
void lcd_clear(LCD1602_I2C_HandleTypeDef *lcd);

#endif /* LCD1602_I2C_H */
