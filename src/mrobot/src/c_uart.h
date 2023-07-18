#ifndef __C_UART_H__
#define __C_UART_H__

#include <stdint.h>
typedef struct
{
    int Speed1;
    int Speed2;
} Speed;
int uartInit();
int uartWrite(uint32_t omsg);
Speed uartRead();
void uartClose();

#endif 
