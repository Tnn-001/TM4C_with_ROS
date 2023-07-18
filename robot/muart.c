#include "muart.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_gpio.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"

extern int32_t DesierdSpeed1;
extern int32_t DesierdSpeed2;
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
        a = abs(a);
        *(Array + 1) = (uint8_t)(a&0xff);
    }
    if (b >= 0)
    {
        *(Array + 2) = 0x00;
        *(Array + 3) = (uint8_t)(b);
    }
    else
    {
        *(Array + 2) = 0xff;
        b = abs(b);
        *(Array + 3) = (uint8_t)(b&0xff);
    }
}
void USART0_IRQHandler(void)
{
    uint8_t i = 0;
    uint8_t num_Arry[4];
    uint32_t re_buf;
    // ��ȡ�ж�״̬
    uint32_t status = UARTIntStatus(UART0_BASE, true);
    // ����жϱ�־λ
    UARTIntClear(UART0_BASE, status);
    // �ж�UART0��û���ַ�δ��ȡ
    while (UARTCharsAvail(UART0_BASE))
    {
        // ������ַ�Ϊ��ȡ��ȡ����ʹ��UARTCharGetNonBlocking��ֹ�ȴ�
        re_buf = UARTCharGetNonBlocking(UART0_BASE);
        // ����ȡ�����ַ��ٷ���
        num_Arry[i] = (uint8_t)re_buf;
        i++;
    }
    DesierdSpeed1 = ArrayToVariable(num_Arry, 2);
    DesierdSpeed2 = ArrayToVariable(num_Arry + 2, 2);
    UARTprintf("%i, %i \n", DesierdSpeed1, DesierdSpeed2);
}
void USART_Config(void)
{
    // ʹ������
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    // ʹ�ܸ���
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    // �����ź�
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_1);
    // ����UARTprintfʹ��
    // ���ô���0��ʱ��ΪUART_CLOCK_PIOSC
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);
    // ���ô���0�������벨��ʱ��
    UARTStdioConfig(0, 115200, 16000000);
    // ʹ��FIFO
    UARTFIFOEnable(UART0_BASE);
    // ���շ��͵�FIFO��Ϊ1/4��Ҳ����16*1/4=4���ֽ�
    UARTFIFOLevelSet(UART0_BASE, UART_FIFO_TX2_8,
                     UART_FIFO_RX2_8);
    // ʹ�ܴ��ڵĽ�������ճ�ʱ�ж�
    UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);
    // ע���жϺ���
    UARTIntRegister(UART0_BASE, USART0_IRQHandler);
    // �����ж����ȼ�
    IntPrioritySet(INT_UART0, 0);
    // �����ж�
    IntEnable(INT_UART0);
    IntMasterEnable();
    // ʹ�ܴ���
    UARTEnable(UART0_BASE);
}
