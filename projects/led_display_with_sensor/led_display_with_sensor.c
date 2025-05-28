#include "bmp280_i2c.h"
#include "led_remapping.h"
#include "ws2812.h"

#ifndef PICO_I2C_SDA_PIN 
#define PICO_I2C_SDA_PIN 4
#endif

#ifndef PICO_I2C_SCL_PIN
#define PICO_I2C_SCL_PIN 5
#endif

#define ROWS 16
#define COLS 16
#define NUM_LEDS 256

int main() {
    stdio_init_all();
    // useful information for picotool
    // bi_decl(bi_2pins_with_func(PICO_DEFAULT_I2C_SDA_PIN, PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C));
    // bi_decl(bi_program_description("BMP280 I2C example for the Raspberry Pi Pico"));

    // printf("Hello, BMP280! Reading temperaure and pressure values from sensor...\n");

    // I2C is "open drain", pull ups to keep signal high when no data is being sent
    // i2c_init(i2c_default, 100 * 1000);
    // gpio_set_function(PICO_I2C_SDA_PIN, GPIO_FUNC_I2C);
    // gpio_set_function(PICO_I2C_SCL_PIN, GPIO_FUNC_I2C);
    // gpio_pull_up(PICO_I2C_SDA_PIN);
    // gpio_pull_up(PICO_I2C_SCL_PIN);
    sleep_ms(1000); // sleep so that data polling and register update don't collide

    // configure BMP280
    // bmp280_init();

    // retrieve fixed compensation params
    // struct bmp280_calib_param params;
    // bmp280_get_calib_params(&params);

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
            
            for (int i = 0; i < NUM_LEDS; i++)
            {
                led_strip[i] = rgb_color(155, 155, 155, 0.02);
            }
            
            // Send the colors to the WS2812 LEDs
            send_colors(&ws2812_config, led_strip, NUM_LEDS);

            // Increment the frame index
            i++;
        }
    }

    // int32_t raw_temperature;
    // int32_t raw_pressure;

    // while (1) {
    //     // bmp280_read_raw(&raw_temperature, &raw_pressure);
    //     // int32_t temperature = bmp280_convert_temp(raw_temperature, &params);
    //     // int32_t pressure = bmp280_convert_pressure(raw_pressure, raw_temperature, &params);
    //     // printf("Reading data...\n");
    //     // printf("Pressure = %.3f kPa\n", pressure / 1000.f);
    //     // printf("Temp. = %.2f C\n", temperature / 100.f);
    //     // poll every 500ms
    //     // Send the colors to the WS2812 LEDs
    //     // Send the data to create a lighting effect on the LED
    //     // sensor_wave(led_strip, remapping, i, 0.02, ROWS, COLS, (uint32_t) 40, (uint32_t) 40);
    //     for (int i = 0; i < NUM_LEDS; i++)
    //     {
    //         led_strip[i] = rgb_color(155, 155, 155, 0.02);
    //     }
    //     send_colors(&ws2812_config, led_strip, NUM_LEDS);
    //     sleep_ms(200);
    // }
}