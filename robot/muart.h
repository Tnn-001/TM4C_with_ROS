#ifndef __MY_UART_H
#define __MY_UART_H

#include <stdint.h>
int32_t ArrayToVariable(uint8_t *Array, uint8_t Length);
void VariableToArray(uint8_t *Array, int16_t a, int16_t b);
void USART0_IRQHandler(void);
void USART_Config(void);

#endif
