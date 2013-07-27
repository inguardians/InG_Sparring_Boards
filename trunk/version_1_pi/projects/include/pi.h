/*! \file pi.h
  \author Travis Goodspeed
  \modded Don C. Weber (cutaway)
  \brief Port descriptions for the PI platform. Combination of platform.h, config.h, and other configurations.
*/

#ifndef __pi
#define __pi

#ifdef __cplusplus
extern "C" {
#endif

/************************************************************
* STANDARD BITS - taken from msp430f2274.h
************************************************************/

#define BIT0                (0x0001)
#define BIT1                (0x0002)
#define BIT2                (0x0004)
#define BIT3                (0x0008)
#define BIT4                (0x0010)
#define BIT5                (0x0020)
#define BIT6                (0x0040)
#define BIT7                (0x0080)
#define BIT8                (0x0100)
#define BIT9                (0x0200)
#define BITA                (0x0400)
#define BITB                (0x0800)
#define BITC                (0x1000)
#define BITD                (0x2000)
#define BITE                (0x4000)
#define BITF                (0x8000)


/************************************************************
* SERIAL SETUP - taken from http://www.raspberry-projects.com/pi/programming-in-c/uart-serial-port/using-the-uart
************************************************************/

// Define Raspberry Pi version - default to RPv2
#ifndef RPv1
    #ifndef RPv2
    #define RPv2
    #endif
#endif

/************************************************************
* COMMANDS - temp for testing. These should be same as command.h
************************************************************/

// Global Commands
#define FLASH4  0x61 //a
#define FLASH17 0x62 //b
#define FLASH18 0x63 //c
#define FLASH27 0x64 //d
#define FLASH22 0x65 //e
#define FLASH23 0x66 //f
#define FLASH24 0x67 //g
#define FLASH25 0x68 //h
#define DONE    0x20 //space
#define ENTER   0x0a //enter

/************************************************************
* PINOUT DEFINITIONS
************************************************************/
// Access from ARM Running Linux

#define BCM2708_PERI_BASE        0x20000000
#define GPIO_BASE                (BCM2708_PERI_BASE + 0x200000) /* GPIO controller */

#define PAGE_SIZE (4*1024)
#define BLOCK_SIZE (4*1024)

// GPIO setup macros. Always use INP_GPIO(x) before using OUT_GPIO(x) or SET_GPIO_ALT(x,y)
#define GPIO_SET *(gpio+7)  // sets   bits which are 1 ignores bits which are 0
#define GPIO_CLR *(gpio+10) // clears bits which are 1 ignores bits which are 0
#define GPIO_LVL *(gpio+13) // clears bits which are 1 ignores bits which are 0

#define INP_GPIO(g) *(gpio+((g)/10)) &= ~(7<<(((g)%10)*3))
#define OUT_GPIO(g) *(gpio+((g)/10)) |=  (1<<(((g)%10)*3))
#define SET_GPIO_ALT(g,a) *(gpio+(((g)/10))) |= (((a)<=3?(a)+4:(a)==4?3:2)<<(((g)%10)*3))
#define GPIO_TGL(g) GPIO_LVL & (1<<g) ? (GPIO_CLR = 1<<g): (GPIO_SET = 1<<g);

// I/O access
int mem_fd;
void *gpio_map;
volatile unsigned *gpio;

void setup_io();

#define PIPIN0 4  //GPIO4
#define PIPIN1 17  //GPIO17
#define PIPIN2 18  //GPIO18
#ifdef RPv1
#define PIPIN3 21  //GPIO21
#else
#define PIPIN3 27  //GPIO27
#endif
#define PIPIN4 22  //GPIO22
#define PIPIN5 23  //GPIO23
#define PIPIN6 24  //GPIO24
#define PIPIN7 25  //GPIO25
#define PIPIN15 15  //UART TXD
#define PIPIN16 16  //UART RXD

//LED on P1.0
//#define PLEDOUT P1OUT
#define PLEDOUT P1OUT
#define PLEDDIR P1DIR
#define PLEDPIN BIT0

#define SPIOUT P5OUT
#define SPIDIR P5DIR
#define SPIIN  P5IN
#define SPIREN P5REN

/************************************************************
* End of Modules
************************************************************/
#ifdef __cplusplus
}
#endif /* extern "C" */

#endif/* __pi */
