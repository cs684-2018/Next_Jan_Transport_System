/*

* Author: Texas Instruments

* Edited by: Abhijit Divekar, AjayKumar Maurya, Punit Jain
          Electrical Engineering Department, IIT Bombay

* Description: This code structure will assist you in completing Lab 4
* Filename: main.c

* Functions: uart_init(void), uart_num_put(uint32_t number1), adc_init(void), main(void).
*/
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/sysctl.h"
#include "driverlib/adc.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/uart.h"

uint32_t ui32ADC0ValueSeq1[4];
uint32_t ui32ADC0ValueSeq2[4];

uint32_t JoyStick_Horizontal_ADC_Value = 2048;
uint32_t JoyStick_Vertical_ADC_Value = 2048;

void uart_init(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200,
                        (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
}


void uart_num_put(uint32_t number1)
{
    int i = 0;
    unsigned char dummy1;
    for(i=3; i>=0; i--)
    {
        dummy1 = number1%10;
        UARTCharPut(UART0_BASE, dummy1 + 0x30);
        SysCtlDelay(100);
        number1 = number1/10;
    }
}

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
int main(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);

    //Disabling the Buzzer at PC4
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_4);
    GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4, 0x00000010);


    //Enabling GPIOF for LEDS
    //SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    //ssGPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);


    // Enabling the ADC0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    GPIOPinTypeADC(GPIO_PORTD_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    adc_init();
    uart_init();
    int x = 1;
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

       // unsigned char adc_val_8bit = JoyStick_Vertical_ADC_Value/32;
        UARTCharPut(UART0_BASE, 'h');
        SysCtlDelay(100);
        uart_num_put(JoyStick_Vertical_ADC_Value);
        UARTCharPut(UART0_BASE,',');
        SysCtlDelay(100);
        UARTCharPut(UART0_BASE, 'v');
        SysCtlDelay(100);
        uart_num_put(JoyStick_Horizontal_ADC_Value);
        UARTCharPut(UART0_BASE,'\n');
        SysCtlDelay(100);
        //GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, x);
        //x = ~x;
        SysCtlDelay(16000);


    }

    return 0;
}
