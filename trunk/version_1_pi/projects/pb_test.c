###############################################
# Name: pb_test.c
# Version: 0.1
# Company: InGuardians, Inc.
# Start Date: July 27, 2013
#
# Purpose:
#
#  This code is designed to test serial communications from a computer's
#  serial (USB) port to the Raspberry Pi and this programming running
#  on the Pi. It leverages serial receive and transmit functions stripped
#  from the Goodfet project.
#
# Notes:
#  Compile on the Raspberry Pi using the following command:
#    gcc pb_test.c -o pb_test
#
# Developers: 
#   Cutaway (Don C. Weber)
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

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
     
#include <sys/types.h>
#include <sys/stat.h>

#include <stdint.h>

#define SERIAL_PORT "/dev/ttyAMA0"

//Raspberry Pi to Arduino UART
//Takes one argument and sends it over the UART as a string
//Uart /dev/ttyAMA0 is setup and settings are not restored.
int ser_fd;

int setup_serial(){
    // Open the Port. We want read/write, no "controlling tty" status, and open it no matter what state DCD is in
    //fd = open("/dev/ttyAMA0", O_WRONLY | O_NOCTTY | O_NDELAY | O_NONBLOCK);
    //fd = open(SERIAL_PORT, O_WRONLY | O_NOCTTY | O_NDELAY | O_NONBLOCK);
    ser_fd = open(SERIAL_PORT, O_RDWR | O_NOCTTY | O_NDELAY | O_NONBLOCK);
    //fd = open(SERIAL_PORT, O_RDWR | O_NOCTTY | O_NDELAY);
    if (ser_fd == -1) {
        //perror("open_port: Unable to open /dev/ttyAMA0 - ");
        perror("open_port: Unable to open serial port - ");
        return(-1);
    }
     
    //Set the baud rate
    struct termios options;
    tcgetattr(ser_fd, &options);
    cfsetispeed(&options, B9600);
    cfsetospeed(&options, B9600);
    //Set flow control and all that
    options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;
    options.c_iflag = IGNPAR | ICRNL;
    options.c_oflag = 0;
    //Flush line and set options
    tcflush(ser_fd, TCIFLUSH);
    tcsetattr(ser_fd, TCSANOW, &options);
     
    // Turn off blocking for reads, use (fd, F_SETFL, FNDELAY) if you want that
    fcntl(ser_fd, F_SETFL, 0);
    //fcntl(fd, F_SETFL, FNDELAY);

    return 0;
}

#define serial_rx sergetc
#define serial_tx serputc
#define serial0_rx serial_rx
#define serial1_rx serial_rx
#define serial0_tx serial_tx
#define serial1_tx serial_tx

void serputc(uint8_t c, int fd){
    int err = write(fd, &c, sizeof(uint8_t));

    // Handle any errors
    if (err < 0) {
        perror("serial write error");
    }
}

int sergetc(int fd){
    uint8_t c;
    int err = read(fd,&c,sizeof(uint8_t));

    // Handle any errors
    if (err < 0){
        perror("serial read error");
    }
    return c;
}

int main(int argc, char ** argv) {
    int error;
    int cnt = 0;
    int ind;
    char* teststr = "cutaway\n\0";
     
    error = setup_serial();
    if (error < 0){
        printf("serial setup error\n");
        return -1;
    }

    while (cnt < strlen(teststr)){
        //serputc(teststr[cnt],ser_fd);
        serial_tx(teststr[cnt],ser_fd);
        cnt++;
    }
    
    //ind = sergetc(ser_fd);
    ind = serial_rx(ser_fd);
    printf("sergetc\n");
    printf("decimal incoming: %d\n",ind);
    printf("char incoming: %c\n",(char)ind);
    
    // Don't forget to clean up
    close(ser_fd);
    return 0;
}
//END OF FILE
