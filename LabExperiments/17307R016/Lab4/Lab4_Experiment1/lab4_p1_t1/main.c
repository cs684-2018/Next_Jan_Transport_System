/*

* Author: Texas Instruments

* Edited by: Abhijit Divekar, AjayKumar Maurya, Punit Jain
          Electrical Engineering Department, IIT Bombay

* Description: This code structure will assist you in completing Lab 4
* Filename: main.c

* Functions: adc_init(void), main(void)
*/
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/sysctl.h"
#include "driverlib/adc.h"
#include "driverlib/gpio.h"

uint32_t ui32ADC0ValueSeq1[4];
uint32_t ui32ADC0ValueSeq2[4];

uint32_t JoyStick_Horizontal_ADC_Value = 2048;
uint32_t JoyStick_Vertical_ADC_Value = 2048;



void adc_init(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);

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
/**
 * main.c
 */
void main(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);

    //Disabling the Buzzer at PC4
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_4);
    GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4, 0x00000010);

    // Enabling the ADC0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    GPIOPinTypeADC(GPIO_PORTD_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    adc_init();
    while(1)
    {
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

}
