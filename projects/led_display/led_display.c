/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "pico/stdlib.h"
#include "hardware/timer.h"
#include "led_remapping.h"
#include "ws2812.h"


// Pico W devices use a GPIO on the WIFI chip for the LED,
// so when building for Pico W, CYW43_WL_GPIO_LED_PIN will be defined
#ifdef CYW43_WL_GPIO_LED_PIN
#include "pico/cyw43_arch.h"
#endif

#ifndef LED_DELAY_MS
#define LED_DELAY_MS 300
#endif

#define ROWS 16
#define COLS 16
#define NUM_LEDS 256
#define NUM_MODES 5

// Perform initialisation
int pico_led_init(void) {
#if defined(PICO_DEFAULT_LED_PIN)
    // A device like Pico that uses a GPIO for the LED will define PICO_DEFAULT_LED_PIN
    // so we can use normal GPIO functionality to turn the led on and off
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    return PICO_OK;
#elif defined(CYW43_WL_GPIO_LED_PIN)
    // For Pico W devices we need to initialise the driver etc
    return cyw43_arch_init();
#endif
}

// Turn the led on or off
void pico_set_led(bool led_on) {
#if defined(PICO_DEFAULT_LED_PIN)
    // Just set the GPIO on or off
    gpio_put(PICO_DEFAULT_LED_PIN, led_on);
#elif defined(CYW43_WL_GPIO_LED_PIN)
    // Ask the wifi "driver" to set the GPIO on or off
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, led_on);
#endif
}

void blink() 
{
    pico_set_led(true);
    sleep_ms(LED_DELAY_MS);
    pico_set_led(false);
    sleep_ms(LED_DELAY_MS);
}

void sleep()
{
    pico_set_led(false);
    sleep_ms(LED_DELAY_MS); 
}

static void set_mode(int * mode)
{
    // Circle around when the mode is increased
    *mode = (*mode < NUM_MODES) ? *mode + 1 : 1;
}

void poll_mode(int * mode)
{
    if (!gpio_get(BUTTON))
    {
        sleep_ms(100);
        if (!gpio_get(BUTTON))
        {
            set_mode(mode);
        }
    }
}

int main() {
    // Initialize the hardware
    stdio_init_all();
    gpio_init(BUTTON);
    gpio_set_dir(BUTTON, GPIO_IN);
    gpio_pull_up(BUTTON);
    int rc = pico_led_init();
    hard_assert(rc == PICO_OK);
    // Time to breathe
    sleep_ms(1000);
    
    int i = 0;
    int mode = 1;
    // Create a one dimensional array to map the colors onto the LED strip
    uint32_t led_strip[ROWS*COLS] = {0};
    // Create a two dimensional array for convenience mapping effects
    uint32_t** remapping = remap(led_strip, ROWS, COLS);

    // Initialize the hardware
    ws2812_init(&ws2812_config);
    
    // Timer variables
    uint64_t last_time = time_us_64();
    uint64_t frame_delay_us = 300000; // 300 milliseconds in microseconds
    // Prepare the LED. Without this the first pixel flashes green for some reason. 
    clear_colors(&ws2812_config, NUM_LEDS);

    while (1) 
    {
        // Continuously poll for mode changes
        poll_mode(&mode);
        
        // Check if enough time has passed for the next frame
        uint64_t current_time = time_us_64();
        if (current_time - last_time >= frame_delay_us) 
        {
            // Reset the timer
            last_time = current_time;

            // Reset i when it reaches 720
            if (i == 720) {
                i = 0;
            }

            // Execute the selected mode animation
            switch (mode)
            {
                case 1:
                    rainbow(led_strip, remapping, i, 0.01, ROWS, COLS);
                    break;
                case 2:
                    aurora(led_strip, remapping, i, 0.015, ROWS, COLS);
                    break;
                case 3:
                    beating_heart(led_strip, remapping, i, 0.01, ROWS, COLS);
                    break;
                case 4:
                    chessboard(led_strip, remapping, i, 0.01, ROWS, COLS);
                    break;
                case 5:
                    old_glory(led_strip, remapping, i, 0.01, ROWS, COLS);
                    break;
                default:
                    break;
            }

            // Send the colors to the WS2812 LEDs
            send_colors(&ws2812_config, led_strip, NUM_LEDS);

            // Increment the frame index
            i++;
        }
    }
}
