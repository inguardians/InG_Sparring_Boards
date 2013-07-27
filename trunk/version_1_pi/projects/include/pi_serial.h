#ifndef _pi_serial_h
#define _pi_serial_h
/* Name: pi_serial.h
 * Modded by: Don C. Weber (cutaway)
 * serput and sergetc functionality as in msp430_serial.c
 */

#include <stdint.h>

//Stubs for goodfet serial calls
#define serial_rx sergetc
#define serial_tx serputc
#define serial0_rx serial_rx
#define serial1_rx serial_rx
#define serial0_tx serial_tx
#define serial1_tx serial_tx

//Globals
int fd;                 //Serial file descriptor

//Functions
//int pi_ser_init(int fd);
int pi_ser_init();
//void serputc(uint8_t c, int fd);
void serputc(uint8_t c);
//int sergetc(int fd);
int sergetc();

#endif
