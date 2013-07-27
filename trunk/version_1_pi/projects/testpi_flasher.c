###############################################
# Name: testpi_flasher.c
# Version: 0.1
# Company: InGuardians, Inc.
# Start Date: July 27, 2013
#
# Purpose:
#
#  This code is designed to turn on and off specific
#  Raspberry Pi GPIO pins as sent by a client. The serial
#  and GPIO setup components have been split out into 
#  header files.
#
# Developers: 
#   Cutaway (Don C. Weber)
#     The GPIO code is based on the example from Dom and Gert
#     on the Raspberry Pi website.
#
# Notes:
#  Compile on the Raspberry Pi using the following command:
#    gcc testpi_flasher.c -o testpi_flasher
#  If you are using a Raspberry Pi version one you may want to 
#  change the #define below or compile using the following:
#    gcc -DRPv1 testpi_flasher.c -o testpi_flasher
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
#ifndef RPv1
#define RPv2
#endif

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#include <stdint.h>

#include "include/pi.h"
#include "include/pi_serial.h"

int main(int argc, char **argv)
{
    short err = 0;
    short cnt = 0;
    short delay = 1;
    int qsec = 250000;
    #ifdef RPv1
    short pins[] = {4,17,18,21,22,23,24,25};    // Raspberry Pi v1 GPIO Pins
    #else
    short pins[] = {4,17,18,27,22,23,24,25};    // Raspberry Pi v2 GPIO Pins
    #endif
    short psize = sizeof(pins) / sizeof(short);
    uint8_t cmd = 0;

    // Set up serial
    printf("Setting serial.\n");
    err = pi_ser_init();
    if (err < 0){
        printf("Setting serial failed.\n");
        return -1;
    }

    // Set up gpi pointer for direct register access
    setup_io();
    printf("Serial setup successful.\n");

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

    // Turn OFF all LEDs
    printf("Setting GPIO pins to HIGH.\n");
    for (cnt=0;cnt<psize;cnt++) {
        GPIO_SET = 1<<pins[cnt];
        //GPIO_CLR = 1<<pins[cnt];
    }

    // Pause for effect
    usleep(qsec);

    // Roll through LEDs to show they work
    for (cnt=0;cnt<psize;cnt++) {
        printf("Setting GPIO pin %d to HIGH (CLR).\n",pins[cnt]);
        //GPIO_CLR = 1<<pins[cnt];
        //GPIO_LVL & (1<<pins[cnt]) ? (GPIO_CLR = 1<<pins[cnt]): (GPIO_SET = 1<<pins[cnt]);
        GPIO_TGL(pins[cnt])
        usleep(qsec);
        printf("Level: %s\n",(GPIO_LVL & (1<<pins[cnt]) ? "Low":"High"));
        printf("Setting GPIO pins %d to LOW (SET).\n",pins[cnt]);
        //GPIO_SET = 1<<pins[cnt];
        //GPIO_LVL & (1<<pins[cnt]) ? (GPIO_CLR = 1<<pins[cnt]): (GPIO_SET = 1<<pins[cnt]);
        GPIO_TGL(pins[cnt])
        usleep(qsec);
        printf("Level: %s\n",(GPIO_LVL & (1<<pins[cnt]) ? "Low":"High"));
    }
    printf("GPIO setup complete.\n");

    // Grab incoming
    //printf("incoming: 0x%x\n",serial_rx());

    printf("Start Main Loop.\n");
    // Grap first incoming command
    cmd = serial_rx();

    while (cmd != DONE){
        printf("incoming: 0x%x\n",cmd);
        switch (cmd){
            case ENTER :  // 0x0a //enter
                //Handle enter key which is used to flush incoming serial
                break;
            case FLASH4 :  // 0x61 //a
                printf("flashing: %d\n",PIPIN0);
                GPIO_CLR = 1<<PIPIN0;
                sleep(delay);
                GPIO_SET = 1<<PIPIN0;
                break;
            case FLASH17 : // 0x62 //b
                printf("flashing: %d\n",PIPIN1);
                GPIO_CLR = 1<<PIPIN1;
                sleep(delay);
                GPIO_SET = 1<<PIPIN1;
                break;
            case FLASH18 : // 0x63 //c
                printf("flashing: %d\n",PIPIN2);
                GPIO_CLR = 1<<PIPIN2;
                sleep(delay);
                GPIO_SET = 1<<PIPIN2;
                break;
            case FLASH27 : // 0x64 //d
                printf("flashing: %d\n",PIPIN3);
                GPIO_CLR = 1<<PIPIN3;
                sleep(delay);
                GPIO_SET = 1<<PIPIN3;
                break;
            case FLASH22 : // 0x65 //e
                printf("flashing: %d\n",PIPIN4);
                GPIO_CLR = 1<<PIPIN4;
                sleep(delay);
                GPIO_SET = 1<<PIPIN4;
                break;
            case FLASH23 : // 0x66 //f
                printf("flashing: %d\n",PIPIN5);
                GPIO_CLR = 1<<PIPIN5;
                sleep(delay);
                GPIO_SET = 1<<PIPIN5;
                break;
            case FLASH24 : // 0x67 //g
                printf("flashing: %d\n",PIPIN6);
                GPIO_CLR = 1<<PIPIN6;
                sleep(delay);
                GPIO_SET = 1<<PIPIN6;
                break;
            case FLASH25 : // 0x68 //h
                printf("flashing: %d\n",PIPIN7);
                GPIO_CLR = 1<<PIPIN7;
                sleep(delay);
                GPIO_SET = 1<<PIPIN7;
                break;
            default:
                printf("CMD not implemented.\n");
        }

        // Grap incoming command
        cmd = serial_rx();
    }

    printf("Done.\n");
    return 0;

} // main

