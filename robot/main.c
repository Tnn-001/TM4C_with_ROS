/**
 * main.c
 */
#include <stdint.h>
#include <stdio.h>
#include "buct_hal.h"
#include "pid.h"
#include <stdbool.h>
#include "uartstdio.h"


volatile int32_t DesierdSpeed1=50;
volatile int32_t DesierdSpeed2=50;
int32_t ActualSpeed1, ActualSpeed2;
volatile uint32_t Speed1 = 500;
volatile uint32_t Speed2 = 500;
uint8_t sw1Pressednum = 0; // Variable to record the state of SW1 button
float Velcity_Kp = 0.1, Velcity_Ki = 1.1, Velcity_Kd = 0;


int main()
{
    USART_Config();
    initQEI0();        // Initialize Quadrature Encoder Interface 0
    initQEI1();        // Initialize Quadrature Encoder Interface 1
    initCarPWM();      // Initialize Car PWM signals
    enableGPIO(GPIOF); // Enable GPIO Port F

    pinMode(5, 4, INPUT_PULLUP);
    // Set pin 5 of port 4 as input with pull-up resistor

    while (1)
    {
        // // Wait for the user to press SW1 button
        // if (sw1Pressed() == 1)
        // {
        //     eputs("Pressed !"); // Print "Pressed !" message
        //     delay(1e6);         // Delay for 1 million cycles
        //     if (sw1Pressednum == 0)
        //     {
        //         sw1Pressednum = 1; // Set the SW1 button state to 1
        //     }
        //     else
        //     {
        //         sw1Pressednum = 0; // Set the SW1 button state to 0
        //     }
        // }

        // // Check the state of SW1 button and perform the corresponding operation
        // if (sw1Pressednum == 1)
        // {
        ActualSpeed1 = getMotor1Velocity(); // Get the actual speed from QEI0 module
        ActualSpeed2 = getMotor2Velocity(); // Get the actual speed from QEI1 module
//        UARTprintf("\nVelocity1 : %i\n",ActualSpeed1);
//        UARTprintf("\nVelocity2 : %i\n\n",-1*ActualSpeed2);


        // First time pressing the SW1 button

        Speed1 = Velocity_FeedbackControl1(DesierdSpeed1, ActualSpeed1);
        Speed2 = Velocity_FeedbackControl2(-1*DesierdSpeed2, ActualSpeed2);

//        UARTprintf("\nSpeed1 : %d\n",Speed1);
//        UARTprintf("\nSpeed2 : %d\n\n",Speed2);
        UARTprintf("%i,%i,%i\r\n",DesierdSpeed1,ActualSpeed1,-1*ActualSpeed2);
        setMotor1(Speed1); // Set the speed of Motor 1
        setMotor2(Speed2); // Set the speed of Motor 2
        waitQEI0Speed();   // Wait for the next velocity reading from QEI0 to complete
        waitQEI1Speed();   // Wait for the next velocity reading from QEI1 to complete
        // }
        // else
        // {
        //     // Second time pressing the SW1 button, stop Motor 1 (50% load)
        //     eputs("Stop !\n"); // Print "Stop !" message
        //     Speed1 = 500;      // Set Speed1 to 500
        //     Speed2 = 500;      // Set Speed2 to 500
        //     setMotor1(Speed1); // Set the speed of Motor 1
        //     setMotor2(Speed2); // Set the speed of Motor 2
        // }
    }
    return 0;
}
