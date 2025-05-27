/**
 * MIT License
 * 
 * Copyright (c) 2025 Benjamin Gunnels
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
 * for use with the Raspberry Pi Pico microcontroller board. Uses PIO on the board to 
 * control the timing protocol necessary for the ws2812 LED.
 * 
 * Credit to Kevin Thomas @ My Techno Talent 04/11/2023 for inspiration on this code. 
 *
 * @author Benjamin Gunnels
 * @date   05/20/2025
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

void rainbow(uint32_t *led_strip, uint32_t** remapping, int i, float brightness, int rows, int cols) 
{
    int row = 0;
    int col = 0;
    // Defining Rainbow Colors with Brightness Control (e.g. 0.3 = 30% brightness)
    uint32_t red = rgb_color(255, 0, 0, brightness);
    uint32_t orange = rgb_color(255, 165, 0, brightness);
    uint32_t yellow = rgb_color(255, 255, 0, brightness);
    uint32_t green = rgb_color(0, 255, 0, brightness);
    uint32_t blue = rgb_color(0, 0, 255, brightness);
    uint32_t indigo = rgb_color(75, 0, 130, brightness);
    uint32_t violet = rgb_color(138, 43, 226, brightness);

    uint32_t rainbow[7] = { red, orange, yellow, green, blue, indigo, violet };

    // Get the current color of the first column
    int current = i % 7;

    for (row = 0; row < rows; row++)
    {
        for (col = 0; col < cols; col++)
        {
            printf("Remapping: %d, %d, %d, Color: %d\n", remapping[row][col], row, col, rainbow[current]);
            led_strip[remapping[row][col]] = rainbow[current];
        }

        current = (current + 1) % 7;
    }
}

static inline int line1(int x, int ps, int rows)
{
    return (int)(rows - (3 * sin((1.0/5.0) * x + ps) + 13));
}

static inline int line2(int x, int ps, int rows)
{
    return (int)(rows - (3 * sin((1.0/8.0) * x + ps) + 10));
}

static inline int line3(int x, int ps, int rows)
{
    return (int)(rows - (3 * sin((1.0/7.0) * x + ps) + 7));
}

static inline int line4(int x, int ps, int rows)
{
    return (int)(rows - (3 * sin((1.0/4.0) * x + ps) + 5));
}

static inline int line5(int x, int ps, int rows)
{
    return (int)(rows - (3 * sin((1.0/10.0) * x + ps) + 3));
}

void aurora(uint32_t *led_strip, uint32_t** remapping, int i, float brightness, int rows, int cols)
{
    int row = 0;
    int col = 0;
    uint32_t color = 0;
    uint32_t background_blue_dark = rgb_color(40, 30, 90, brightness);
    uint32_t majestic_purple      = rgb_color(90, 70, 230, brightness);
    uint32_t pink                 = rgb_color(255, 20, 160, brightness);
    uint32_t aurora_green         = rgb_color(50, 240, 170, brightness);
    uint32_t bright_green         = rgb_color(130, 230, 130, brightness);

    for (row = 0; row < rows; row++)
    {
        for (col = 0; col < cols; col++)
        {
            int phase_shift = i / 4;
            // Calculate the sin line values
            int l1 = line1(col, phase_shift, rows);
            int l2 = line2(col, phase_shift, rows);
            int l3 = line3(col, phase_shift, rows);
            int l4 = line4(col, phase_shift, rows);
            int l5 = line5(col, phase_shift, rows);

            printf("l1: %d, l2: %d, l3: %d, l4: %d, l5: %d\n", l1, l2, l3, l4, l5);
    
            if (row <= l1 | \
                row >= l5)
            {
                printf("Background Blue\n");
                color = background_blue_dark;
            }
    
            else if (row >= l1 & \ 
                     row <= l2)
            {
                printf("Aurora Green\n");
                color = aurora_green;
            }
    
            else if (row >= l2 & \ 
                     row <= l3)
            {
                printf("Pink\n");
                color = pink;
            }
    
            else if (row >= l3 & \
                     row <= l4)
            {
                printf("Bright Green\n");
                color = bright_green;
            }
    
            else if (row >= l4 & \
                     row <= l5)
            {
                printf("Majestic Purple\n");
                color = majestic_purple;
            }
        
            led_strip[remapping[row][col]] = color;
        }
    }
}


static inline int left_atrium(int x, int expansion)
{
    // Quadratic curve in upper left
    return (1.0/2.0)*(x-5)*(x-7) + 5 - expansion;
}

static inline int right_atrium(int x, int expansion)
{
    // Quadratic curve in upper right
    return (1.0/2.0)*(x-11)*(x-9) + 5 - expansion;
}

static inline int left_ventricle(int x, int expansion)
{
    // Line on left
    return (-3.0/2.0) * x + 24 + expansion;
}

static inline int right_ventricle(int x, int expansion)
{
    // Line on right
    return (3.0/2.0) * x + expansion;
}

void beating_heart(uint32_t *led_strip, uint32_t** remapping, int i, float brightness, int rows, int cols)
{
    int row = 0;
    int col = 0;
    uint32_t red = rgb_color(255, 0, 0, brightness);
    uint32_t baby_blue = rgb_color(137, 207, 240, brightness);

    // Whether the current phase will animate the heart expanding
    int beating_phase = !((i/4) % 2);
    // The heart will expand every other frame by 1 pixel if it is in beating phase
    int expansion = (beating_phase & i % 2);

    int la, ra, lv, rv;
    for (row = 0; row < rows; row++)
    {
        for (col = 0; col < cols; col++)
        {
            la = left_atrium(col, expansion);
            ra = right_atrium(col, expansion);
            lv = left_ventricle(col, expansion);
            rv = right_ventricle(col, expansion);

            if ((row >= la | \
                 row >= ra) & \
                (row <= lv & \
                 row <= rv))
            {
                led_strip[remapping[row][col]] = red;
            }
            else
            {
                led_strip[remapping[row][col]] = baby_blue;
            }
        }
    }
}

void chessboard(uint32_t *led_strip, uint32_t** remapping, int i, float brightness, int rows, int cols)
{
    int row = 0;
    int col = 0;
    uint32_t forest_emerald = rgb_color(0, 220, 30, brightness);
    uint32_t crimson_velvet = rgb_color(120, 0, 20, brightness);

    uint32_t color = 0;
    int choose = i % 2;

    for (row = 0; row < rows; row++)
    {
        for (col = 0; col < cols; col++)
        {
            if (!((int)(row / 2) % 2 == (int)(col / 2) % 2))
            {
                if (choose)
                {
                    color = forest_emerald;
                }
                else
                {
                    color = crimson_velvet;
                }
            }
            else 
            {
                if (choose)
                {
                    color = crimson_velvet;
                }
                else 
                {
                    color = forest_emerald;
                }
            }

            led_strip[remapping[row][col]] = color;
        }
    }
}

void old_glory(uint32_t *led_strip, uint32_t** remapping, int i, float brightness, int rows, int cols)
{
    int row = 0;
    int col = 0;
    uint32_t red = rgb_color(255, 0, 10, brightness);
    uint32_t white = rgb_color(255, 255, 255, brightness);
    uint32_t blue = rgb_color(0, 0, 255, brightness);

    uint32_t color = 0;

    for (row = 0; row < rows; row++)
    {
        for (col = 0; col < cols; col++)
        {
            if (row < 7 & col > cols - 8) // Left corner, blue with white stars
            {
                if (row % 2 == col % 2) color = white; 
                else color = blue;
            }
            else 
            {
                if (row % 2 == 0) color = red;
                else color = white;
            }  
            
            led_strip[remapping[row][col]] = color;
        }
    }
}
