/*

* Author: Texas Instruments

* Editted by: Parin Chheda
          ERTS Lab, CSE Department, IIT Bombay

* Description: This code structure will assist you in completing Lab 1
* Filename: lab-1.c

* Functions: setup(), config(), main()


*/

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "inc/hw_ints.h"
#include <time.h>


/*

* Function Name: setup()

* Input: none

* Output: none

* Description: Set crystal frequency,enable GPIO Peripherals and unlock Port F pin 0 (PF0)

* Example Call: setup();

*/
void setup(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_4|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
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

    //GPIOIntDisable(GPIO_PORTF_BASE, GPIO_PIN_0); //Disable interrupt on PF0 SW2
    //GPIOIntClear(GPIO_PORTF_BASE, GPIO_PIN_0); // Clear interrupt flag

    //GPIOIntRegister(GPIO_PORTF_BASE, sw2_down); // setting sw2_down as the interrupt service
    //GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_FALLING_EDGE); // configuring Interrupt on Falling Edge of PF0 SW2



    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4); // Configuring PF4 as Input
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD_WPU); // Configuring PF4 as a Weak pull up, 4MA

    //GPIOIntDisable(GPIO_PORTF_BASE, GPIO_PIN_4); // Disable interrupt on PF4 SW1
    //GPIOIntClear(GPIO_PORTF_BASE, GPIO_PIN_4); // Clear interrupt flag

    //GPIOIntRegister(GPIO_PORTF_BASE, sw1_down); // setting sw1_down as the interrupt service
    //GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_FALLING_EDGE); // configuring Interrupt on Falling Edge of PF4 SW1


    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3); // Configuring Pins PF1, PF2, and PF3 as outputs

    GPIOIntEnable(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4); // Enabling Interrupts at Pins PF4 (SW1) and PF0 (SW2)
}

/*void sw1_down(void)
{
    if (GPIOIntStatus(GPIO_PORTF_BASE, false) & GPIO_PIN_4)
    {
            GPIOIntRegister(GPIO_PORTF_BASE, sw1_up); // Configure sw1_up as the interrupt service for PF4
            GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_RISING_EDGE); // Configure SW1 PF4 for rising edge trigger
            GPIOIntClear(GPIO_PORTF_BASE, GPIO_PIN_4);  // Clear interrupt flag


            SysCtlDelay(1000000); //debouncing
            if (delaymultiplier == 4)
            {
                delaymultiplier = 1;
            }
            else
            {
            delaymultiplier = delaymultiplier*2;
            }
            blink_rgb_led();

    }
}*/

/*void sw1_up(void)
{
    if (GPIOIntStatus(GPIO_PORTF_BASE, false) & GPIO_PIN_4)
    {
            GPIOIntRegister(GPIO_PORTF_BASE, sw1_down); // Configure sw1_up as the interrupt service for PF4
            GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_FALLING_EDGE); // Configure SW1 PF4 for rising edge trigger
            GPIOIntClear(GPIO_PORTF_BASE, GPIO_PIN_4);  // Clear interrupt flag
    }
}*/
/*void blink_rgb_led()
{

    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3)
}*/


int main(void)
{


    setup();
    pin_config();
//    uint32_t sw2status = 2;
    int32_t SW1_current = 0xffffffff;
    int32_t SW2_current = 0xffffffff;
    uint32_t delaymultiplier = 1;
    uint32_t delaytime = 100;
    uint32_t colormultiplier = 1;
    while(1)
    {
        /*
            Refer to helper document for writing logic for different parts of the lab-1 problem statement
        */
        SW1_current = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4);
        SW2_current = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0);

        if (SW1_current == 0)
        {
            SysCtlDelay(1000000); //debouncing
            if (delaymultiplier == 4)
            {
                delaymultiplier = 1;
            }
            else
            {
            delaymultiplier = delaymultiplier*2;
            }
            delaytime = 670000*delaymultiplier;
            while(SW1_current == 0)
            {
                SW1_current = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0);
            }

        }

        if (SW2_current == 0)
        {
            SysCtlDelay(1000000); //debouncing
            if (colormultiplier == 8)
            {
                colormultiplier = 2;
            }
            else
            {
            colormultiplier = colormultiplier*2;
            }
            while(SW2_current == 0)
            {
                SW2_current = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0);
            }
        }
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, colormultiplier);
        SysCtlDelay(delaytime);
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0);
        SysCtlDelay(delaytime);


    }
}
