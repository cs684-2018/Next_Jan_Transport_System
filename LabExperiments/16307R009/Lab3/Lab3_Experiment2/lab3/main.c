/*

* Author: Texas Instruments

* Edited by: Abhijit Divekar, AjayKumar Maurya, Punit Jain
          Electrical Engineering Department, IIT Bombay

* Description: This code structure will assist you in completing Lab 3
* Filename: main.c

* Functions: detectKeyPress4(void), detectKeyPress0(),PWM_Work(), main()
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

#include "driverlib/debug.h"
#include "driverlib/pwm.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"

#define PWM_FREQUENCY 50
volatile uint32_t ui32Load;
volatile uint32_t ui32PWMClock;
volatile uint8_t ui8Adjust;

uint8_t state4=1,flag4=0,state0=1,flag0=0;

uint8_t detectKeyPress4(void)
{
    int val4 =GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0 |GPIO_PIN_4) &0x10;
    if(val4==0)
    {
        if(state4==1)
        {
            state4=2;
            flag4=0;
        }
        else if(state4==2)
        {
            state4=3;
            flag4=1;
        }
        else if(state4==3)
        {
            state4=3;
            flag4=0;
        }
    }
    else
    {
        state4=1;
        flag4=0;
    }
    return flag4;

}

uint8_t detectKeyPress0(void)
{
    int val0 =GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0 |GPIO_PIN_0) &0x01;
    if(val0==0)
    {
        if(state0==1)
        {
            state0=2;
            flag0=0;
        }
        else if(state0==2)
        {
            state0=3;
            flag0=1;
        }
        else if(state0==3)
        {
            state0=3;
            flag0=0;
        }
    }
    else
    {
        state0=1;
        flag0=0;
    }
    return flag0;

}

uint16_t count0, count4;
void PWM_Work()
{

    if(detectKeyPress4())
    {
        ui8Adjust-=6;
        count4+=1;
        if (ui8Adjust < 20)
        {
            ui8Adjust = 20;
        }
        PWMPulseWidthSet(PWM1_BASE, PWM_OUT_0, ui8Adjust * ui32Load / 1000);
    }

    if(detectKeyPress0())
    {
        count0 += 1;
        ui8Adjust+=6;
        if (ui8Adjust > 140)
        {
            ui8Adjust = 140;
        }
        PWMPulseWidthSet(PWM1_BASE, PWM_OUT_0, ui8Adjust * ui32Load / 1000);
    }

    SysCtlDelay(100000);


}


int main()
{
    ui8Adjust = 80;

    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);
    SysCtlPWMClockSet(SYSCTL_PWMDIV_64);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypePWM(GPIO_PORTD_BASE, GPIO_PIN_0);
    GPIOPinConfigure(GPIO_PD0_M1PWM0);
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01;
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;
    GPIODirModeSet(GPIO_PORTF_BASE, GPIO_PIN_4|GPIO_PIN_0, GPIO_DIR_MODE_IN);
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4|GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
    ui32PWMClock = SysCtlClockGet() / 64;
    ui32Load = (ui32PWMClock / PWM_FREQUENCY) - 1;
    PWMGenConfigure(PWM1_BASE, PWM_GEN_0, PWM_GEN_MODE_DOWN);
    PWMGenPeriodSet(PWM1_BASE, PWM_GEN_0, ui32Load);

    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_0, ui8Adjust * ui32Load / 1000);
    PWMOutputState(PWM1_BASE, PWM_OUT_0_BIT, true);
    PWMGenEnable(PWM1_BASE, PWM_GEN_0);

    while(1)
    {
        //detectKeyPress4();
        //detectKeyPress0();
        PWM_Work();
    }
    return 0;
}
