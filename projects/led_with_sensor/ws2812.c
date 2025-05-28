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
 * @file ws2812.c
 *
 * @brief Struct inits and functions for the ws2812 driver.
 * 
 * This contains the struct initializations and functions for the ws2812 driver 
 * for use with the Raspberry Pi Pico microcontroller board.
 *
 * @author Kevin Thomas
 * @date   04/11/2023
 */

#include "ws2812.h"

ws2812_config_t ws2812_config = {
    .pin = WS2812
};

/*
    Color Management 
*/
// Helper function to create an RGB color
uint32_t rgb_color(uint8_t r, uint8_t g, uint8_t b, float brightness) {
    if (brightness < 0 | brightness > 1.0)
    {
        printf("Brightness must be a value between 0 and 1");
        return 0;
    }
    r = (uint8_t)(r * brightness);
    g = (uint8_t)(g * brightness);
    b = (uint8_t)(b * brightness);
    return ((uint32_t)(g) << 16) | ((uint32_t)(r) << 8) | (uint32_t)(b);
}

void ws2812_init(ws2812_config_t *ws2812_config) {
    gpio_init(ws2812_config->pin);
    gpio_set_dir(ws2812_config->pin, 1);
}

void ws2812_send_bit(ws2812_config_t *ws2812_config, bool bit)
{
    uint8_t index = 0;
    // uint64_t start, end;

    // start = time_us_64();

    if (bit) {
        // one
        for (index = 0; index <= 10; index++)
            gpio_put(ws2812_config->pin, 1);
        for (index = 0; index <= 2; index++)
            gpio_put(ws2812_config->pin, 0);
    } else {
        // zero
        for (index = 0; index <= 3; index++)
            gpio_put(ws2812_config->pin, 1);
        for (index = 0; index <= 10; index++)
            gpio_put(ws2812_config->pin, 0);
    }

    // end = time_us_64();

    // printf("ws2812_send_bit(%d) took %llu ns\n", bit, (end - start) * 1000);
}

void ws2812_send_color(ws2812_config_t *ws2812_config, uint32_t set_color)
{
    for (int b = 23; b >= 0; b--) // Big endian
    {
        ws2812_send_bit(ws2812_config, set_color & (1<<b));
    }
}

void send_colors(ws2812_config_t *ws2812_config, uint32_t* colors, uint32_t num_leds)
{
    // Send each color
    for (int i = 0; i < num_leds; i++)
    {
        ws2812_send_color(ws2812_config, colors[i]);
    }

    // Send the end signal: 50us low
    gpio_put(ws2812_config->pin, 0);
    sleep_us(50);
}

void clear_colors(ws2812_config_t *ws2812_config, uint32_t num_leds)
{
    for (int i = 0;  i < num_leds; i++)
    {
        ws2812_send_color(ws2812_config, 0x000000); // Send 0 to clear the color in the LED array
    }

    gpio_put(ws2812_config->pin, 0);
    sleep_us(60);
}

/**
 * 
 * 
 * Color Animations
 * 
 * 
*/
