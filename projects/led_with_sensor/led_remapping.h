#ifndef _LED_REMAPPING_H
#define _LED_REMAPPING_H

#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"


#define LED_PIN     6
#define NUM_LEDS    256
#define BRIGHTNESS  64
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB

uint32_t** remap(uint32_t* led_strip, int rows, int cols);
void free_remap(uint32_t** led_remap, int rows);

#endif /* _LED_REMAPPING_ */