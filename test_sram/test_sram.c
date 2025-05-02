#include <stdio.h>
#include "blink.h"

// SRAM starts at the following address.
// https://datasheets.raspberrypi.com/rp2040/rp2040-datasheet.pdf
// This program is run to test the first n words of the address space.
#define BASE_ADDRESS ((volatile int *) 0x20000000)
#define NUM_BYTES 0x40740 //  ~ 264KB

int * 
memTestDevice(volatile int * baseAddress, unsigned long nBytes)
{
    unsigned long offset;
    unsigned long numWords = nBytes / sizeof(int);

    int pattern;
    int antipattern;

    /*
    * Fill each memory location in the range with a known pattern.
    * Increment the pattern each time the address is changed. 
    */
   for (pattern = 1, offset = 0; offset < numWords; pattern++, offset++)
   {
        baseAddress[offset] = pattern;
   }

   /*
   * Check that the location has the desired pattern and then invert it
   */

   for (pattern = 1, offset = 0; offset < numWords; pattern++, offset++)
   {
        if (baseAddress[offset] != pattern)
        {
            return ((int *) &baseAddress[offset]);
        }

        antipattern = ~pattern;
        baseAddress[offset] = antipattern;
   }


   /*
   * Check each location for the inverted pattern and zero it out.
   */

   for (pattern = 1, offset = 0; offset < numWords; pattern++, offset++)
   {
        antipattern = ~pattern;
        if (baseAddress[offset] != antipattern)
        {
            return ((int *) &baseAddress[offset]);
        }

        baseAddress[offset] = 0;
   }

   return (NULL);
} /* memTestDevice() */

int
main(void)
{
    int success_count = 0;
    // Initialize peripherals
    stdio_init_all();
    pico_led_init();

    blink();
    blink();
    
    /*
        If some memory address resulted in an error, either a bit(s) was stuck high or low.
        The memory has failed in some way and the program has failed.
    */

    int * firstFailedAddress;

    firstFailedAddress = memTestDevice(BASE_ADDRESS, NUM_BYTES);
    if (firstFailedAddress != NULL)
    {   
        for (int i = 0; i < 3; i++) blink();
        // printf("Address of failed memory location: %p\n", (void*)&firstFailedAddress); // For debugging
        return (-1);
    }

    else
    {
        pico_set_led(true);
        while (1)
        {
            printf("Memory has passed the test");
        }
        return (0);
    }

} /* main() */