#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "blink.h"
#include "pico/stdlib.h"
#include "hardware/timer.h"

// https://datasheets.raspberrypi.com/rp2040/rp2040-datasheet.pdf
/*
*   This program simulates a game of two competing processes. 
*   The first process, the mole, assigns values to random addresses in memory.
*   The second process, the gavel, clears these values out in memory.
*   The game is stopped after a random duration, and the score of the mole is counted.
*   The mole's score is how many addresses still remain at the value assigned by the mole. 
*/

// SRAM starts at the following address.
#define NUM_BYTES 8192
static volatile int memory[NUM_BYTES / sizeof(int)];
#define BASE_ADDRESS memory
// Keeping track of the memory that has been cleaned by the gavel
static int cleaned_counter = 0;

// Holds the data needed to update the game from the pov of the gavel
struct gavel_data 
{
    volatile int *base_address;
    int *cleaned; // Holds the global score for the gavel
};

static struct gavel_data gavel_info = {
    .base_address = memory,
    .cleaned = &cleaned_counter
};

void setup_game() {
    gavel_info.base_address = memory;
    gavel_info.cleaned = &cleaned_counter;
}

int random_range(int min, int max)
{
    return rand() % (max - min + 1) + min;
}  /* random_range() */

/*
* The mole infects randomly generated addresses leaving its message.
*/
int64_t 
mole(alarm_id_t id, void *user_data)
{
    printf("Mole up to no good...\n");
    volatile int *base_address = (volatile int *)user_data;
    // Leaves messages on random memory addresses
    unsigned long message = 0xfeedbabe;

    int num_words = NUM_BYTES / sizeof(int);

    int target_address =  random_range(0, num_words); // Generate an offset in the range 0-NUM_BYTES to place infected data
    printf("Mole targeting %p\n", target_address);
    fflush(stdout);
    base_address[target_address] = message;

    return (10000);
}  /* mole() */

/*
* The gavel tries to iterate through the memory range in time to clear the infected memory 
* left by the mole.
* 
*/
int64_t 
gavel(alarm_id_t id, void *user_data) 
{
    printf("Gavel working...\n");
    fflush(stdout);
    unsigned long reset = 0;
    int num_words = NUM_BYTES / sizeof(int);
    fflush(stdout);
    struct gavel_data *info = (struct gavel_data *)user_data;
    volatile int *base_address = info->base_address;

    int target_address =  random_range(0, num_words); // Generate an offset in the range 0-NUM_BYTES to place infected data
    if (base_address[target_address]) {
        printf("Infected memory being cleaned!\n");
        fflush(stdout);
        base_address[target_address] = reset;
        *(info->cleaned) += 1;
    }
    
    fflush(stdout);
    return (1000);
} /* gavel() */

int 
scorekeeper(volatile int * base_address)
{
    unsigned long offset;
    int mole_score = 0;

    unsigned long numWords = NUM_BYTES / sizeof(int);

    for (offset=0; offset < numWords; offset++)
    {
        if (base_address[offset])
        {
            mole_score++;
        }
    }
    return (mole_score);
} /* scorekeeper() */

int64_t 
put_down_flag(alarm_id_t id, void *user_data)
{
    printf("Game ending...\n");
    volatile int * run_flag = (volatile int *)user_data;
    *run_flag = 0;
    return (0);
}

int main()
{
    alarm_id_t mole_id;
    alarm_id_t gavel_id;
    stdio_init_all();
    pico_led_init();

    while (!stdio_usb_connected())
    {
        sleep_ms(100);
    }

    sleep_ms(1000);

    blink();
    blink();
    printf("Initializing...\n");
    printf("Mole Game!\n");
    printf("From the Raspberry Pi Pico RP2040\n");
    fflush(stdout);


    srand(time(NULL));
    printf("Setting run_flag\n");
    volatile int run_flag = 1;
    printf("run_flag address set\n");
    fflush(stdout);
    
    printf("run_flag value: %p\n", (void *)&run_flag);
    fflush(stdout);

    int random_end = random_range(0, 10000);
    printf("Program will run for %i miliseconds\n", random_end);
    fflush(stdout);

    // Timer example code - This example fires off the callback after 2000ms
    mole_id = add_alarm_in_ms(10, mole, BASE_ADDRESS, true);
    gavel_id = add_alarm_in_ms(100, gavel, (void *)&gavel_info, true);
    add_alarm_in_ms(random_end, put_down_flag, &run_flag, true);

    while(run_flag) {
        tight_loop_contents(); // Run the game
        int score = scorekeeper(BASE_ADDRESS);
        printf("Score: %d\n", score);
        sleep_ms(500);
    }
    // After flag is down, kill both repeating alarms
    cancel_alarm(mole_id);
    cancel_alarm(gavel_id);

    int mole_score = scorekeeper(BASE_ADDRESS);

    while (1)
    {
        printf("The Mole scored: %i\n", mole_score);
        printf("The Gavel scored: %d\n", *gavel_info.cleaned);
        blink();
        sleep_ms(1000);
    }

} /* main() */
