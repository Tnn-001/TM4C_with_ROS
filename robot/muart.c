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
    // 读取中断状态
    uint32_t status = UARTIntStatus(UART0_BASE, true);
    // 清除中断标志位
    UARTIntClear(UART0_BASE, status);
    // 判断UART0有没有字符未读取
    while (UARTCharsAvail(UART0_BASE))
    {
        // 如果有字符为读取就取出，使用UARTCharGetNonBlocking防止等待
        re_buf = UARTCharGetNonBlocking(UART0_BASE);
        // 将读取出的字符再发送
        num_Arry[i] = (uint8_t)re_buf;
        i++;
    }
    DesierdSpeed1 = ArrayToVariable(num_Arry, 2);
    DesierdSpeed2 = ArrayToVariable(num_Arry + 2, 2);
    UARTprintf("%i, %i \n", DesierdSpeed1, DesierdSpeed2);
}
void USART_Config(void)
{
    // 使能外设
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    // 使能复用
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    // 分配信号
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_1);
    // 搭配UARTprintf使用
    // 设置串口0的时钟为UART_CLOCK_PIOSC
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);
    // 设置串口0波特率与波特时钟
    UARTStdioConfig(0, 115200, 16000000);
    // 使能FIFO
    UARTFIFOEnable(UART0_BASE);
    // 接收发送的FIFO都为1/4，也就是16*1/4=4个字节
    UARTFIFOLevelSet(UART0_BASE, UART_FIFO_TX2_8,
                     UART_FIFO_RX2_8);
    // 使能串口的接收与接收超时中断
    UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);
    // 注册中断函数
    UARTIntRegister(UART0_BASE, USART0_IRQHandler);
    // 设置中断优先级
    IntPrioritySet(INT_UART0, 0);
    // 开启中断
    IntEnable(INT_UART0);
    IntMasterEnable();
    // 使能串口
    UARTEnable(UART0_BASE);
}
