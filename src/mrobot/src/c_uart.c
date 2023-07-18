// C library headers
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
// Linux headers
#include <fcntl.h>   // Contains file controls like O_RDWR
#include <errno.h>   // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h>  // write(), read(), close()
#include "c_uart.h"
int serial_port;



int32_t ArrayToVariable(uint8_t *Array, uint8_t Length)
{
    int64_t Variable = 0;

    if (Length == 2)
    {
        Variable = (uint16_t)Array[1];
        if ((uint16_t)Array[0] > 0x88)
            Variable *= -1;
    }
    // else if(Length == 4)
    // {
    //     Variable = (((uint32_t)Array[0] << 24) + ((uint32_t)Array[1] << 16)
    //               + ((uint32_t)Array[2] << 8)  + ((uint32_t)Array[3]));
    // }
    return Variable;
}

void VariableToArray(uint8_t *Array, int16_t a, int16_t b)
{
    if (a >= 0)
    {
        *(Array) = 0x00;
        *(Array + 1) = (uint8_t)(a);
    }
    else
    {
        *(Array) = 0xff;
        uint16_t t = a;
        *(Array + 1) = (uint8_t)(t & 0xff);
    }
    if (b >= 0)
    {
        *(Array + 2) = 0x00;
        *(Array + 3) = (uint8_t)(b);
    }
    else
    {
        *(Array + 2) = 0xff;
        uint16_t t = b;
        *(Array + 3) = (uint8_t)(t & 0xff);
    }
    // *(Array) = (uint8_t)(a >> 8);
    // *(Array + 1) = (uint8_t)(a);
    // *(Array + 2) = (uint8_t)(b >> 8);
    // *(Array + 3) = (uint8_t)(b);
}
int uartInit()
{
    // Open the serial port. Change device path as needed (currently set to an standard FTDI USB-UART cable type device)
    serial_port = open("/dev/ttyACM0", O_RDWR);

    // Create new termios struct, we call it 'tty' for convention
    struct termios tty;

    // Read in existing settings, and handle any error
    if (tcgetattr(serial_port, &tty) != 0)
    {
        printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
        return -1;
    }

    tty.c_cflag |= PARODD;
    tty.c_cflag |= PARENB; // Clear parity bit, disabling parity (most common)
    tty.c_cflag |= CSTOPB; // 2位停止位
    tty.c_cflag &= ~CSIZE; // Clear all bits that set the data size
    tty.c_cflag |= CS8;    // 8 bits per byte (most common)
    // tty.c_cflag &= ~CRTSCTS;       // Disable RTS/CTS hardware flow control (most common)
    tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)

    tty.c_lflag &= ~ICANON;
    tty.c_lflag &= ~ECHO;                   // Disable echo
    tty.c_lflag &= ~ECHOE;                  // Disable erasure
    tty.c_lflag &= ~ECHONL;                 // Disable new-line echo
    tty.c_lflag &= ~ISIG;                   // Disable interpretation of INTR, QUIT and SUSP
    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
    tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK |
                     ISTRIP | INLCR | IGNCR | ICRNL); // Disable any special handling of received bytes

    tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
    tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed
    // tty.c_oflag &= ~OXTABS; // Prevent conversion of tabs to spaces (NOT PRESENT ON LINUX)
    // tty.c_oflag &= ~ONOEOT; // Prevent removal of C-d chars (0x004) in output (NOT PRESENT ON LINUX)

    tty.c_cc[VTIME] = 1; // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
    tty.c_cc[VMIN] = 0;

    // Set in/out baud rate to be 115200
    cfsetispeed(&tty, B115200);
    cfsetospeed(&tty, B115200);

    // Save tty settings, also checking for error
    if (tcsetattr(serial_port, TCSANOW, &tty) != 0)
    {
        printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
        return -1;
    }
    else
        return 1;
}
// Write to serial port
int uartWrite(uint32_t omsg)
{
    printf("omsg: %x \r\n", omsg);

    uint8_t msg[4];
    printf("omsg:>>16 %x \r\n", omsg>>16);
    printf("omsg:& 0xffff %x \r\n", omsg& 0xffff);
    VariableToArray(msg, omsg >> 16, omsg & 0xffff);
    printf("Write message: %x %x %x %x \r\n", msg[0], msg[1], msg[2], msg[3]);

    write(serial_port, msg, 4);
    tcflush(serial_port, TCIFLUSH);
}
Speed uartRead()
{
    Speed s;
    tcflush(serial_port, TCIFLUSH);
    // Allocate memory for read buffer, set size according to your needs
    unsigned char read_buf[4];

    // Normally you wouldn't do this memset() call, but since we will just receive
    // ASCII data for this example, we'll set everything to 0 so we can
    // call printf() easily.
    memset(&read_buf, '\0', sizeof(read_buf));

    // Read bytes. The behaviour of read() (e.g. does it block?,
    // how long does it block for?) depends on the configuration
    // settings above, specifically VMIN and VTIME
    int num_bytes = read(serial_port, &read_buf, sizeof(read_buf));

    // n is the number of bytes read. n may be 0 if no bytes were received, and can also be -1 to signal an error.
    if (num_bytes < 0)
    {
        printf("Error reading: %s", strerror(errno));
        return s;
    }

    // Here we assume we received ASCII data, but you might be sending raw bytes (in that case, don't try and
    // print it to the screen like this!)
    s.Speed1 = ArrayToVariable(read_buf, 2);
    s.Speed2 = ArrayToVariable(read_buf + 2, 2);
    if (s.Speed1 || s.Speed2)
    {
        // printf("Read %i bytes. Received message: %x %x %x %x \r\n", num_bytes, read_buf[0], read_buf[1], read_buf[2], read_buf[3]);
        // printf("True Speed: %i %i \n", s.Speed1, s.Speed2);
    }
    return s;
}

void uartClose()
{
    close(serial_port);
}