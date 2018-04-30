/*

* Author: Texas Instruments

* Editted by: Parin Chheda
          ERTS Lab, CSE Department, IIT Bombay

* Further Edited by: Abhijit Divekar, AjayKumar Maurya, Punit Jain
          Electrical Engineering Department, IIT Bombay


* Description: This code structure will assist you in completing Lab 2
* Filename: lab2_t2.c

* Functions: setup(), config(),Timer0IntHandler(), detect_sw1_press(), detect_sw2_press(), main()


*/

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "inc/hw_ints.h"
#include <time.h>
#include "driverlib/timer.h"


/*

* Function Name: setup()

* Input: none

* Output: none

* Description: Set crystal frequency,enable GPIO Peripherals and unlock Port F pin 0 (PF0)

* Example Call: setup();

*/

enum sw1_state {idle1, pressed1, release1};
enum sw2_state {idle2, pressed2, release2};
sw1_state = idle1;
sw2_state = idle2;

unsigned char sw1_status = 0; // 1 indicates key pressed mode 0 indicates key released

unsigned char sw2_status = 0; // 1 indicates key pressed mode 0 indicates key released

unsigned char sw1_shortFlag = 0;
unsigned char sw2_shortFlag = 0;

unsigned char sw1_longFlag = 0;
unsigned char sw2_longFlag = 0;

unsigned char sw2_flag = 0;

int Sw1_Count = 0;

int Sw2_Count = 0;

int sw1_overflow_count = 0;

int sw2_overflow_count = 0;

int color = 0;

int rgbcolorvalue = 1;

void setup(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    //unlock PF0 based on requirement

}

/*

* Function Name: pin_config()

* Input: none

* Output: none

* Description: Set Port F Pin 1, Pin 2, Pin 3 as output. On this pin Red, Blue and Green LEDs are connected.
               Set Port F Pin 0 and 4 as input, enable pull up on both these pins.

* Example Call: pin_config();

*/

void pin_config(void)
{
    // Unlocking PF0
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY ;
    HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01 ;
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0 ;

    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0); // Configuring PF0 as Input
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD_WPU); // Configuring PF0 as a Weak pull up, 4MA

//    GPIOIntDisable(GPIO_PORTF_BASE, GPIO_PIN_0); //Disable interrupt on PF0 SW2
//    GPIOIntClear(GPIO_PORTF_BASE, GPIO_PIN_0); // Clear interrupt flag

//    GPIOIntRegister(GPIO_PORTF_BASE, sw2_down); // setting sw2_down as the interrupt service
//    GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_FALLING_EDGE); // configuring Interrupt on Falling Edge of PF0 SW2



    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4); // Configuring PF4 as Input
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD_WPU); // Configuring PF4 as a Weak pull up, 4MA

//    GPIOIntDisable(GPIO_PORTF_BASE, GPIO_PIN_4); // Disable interrupt on PF4 SW1
//    GPIOIntClear(GPIO_PORTF_BASE, GPIO_PIN_4); // Clear interrupt flag

//    GPIOIntRegister(GPIO_PORTF_BASE, sw1_down); // setting sw1_down as the interrupt service
//    GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_FALLING_EDGE); // configuring Interrupt on Falling Edge of PF4 SW1


    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3); // Configuring Pins PF1, PF2, and PF3 as outputs

    GPIOIntEnable(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4); // Enabling Interrupts at Pins PF4 (SW1) and PF0 (SW2)
}


void Timer0IntHandler(void)
{
    // Clear the timer interrupt
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    TimerDisable(TIMER0_BASE, TIMER_A);
    // Read the current state of the GPIO pin and
    // write back the opposite state
/*    if(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_2))
    {
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0);
    }
    else
    {
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, rgbcolorvalue);
    }*/
    detect_sw1_press();
    detect_sw2_press();
    TimerEnable(TIMER0_BASE, TIMER_A);
    IntMasterEnable();
}

void detect_sw1_press(void)
{
    switch(sw1_state)
    {
    case idle1:
    {
        if(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4) == 0)
        {
            sw1_state = pressed1;
            break;
        }
        else
        {
            sw1_state = idle1;
        }
        break;
    }
    case pressed1:
    {
        if(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4) == 0)
        {
            sw1_state = release1;
            sw1_overflow_count++;
            break;
        }
        else
        {
            sw1_state = idle1;
        }
        break;
    }
    case  release1:
    {
        if(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4) == 0)
        {
            sw1_overflow_count++;
        }
        else
        {
            if(sw1_overflow_count>50)
            {
                sw1_longFlag = 1;
            }
            else
            {
                sw1_shortFlag = 1;
            }
            sw1_overflow_count = 0;
            sw1_state = idle1;
        }
        break;

    }
    }

/*    if (sw1_status == 1)
    {
        if (GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4) == 0)
        {
            sw1_flag = 1;
            sw1_status = 0;
            while(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4) == 0);
        }

    }
    else
    {
        if (GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4) == 0)
        {
            sw1_status = 1;
        }
    }*/

}


void detect_sw2_press(void)
{
    switch(sw2_state)
    {
    case idle2:
    {
        if(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0) == 0)
        {
            sw2_state = pressed2;
            break;
        }
        else
        {
            sw2_state = idle2;
        }
        break;
    }
    case pressed2:
    {
        if(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0) == 0)
        {
            sw2_state = release2;
            sw2_overflow_count++;
            break;
        }
        else
        {
            sw2_state = idle1;
        }
        break;
    }
    case  release2:
    {
        if(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0) == 0)
        {
            sw2_overflow_count++;
        }
        else
        {
            if(sw2_overflow_count>50)
            {
                sw2_longFlag = 1;
            }
            else
            {
                sw2_shortFlag = 1;
            }
            sw2_overflow_count = 0;
            sw2_state = idle2;
        }
        break;

    }
    }

/*    if (sw2_status == 1)
    {
        if (GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0) == 0)
        {
            sw2_flag = 1;
            sw2_status = 0;
            while(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0) == 0);
        }

    }
    else
    {
        if (GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0) == 0)
        {
            sw2_status = 1;
        }
    }*/
}

int main(void)
{
    uint32_t timer0Period;

    setup();
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
    pin_config();

    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);


    timer0Period = (SysCtlClockGet() / 100);
    TimerLoadSet(TIMER0_BASE, TIMER_A, timer0Period -1);

    IntEnable(INT_TIMER0A);
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);


    TimerEnable(TIMER0_BASE, TIMER_A);


    IntMasterEnable();
    //int count = 0;

//variable initialization
//    uint32_t sw2status = 2;
//    int32_t SW1_current = 0xffffffff;
//    int32_t SW2_current = 0xffffffff;
//    uint32_t delaymultiplier = 1;
//    uint32_t delaytime = 0;
//    uint32_t colormultiplier = 1;
    while(1)
    {
        if(sw1_shortFlag==1)
        {
            color++;
            Sw1_Count++;
            sw1_shortFlag = 0;
        }

        if(sw2_shortFlag==1)
        {
            Sw2_Count++;
            sw2_shortFlag = 0;
        }

        switch(color%3)
        {
        case 0: // if color = 0 Red
            {
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 2);
                rgbcolorvalue = 2;
                break;
            }
        case 1: // if color = 1 Green
            {
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 8);
                rgbcolorvalue = 8;
                break;
            }
        case 2: // if color = 2 Blue
            {
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 4);
                rgbcolorvalue = 4;
                break;
            }
        }
        SysCtlDelay(500000);
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0);
        SysCtlDelay(5000000);

        //count++;
    }
}
