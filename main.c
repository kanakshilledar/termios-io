#include <termios.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#define SERIAL_PORT "/dev/ttyUSB0"

int main() {
    int pd = open(SERIAL_PORT, O_RDWR);

    /* Opening serial port */
    if (pd < 0) {
        printf("Error %i: %s while opening %s\n", errno, strerror(errno), SERIAL_PORT);
        return 1;
    }

    /* Reading existing settings from serial port */
    struct termios tty;
    if (tcgetattr(pd, &tty) != 0) {
        printf("Error %i: %s from tcgetattr\n", errno, strerror(errno));
        return 1;
    }

    /* Setting Control Mode */
    tty.c_cflag &= ~PARENB;         // clear parity bit (mostly this is not used) (use |= PARENB to set)
    tty.c_cflag &= ~CSTOPB;         // clear stop field (only one is used)
    tty.c_cflag &= ~CSIZE;          // clear all size bits    
    tty.c_cflag |= CS8;             // 8 bits per byte (most common)
    // tty.c_cflag &= ~CRTSCTS;     // disable RTS/CTS (hardware) flow control (not for POSIX)
    tty.c_cflag |= CREAD | CLOCAL;  // turn on READ and ignore ctrl lines

    /* Setting Local Mode */
    tty.c_lflag &= ~ICANON;         // disable canonical mode
    tty.c_lflag &= ~ECHO;           // disable echo
    tty.c_lflag &= ~ECHOE;          // disable erasure echo
    tty.c_lflag &= ~ECHONL;         // disable newline echo
    tty.c_lflag &= ~ISIG;           // disable signal chars

    /* Setting Input Mode */
    tty.c_iflag &= ~(IXON | IXOFF | IXANY);     // turnoff s/w flow control
    tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL); // disable special handling of received bytes

    /* Setting Output Mode */
    tty.c_oflag &= ~OPOST;          // prevent output processing
    tty.c_oflag &= ~ONLCR;          // prevent conversion of NL to CRNL

    /* Setting VMIN and VTIME */
    tty.c_cc[VTIME] = 10;           // wait for 1s, returning data asap
    tty.c_cc[VMIN] = 0;

    /* Setting Baud Rate */
    cfsetispeed(&tty, B115200);
    cfsetospeed(&tty, B115200);


    /* Saving tty settings */
    if (tcsetattr(pd, TCSANOW, &tty) != 0) {
        printf("Error %i: %s from tcsetattr\n", errno, strerror(errno));
        return 1;
    }


    /* Writing on serial port */
    unsigned char message[] = "Hello\r";
    write(pd, message, sizeof(message));


    /* Reading from serial device */
    char buff[1024];

    int n = read(pd, &buff, sizeof(buff));
    printf("Data read: %s", buff);


    /* Close serial port */
    close(pd);
    return 0;
}