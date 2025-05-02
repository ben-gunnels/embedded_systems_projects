#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "blink.h"

// https://datasheets.raspberrypi.com/rp2040/rp2040-datasheet.pdf
/*
*   This program simulates a game of two competing processes. 
*   The first process, the mole, assigns values to random addresses in memory.
*   The second process, the gavel, clears these values out in memory.
*   The game is stopped after a random duration, and the score of the mole is counted.
*   The mole's score is how many addresses still remain at the value assigned by the mole. 
*/

// SRAM starts at the following address.
#define BASE_ADDRESS ((volatile int *) 0x20000000)
#define NUM_BYTES 0x40740 //  ~ 264KB

void 
mole()
{

}


int main()
{
    
}

