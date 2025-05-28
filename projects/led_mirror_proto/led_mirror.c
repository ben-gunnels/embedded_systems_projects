/*
*   Logic for interfacing with a button through the GPIO.
*   The program sets the pin and polls it for a voltage drop.
*   When the button is pressed, it pull the current to ground, draining the pin.
*   This signal is used to trigger a new mode.
*   The mode signals the LED how many times to blink. 
*
*
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "pico/stdlib.h"

#define LED_PIN 25
#define GPIO_PIN 4

void 
set_mode(int * mode)
{
    *mode = (*mode < 4) ? *mode + 1 : 0;
}

int main() {
    stdio_init_all();
    sleep_ms(100);
    gpio_init(LED_PIN);
    gpio_init(GPIO_PIN);

    int mode = 0;

    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_set_dir(GPIO_PIN, GPIO_IN);

    gpio_pull_up(GPIO_PIN);
    while (1)
    {  
        if (!gpio_get(GPIO_PIN))
        {
            sleep_ms(100);
            if (!gpio_get(GPIO_PIN))
            {
                set_mode(&mode);
                for (int i = 0; i < mode; i++)
                {
                    gpio_put(LED_PIN, 1);
                    sleep_ms(200);
                    gpio_put(LED_PIN, 0);
                    sleep_ms(200);
                }
            }
        }
        else 
        {
            printf("GPIO PIN HIGH\n");
            gpio_put(LED_PIN, 0);
        }
    }
    return (0);
}