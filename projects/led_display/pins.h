/**
 * MIT License
 * 
 * Copyright (c) 2024 My Techno Talent
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/**
 * @file pins.h
 *
 * @brief Config defines for the Pico board.
 * 
 * This contains the config defines for the  Raspberry Pi Pico microcontroller 
 * board.
 *
 * @author Kevin Thomas
 * @date   04/10/2024
 */

#ifndef _PINS_H_
#define _PINS_H_

#include "pico/stdlib.h"

/** 
 * @brief Defines ws2812 LED config value.
 * 
 * These config defines are used to target ws2812 LED pin.
 */
#define WS2812                           18

/** 
 * @brief Defines LED config values.
 * 
 * These config defines are used to target LED pins.
 */
#define LED                              25 

/**
 * @brief These config defines are used for the mode changing button
 * 
 * 
 */
#define BUTTON                           22

/** 
 * @brief Defines button config values.
 * 
 * These config defines are used to target button pins.
 */
#define NO_BUTTON                        255
#define LEFT_BUTTON                      4
#define RIGHT_BUTTON                     5
#define SUBMIT_BUTTON                    7

/** 
 * @brief Defines SSD1306 config values.
 * 
 * These config defines are used to target SSD1306 registers.
 */
#define I2C                              i2c1
#define SDA                              2
#define SCL                              3

#endif /* _PINS_H_ */