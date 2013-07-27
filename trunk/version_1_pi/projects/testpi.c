###############################################
# Name: testpi.c
# Version: 0.1
# Company: InGuardians, Inc.
# Start Date: July 27, 2013
#
# Purpose:
#
#  This code is designed to turn on and off all of the
#  Raspberry Pi GPIO pins.
#
# Developers: 
#   Cutaway (Don C. Weber)
#     The GPIO code is based on the example from Dom and Gert
#     on the Raspberry Pi website.
#
# Notes:
#  Compile on the Raspberry Pi using the following command:
#    gcc testpi.c -o testpi
#  If you are using a Raspberry Pi version one you may want to 
#  change the #define below or compile using the following:
#    gcc -DRPv1 testpi.c -o testpi
#
# Resources:
#   http://goodfet.sourceforge.net/
#   http://elinux.org/RPi_Low-level_peripherals
#
# TODO: 
#
# Change Log:
#
############################################

// Define Raspberry Pi version
//#define RPv1
#define RPv2

// Access from ARM Running Linux

#define BCM2708_PERI_BASE        0x20000000
#define GPIO_BASE                (BCM2708_PERI_BASE + 0x200000) /* GPIO controller */


#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#define PAGE_SIZE (4*1024)
#define BLOCK_SIZE (4*1024)

int  mem_fd;
void *gpio_map;

// I/O access
volatile unsigned *gpio;

// GPIO setup macros. Always use INP_GPIO(x) before using OUT_GPIO(x) or SET_GPIO_ALT(x,y)
#define INP_GPIO(g) *(gpio+((g)/10)) &= ~(7<<(((g)%10)*3))
#define OUT_GPIO(g) *(gpio+((g)/10)) |=  (1<<(((g)%10)*3))
#define SET_GPIO_ALT(g,a) *(gpio+(((g)/10))) |= (((a)<=3?(a)+4:(a)==4?3:2)<<(((g)%10)*3))

#define GPIO_SET *(gpio+7)  // sets   bits which are 1 ignores bits which are 0
#define GPIO_CLR *(gpio+10) // clears bits which are 1 ignores bits which are 0

void setup_io();

int main(int argc, char **argv)
{
    short iter = 0;
    short miter = 5;
    short cnt = 0;
    short delay = 1;
    #ifdef RPv1
    short pins[] = {4,17,18,21,22,23,24,25};    // Raspberry Pi v1 GPIO Pins
    #else
    short pins[] = {4,17,18,27,22,23,24,25};    // Raspberry Pi v2 GPIO Pins
    #endif
    short psize = sizeof(pins) / sizeof(short);

    // Set up gpi pointer for direct register access
    setup_io();

    /************************************************************************\
     * You are about to change the GPIO settings of your computer.          *
     * Mess this up and it will stop working!                               *
     * It might be a good idea to 'sync' before running this program        *
     * so at least you still have your code changes written to the SD-card! *
    \************************************************************************/
    // Set GPIO pins to output
    printf("Setting GPIO pins for output.\n");
    for (cnt=0;cnt<psize;cnt++) {
        INP_GPIO(pins[cnt]); // must use INP_GPIO before we can use OUT_GPIO
        OUT_GPIO(pins[cnt]);
    }

    // Turn off all LEDs
    printf("Setting GPIO pins to LOW.\n");
    for (cnt=0;cnt<psize;cnt++) {
        GPIO_SET = 1<<pins[cnt];
        //GPIO_CLR = 1<<pins[cnt];
    }

    // Pause for effect
    sleep(delay);

    while (iter < miter){
        for (cnt=0;cnt<psize;cnt++) {
            printf("Setting GPIO pin %d to HIGH (CLR).\n",pins[cnt]);
            GPIO_CLR = 1<<pins[cnt];
            sleep(delay);
            printf("Setting GPIO pins %d to LOW (SET).\n",pins[cnt]);
            GPIO_SET = 1<<pins[cnt];
            sleep(delay);
        }
        iter++;
    }

    return 0;

} // main

//
// Set up a memory regions to access GPIO
//
void setup_io()
{
     /* open /dev/mem */
     if ((mem_fd = open("/dev/mem", O_RDWR|O_SYNC) ) < 0) {
          printf("can't open /dev/mem \n");
          exit(-1);
     }

   /* mmap GPIO */
     gpio_map = mmap(
          NULL,             //Any adddress in our space will do
          BLOCK_SIZE,       //Map length
          PROT_READ|PROT_WRITE,// Enable reading & writting to mapped memory
          MAP_SHARED,       //Shared with other processes
          mem_fd,           //File to map
          GPIO_BASE         //Offset to GPIO peripheral
     );

     close(mem_fd); //No need to keep mem_fd open after mmap

     if (gpio_map == MAP_FAILED) {
          printf("mmap error %d\n", (int)gpio_map);//errno also set!
          exit(-1);
     }

     // Always use volatile pointer!
     gpio = (volatile unsigned *)gpio_map;

} // setup_io
