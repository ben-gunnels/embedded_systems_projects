#include <stdio.h>
#include <stdlib.h>

#define LED_PIN     6
#define NUM_LEDS    256
#define BRIGHTNESS  64
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB

/*
    Remaps the single-dimension LEDs array to a 2D remapped array.
    Provides an abstraction allowing the programmer to use a 2-dimensional mapping for simplicity.
    It counts the current LED using the counter. If the row is even, it maps the row and column to that LED directly.
    If the row is odd, it maps the LEDs backward for that row.
    This accounts for the LED board's serpentine pattern, whereby alternating rows are connected left-to-right, then right-to-left. 

    returns: A dynamically allocated 2D array of CRGB pointers (CRGB**)
*/
// Function to create a 2D snake-pattern LED mapping
int** remap(int rows, int cols) {
    // Allocate the 2D array dynamically
    int** led_remap = (int**)malloc(rows * sizeof(int*));
    for (int i = 0; i < rows; i++) {
        led_remap[i] = (int*)malloc(cols * sizeof(int));
    }

    int counter = 0; // Counts the current LED in question serially

    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            if (row % 2 == 0) { // Even rows - left to right
                led_remap[row][col] = counter;
            } else {            // Odd rows - right to left (snake pattern)
                led_remap[row][cols - 1 - col] = counter;
            }
            counter++;
        }
    }

    return led_remap;
}

// Function to free the dynamically allocated 2D array
void free_remap(int** led_remap, int rows) {
    for (int i = 0; i < rows; i++) {
        free(led_remap[i]);
    }
    free(led_remap);
}