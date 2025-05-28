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
 * @file ws2812.h
 * 
 * @brief Constant struct and function prototypes for the ws2812 driver.
 * 
 * This contains the constants, struct and function prototypes for the ws2812 
 * driver for use with the Raspberry Pi Pico microcontroller board.
 *   
 * @author Kevin Thomas
 * @date   04/11/2024
*/

#ifndef _WS2812_H
#define _WS2812_H

#include "pico/stdlib.h"
#include "pins.h"
#include "led_remapping.h"
#include <math.h>

/**
 * @brief Structure for WS2812 LED configuration.
 *
 * This structure defines the configuration for controlling WS2812 LEDs.
 */
typedef struct {
    uint8_t pin;
} ws2812_config_t;

/**
 * @brief External declaration of WS2812 LED configuration.
 *
 * This declares an external variable `ws2812_config` of type `ws2812_config_t`,
 * which holds the configuration settings for controlling WS2812 LEDs.
 * The actual definition of this variable is expected to be provided in another 
 * source file.
 */
extern ws2812_config_t ws2812_config;

/**
 * @brief Initializes WS2812 LED control.
 *
 * This function initializes the GPIO pin specified in the provided `ws2812_config`
 * for controlling WS2812 LEDs. It sets up the GPIO pin direction for output.
 *
 * @param  ws2812_config Pointer to the configuration structure (`ws2812_config_t`)
 *                       containing the GPIO pin used for WS2812 LED control.
 * @return None
 */
void ws2812_init(ws2812_config_t *ws2812_config);

/**
 * @brief Sends a single bit over the bus to the WS2812.
 *
 * This function sends data to a WS2812 LED strip using a specified GPIO pin.
 * Uses loops to match the WS2812's timing protocol whereby a 1 is transmitted as 
 * (800 ns high, 450 ns low). A 0 is transmitted as (400 ns high, 850 ns low).
 *
 * @param  ws2812_config Pointer to the configuration structure (`ws2812_config_t`)
 *                       containing the GPIO pin used for WS2812 LED control.
 * @param  bit        Boolean value to be transmitted to the WS2812 LED.
 * @return None
 */
void ws2812_send_bit(ws2812_config_t *ws2812_config, bool bit);

/**
 * @brief Sends a full color over the bus to the WS2812.
 *
 * This function sends a single point of color data to a WS2812 LED strip using a specified GPIO pin.
 * Makes recurrent calls to ws2812_send_bit to transmit each bit in the color value which is given as a 32 bit integer.
 * 
 *
 * @param  ws2812_config Pointer to the configuration structure (`ws2812_config_t`)
 *                       containing the GPIO pin used for WS2812 LED control.
 * @param  set_color     32 bit integer value representing a GRB color value. The 8 most significant bits are unused. 
 * @return None
 */
void ws2812_send_color(ws2812_config_t *ws2812_config, uint32_t set_color);


/**
 * @brief Sends an array of colors to the WS2812 Led device.
 *
 * This function sends an array of color data to a WS2812 LED strip using a specified GPIO pin.
 * Makes recurrent calls to ws2812_send_bit to transmit each bit in the color value which is given as a 32 bit integer.
 * 
 *
 * @param  ws2812_config Pointer to the configuration structure (`ws2812_config_t`)
 *                       containing the GPIO pin used for WS2812 LED control.
 * @param  colors        An array of size num_leds containing color values corresponding to each pixel in the array.
 *                       The array is single dimensional.  
 * @param  num_leds      The total number of leds in the led array.
 * @return None
 */
void send_colors(ws2812_config_t *ws2812_config, uint32_t* colors, uint32_t num_leds);

/**
 * @brief Clears the led array.
 *
 * Sets all of the LED values to 0
 * 
 *
 * @param  ws2812_config Pointer to the configuration structure (`ws2812_config_t`)
 *                       containing the GPIO pin used for WS2812 LED control.
 * @param  num_leds      The total number of leds in the led array.
 * @return None
 */
void clear_colors(ws2812_config_t *ws2812_config, uint32_t num_leds);

/**
 * @brief Returns a 32 bit integer from an explicit RGB color value.
 *
 * This function makes it convenient to turn explicit R, G, and B values into a 32 bit integer.
 *
 * @param  r    Red color value.
 * @param  g    Green color value.
 * @param  b    Blue color value
 * @param  brightness   Float specified to adjust brightness
 * @return None
 */
uint32_t rgb_color(uint8_t r, uint8_t g, uint8_t b, float brightness);


/**
 *  
 * 
 * Color Animations
 * 
 */

void sensor_wave(uint32_t *led_strip, uint32_t** remapping, int i, float brightness, int rows, int cols, uint32_t temperature, uint32_t pressure);
#endif /* _WS2812_H */