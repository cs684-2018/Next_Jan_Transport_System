/*

* Author: Texas Instruments

* Edited by: Abhijit Divekar, AjayKumar Maurya, Punit Jain
          Electrical Engineering Department, IIT Bombay

* Description: This code structure will assist you in completing Lab 4
* Filename: main.c

* Functions: adc_init(),timer0_init(), Joystick_read(), Timer0IntHandler(), main().

* Global Variables:
*uint32_t ui32ADC0ValueSeq1[4];
*uint32_t ui32ADC0ValueSeq2[4];
*uint32_t JoyStick_Horizontal_ADC_Value = 2048;
*uint32_t JoyStick_Vertical_ADC_Value = 2048;

*/

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_gpio.h"
//#include "inc/hw_ints.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "inc/tm4c123gh6pm.h"
#include "driverlib/interrupt.h"
#include "driverlib/debug.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/timer.h"
#include "driverlib/adc.h"

// Global Variable Declaration

uint32_t ui32ADC0ValueSeq1[4];
uint32_t ui32ADC0ValueSeq2[4];

uint32_t JoyStick_Horizontal_ADC_Value = 2048;
uint32_t JoyStick_Vertical_ADC_Value = 2048;


void adc_init(void)
{
    SysCtlPeripheralDisable(SYSCTL_PERIPH_GPIOD);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);

    GPIOPinTypeADC(GPIO_PORTD_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    ADCSequenceConfigure(ADC0_BASE, 1, ADC_TRIGGER_PROCESSOR, 0);

    ADCSequenceStepConfigure(ADC0_BASE, 1, 0, ADC_CTL_CH7);
    ADCSequenceStepConfigure(ADC0_BASE, 1, 1, ADC_CTL_CH7);
    ADCSequenceStepConfigure(ADC0_BASE, 1, 2, ADC_CTL_CH7);
    ADCSequenceStepConfigure(ADC0_BASE, 1, 3 , ADC_CTL_CH7|ADC_CTL_IE|ADC_CTL_END);
    ADCSequenceEnable(ADC0_BASE, 1);


    ADCSequenceConfigure(ADC0_BASE, 2, ADC_TRIGGER_PROCESSOR, 1);

    ADCSequenceStepConfigure(ADC0_BASE, 2, 0, ADC_CTL_CH6);
    ADCSequenceStepConfigure(ADC0_BASE, 2, 1, ADC_CTL_CH6);
    ADCSequenceStepConfigure(ADC0_BASE, 2, 2, ADC_CTL_CH6);
    ADCSequenceStepConfigure(ADC0_BASE, 2, 3 , ADC_CTL_CH6|ADC_CTL_IE|ADC_CTL_END);
    ADCSequenceEnable(ADC0_BASE, 2);
}


void timer0_init()
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);

    uint32_t timer0Period;
    timer0Period = (SysCtlClockGet() / 100);
    TimerLoadSet(TIMER0_BASE, TIMER_A, timer0Period -1);

    IntEnable(INT_TIMER0A);
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);


    TimerEnable(TIMER0_BASE, TIMER_A);


    IntMasterEnable();
}

void Joystick_read()
{
    adc_init();
    ADCIntClear(ADC0_BASE, 1);
    ADCIntClear(ADC0_BASE, 2);
    ADCProcessorTrigger(ADC0_BASE, 1);

    while(!ADCIntStatus(ADC0_BASE, 1, false))
    {
    }
    ADCSequenceDataGet(ADC0_BASE, 1, ui32ADC0ValueSeq1);

    JoyStick_Vertical_ADC_Value = (ui32ADC0ValueSeq1 [0] + ui32ADC0ValueSeq1 [1] + ui32ADC0ValueSeq1 [2] + ui32ADC0ValueSeq1 [3])/4;


    ADCProcessorTrigger(ADC0_BASE, 2);

    while(!ADCIntStatus(ADC0_BASE, 2, false))
    {
    }
    ADCSequenceDataGet(ADC0_BASE, 2, ui32ADC0ValueSeq2);

    JoyStick_Horizontal_ADC_Value = (ui32ADC0ValueSeq2 [0] + ui32ADC0ValueSeq2 [1] + ui32ADC0ValueSeq2 [2] + ui32ADC0ValueSeq2 [3])/4;
}

void Timer0IntHandler(void)
{
    // Clear the timer interrupt
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    TimerDisable(TIMER0_BASE, TIMER_A);

    // tasks start

    Joystick_read();

    // tasks end

    TimerEnable(TIMER0_BASE, TIMER_A);
    IntMasterEnable();
}

int main()
{
    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);

    /*This ensures buzzer remains OFF, since PC4 when logic 0 turns ON buzzer */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_4);
    GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_4,16);

    adc_init();
    timer0_init();



    while(1)
    {

        //SysCtlDelay(370000);
        // Select a page and display lines on it from column 0 to 127
        //SysCtlDelay(670000);
    }

}
