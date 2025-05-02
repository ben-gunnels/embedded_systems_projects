#ifndef BLINK_H
#define BLINK_H

#include "pico/stdlib.h"
#include "hardware/timer.h"

int pico_led_init(void);

void pico_set_led(bool led_on);
void blink();
void sleep();
#endif