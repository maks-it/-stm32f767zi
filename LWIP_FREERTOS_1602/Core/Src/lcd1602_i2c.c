/* lcd1602_i2c.c
 *
 *  Created on: Apr 15, 2025
 *      Author: Maksym Sadovnychyy (MAKS-IT)
 *  Improved on: Apr 16, 2025
 *
 *  Most generic 1602 LCD modules with an I2C interface are built around the HD44780 controller (or a compatible clone).
 *  The HD44780 datasheet is the primary reference for the full command set and bit definitions,
 *  including how to set up function parameters, control display on/off, clear display, and move the cursor.
 */

#include "lcd1602_i2c.h"

//------------------------------------------------------------------------------
// Macro definitions for LCD control bits for enhanced clarity
//------------------------------------------------------------------------------
#define LCD_EN_HIGH_CMD  0x0C  // Enable high, RS = 0 (command mode)
#define LCD_EN_LOW_CMD   0x08  // Enable low, RS = 0 (command mode)
#define LCD_EN_HIGH_DATA 0x0D  // Enable high, RS = 1 (data mode)
#define LCD_EN_LOW_DATA  0x09  // Enable low, RS = 1 (data mode)

//------------------------------------------------------------------------------
// Private helper function to send a byte (split as two nibbles) via I2C
//------------------------------------------------------------------------------
static void lcd_send_byte(LCD1602_I2C_HandleTypeDef *lcd, uint8_t byte, uint8_t isData)
{
    uint8_t upper_nibble = byte & 0xF0;
    uint8_t lower_nibble = (byte << 4) & 0xF0;
    uint8_t ctrl_high = (isData) ? LCD_EN_HIGH_DATA : LCD_EN_HIGH_CMD;
    uint8_t ctrl_low  = (isData) ? LCD_EN_LOW_DATA  : LCD_EN_LOW_CMD;
    uint8_t data_t[4] = {
        upper_nibble | ctrl_high,
        upper_nibble | ctrl_low,
        lower_nibble | ctrl_high,
        lower_nibble | ctrl_low
    };

    // Transmit the 4-byte sequence; add error handling as needed.
    if (HAL_I2C_Master_Transmit(lcd->hi2c, lcd->address, data_t, sizeof(data_t), 100) != HAL_OK)
    {
        // Error handling (e.g., logging or retry mechanism) can be implemented here.
    }
}

//------------------------------------------------------------------------------
// Sends a command byte to the LCD.
//------------------------------------------------------------------------------
void lcd_send_cmd(LCD1602_I2C_HandleTypeDef *lcd, uint8_t cmd)
{
    lcd_send_byte(lcd, cmd, 0);
}

//------------------------------------------------------------------------------
// Sends a data byte (character) to the LCD.
//------------------------------------------------------------------------------
void lcd_send_data(LCD1602_I2C_HandleTypeDef *lcd, uint8_t data)
{
    lcd_send_byte(lcd, data, 1);
}

//------------------------------------------------------------------------------
// Clears the LCD display using the built-in clear command.
//------------------------------------------------------------------------------
void lcd_clear(LCD1602_I2C_HandleTypeDef *lcd)
{
    lcd_send_cmd(lcd, 0x01);  // Clear display command
    HAL_Delay(2);             // Allow time for the LCD to process the command
}

//------------------------------------------------------------------------------
// Moves the cursor to a specified position on the LCD.
//------------------------------------------------------------------------------
void lcd_gotoxy(LCD1602_I2C_HandleTypeDef *lcd, uint8_t col, uint8_t row)
{
    uint8_t address;

    switch (row)
    {
        case 0: address = 0x80 + col; break;  // First row
        case 1: address = 0xC0 + col; break;  // Second row
        case 2: address = 0x94 + col; break;  // Third row
        case 3: address = 0xD4 + col; break;  // Fourth row
        default: return;                      // Invalid row number: do nothing
    }

    lcd_send_cmd(lcd, address);
}

//------------------------------------------------------------------------------
// Initializes the LCD in 4-bit mode.
//------------------------------------------------------------------------------
void lcd_init(LCD1602_I2C_HandleTypeDef *lcd)
{
    HAL_Delay(50);  // Wait for LCD power-up

    // Begin initialization sequence: three wake-up commands
    lcd_send_cmd(lcd, 0x30);  // First wake-up command
    HAL_Delay(5);
    lcd_send_cmd(lcd, 0x30);  // Second wake-up command
    HAL_Delay(1);
    lcd_send_cmd(lcd, 0x30);  // Third wake-up command
    HAL_Delay(10);
    lcd_send_cmd(lcd, 0x20);  // Set to 4-bit mode
    HAL_Delay(10);

    // LCD configuration commands
    lcd_send_cmd(lcd, 0x28);  // Function set: 4-bit mode, 2 lines, 5x8 font
    HAL_Delay(1);
    lcd_send_cmd(lcd, 0x08);  // Display control: display off, cursor off, blink off
    HAL_Delay(1);
    lcd_send_cmd(lcd, 0x01);  // Clear display
    HAL_Delay(2);
    lcd_send_cmd(lcd, 0x06);  // Entry mode set: cursor moves right, no display shift
    HAL_Delay(1);
    lcd_send_cmd(lcd, 0x0C);  // Display control: display on, cursor off, blink off
}

//------------------------------------------------------------------------------
// Sends a null-terminated string to the LCD.
//------------------------------------------------------------------------------
void lcd_puts(LCD1602_I2C_HandleTypeDef *lcd, const char *str)
{
    while (*str)
    {
        lcd_send_data(lcd, (uint8_t)*str++);
    }
}

//------------------------------------------------------------------------------
// Sends a single character to the LCD.
//------------------------------------------------------------------------------
void lcd_putchar(LCD1602_I2C_HandleTypeDef *lcd, char ch)
{
    lcd_send_data(lcd, (uint8_t)ch);
}
