#include <stdio.h>
#include <math.h>
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

unsigned char x_pos, y_pos, y_page;
unsigned char Dot_Pos_x, Dot_Pos_y;

/* void glcd_cmd(cmd)
* This function sends commands to the GLCD.
* Value of RS is 0
* Command is written on data lines.
* Enable is made 1 for a short duration.
*/
void glcd_cmd(unsigned char cmd)
{
    GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3,0x00);
    GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7,0x00);

    /* RS = 0 */
    GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_6,0x00);

    /* Put command on data lines */
    GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3,cmd);
    GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7,cmd);

    /* Generate a high to low pulse on enable */
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0,0x01);
    SysCtlDelay(1340);
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0,0x00);

}




/* void glcd_data(data)
* This function sends data to the GLCD.
* Value of RS is 1
* Data is written on data lines.
* Enable is made 1 for a short duration.
*/
void glcd_data(unsigned char data)
{
    GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3,0x00);
    GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7,0x00);

    /* RS = 1 */
    GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_6,0x40);

    /* Put command on data lines */
    GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3,data);
    GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7,data);

    /* Generate a high to low pulse on enable */
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0,0x01);
    SysCtlDelay(1340);
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0,0x00);
}



/* void glcd_init()
* This function initializes the GLCD.
* Always call this function at the beginning of main program after configuring the port pins.
*/
void glcd_init()
{
    SysCtlDelay(6700000/50);                            // creates ~10ms delay - TivaWare fxn
    GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_5, 0x00);    //cbi(GPORTC_2,GLCD_RST);
    SysCtlDelay(6700000/50);
    /* Set RST */
    GPIOPinWrite(GPIO_PORTE_BASE,GPIO_PIN_5,0x20);

    /* Set CS1 (CS1=1 and CS2=0) The right side is selected(column>64) */
    GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_3,0x00);

    /* Select the start line */
    glcd_cmd(0xC0);
    SysCtlDelay(6700);

    /* Send the page */
    glcd_cmd(0xB8);
    SysCtlDelay(6700);

    /*Send the column */
    glcd_cmd(0x40);
    SysCtlDelay(6700);

    /* Send glcd on command */
    glcd_cmd(0x3F);
    SysCtlDelay(6700);

    /* Initialize the right side of GLCD */
    /* Set CS2 (CS2=1 and CS1=0) The right side is selected(column>64) */
    GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_3,0x08);

    /* Select the start line */
    glcd_cmd(0xC0);
    SysCtlDelay(6700);

    /* Send the page */
    glcd_cmd(0xB8);
    SysCtlDelay(6700);

    /*Send the column */
    glcd_cmd(0x40);
    SysCtlDelay(6700);

    /* Send glcd on command */
    glcd_cmd(0x3F);
    SysCtlDelay(6700);
}




/* void glcd_setpage(page)
* This function selects page number on GLCD.
* Depending on the value of column number CS1 or CS2 is made high.
*/
void glcd_setpage(unsigned char page)
{
    /* Set CS1 (CS1=1 and CS2=0) The right side is selected(column>64) */
    GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_3,0x00);
    glcd_cmd(0xB8 | page);
    SysCtlDelay(100);

    /* Set CS2 (CS2=1 and CS1=0) The right side is selected(column>64) */
    GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_3,0x08);
    glcd_cmd(0xB8 | page);
    SysCtlDelay(100);
}



/* void glcd_setcolumn(column)
* This function selects column number on GLCD.
* Depending on the value of column number CS1 or CS2 is made high.
*/
void glcd_setcolumn(unsigned char column)
{
    if(column < 64)
    {
        /* Set CS1 (CS1=1 and CS2=0) The left side is selected(column>64) */
        GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_3,0x00);
        glcd_cmd(0x40 | column);
        SysCtlDelay(6700);
    }
    else
    {
        /* Set CS2 (CS2=1 and CS1=0) The right side is selected(column>64) */
        GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_3,0x08);
        glcd_cmd(0x40 | (column-64));
        SysCtlDelay(6700);
    }

}



/* void glcd_cleardisplay()
* This function clears the data on GLCD by writing 0 on all pixels.
*/
void glcd_cleardisplay()
{
    unsigned char i,j;
    for(i=0;i<8;i++)
    {
        glcd_setpage(i);
        for(j=0;j<128;j++)
        {
            glcd_setcolumn(j);
            glcd_data(0x00);
        }
    }
}

void glcd_DisplayLineP1(void)
{
    glcd_setpage(1);
    unsigned char column = 0;
    for (column=0; column<128; column++)
    {
        glcd_setcolumn(column);
        glcd_data(0x01);
        SysCtlDelay(670000);
    }
}


void glcd_ClearDot(unsigned char x_data, unsigned char y_data)
{

    x_pos = x_data;
    y_pos = y_data%8;

    y_page = y_data/8;
    glcd_setpage(y_page);
    glcd_setcolumn(x_pos);
    glcd_data(0x00);
    //glcd_data(0x01);
}


void glcd_DisplayDot(unsigned char x_data, unsigned char y_data)
{

    x_pos = x_data;
    y_pos = y_data%8;

    y_page = y_data/8;
    glcd_setpage(y_page);
    glcd_setcolumn(x_pos);
    switch(y_pos)
    {
    case 0:
        glcd_data(0x01);
        break;
    case 1:
        glcd_data(0x02);
        break;
    case 2:
        glcd_data(0x04);
        break;
    case 3:
        glcd_data(0x08);
        break;
    case 4:
        glcd_data(0x10);
        break;
    case 5:
        glcd_data(0x20);
        break;
    case 6:
        glcd_data(0x40);
        break;
    case 7:
        glcd_data(0x80);
        break;
    default:
        glcd_data(0xFF);
        break;
    }
    SysCtlDelay(670000);
    glcd_setcolumn(x_pos);
    glcd_data(0x00);
    SysCtlDelay(1000);
    //glcd_data(0x01);
}


void glcd_DisplayBlock(uint32_t x_data, uint32_t y_data)
{
    /*uint32_t x_topleft, y_topleft;
    x_topleft = x_data-4;
    y_topleft = y_data-4;*/
    unsigned char y_upper, y_lower, y_bits_upper, y_bits_lower, y_page_upper, y_page_lower;
    int i;
    x_pos = x_data;
    y_pos = y_data%8;

    y_page = y_data/8;

    if(y_pos >3) // the age lower than the middle point is to be acessed and turned.
    {
        //print y_upper
        y_upper = y_pos - 3;
        y_bits_upper = 0xFF;
        for (i=0; i<y_upper;i++)
        {
            y_bits_upper = y_bits_upper - pow(2,i);
        }
        y_page_upper = y_page;

        //print y_lower

        y_lower = y_pos-4;
        y_bits_lower = 0;
        for (i=0;i<=y_lower;i++)
        {
            y_bits_lower = y_bits_lower + pow(2,i);
        }
        y_page_lower = y_page + 1;

    }
    else
    {
        y_upper = 3 - y_pos;
        y_bits_upper = 0x00;
        for (i=0; i<y_upper;i++)
        {
            y_bits_upper = y_bits_upper + pow(2,7-i);
        }
        y_page_upper = y_page - 1;

        //print y_lower

        y_lower = y_pos + 4;
        y_bits_lower = 0x00;
        for (i=0;i<=y_lower;i++)
        {
            y_bits_lower = y_bits_lower + pow(2,i);
        }
        y_page_lower = y_page;

    }


    glcd_setpage(y_page_upper);
    for (i = x_pos - 3; i < x_pos + 5; i++)
    {
        glcd_setcolumn(i);
        glcd_data(y_bits_upper);
    }
    glcd_setpage(y_page_lower);
    for (i = x_pos - 3; i < x_pos + 5; i++)
    {
        glcd_setcolumn(i);
        glcd_data(y_bits_lower);
    }

    SysCtlDelay(670000);

    glcd_setpage(y_page_upper);
    for (i = x_pos - 3; i < x_pos + 5; i++)
    {
        glcd_setcolumn(i);
        glcd_data(0x00);
    }
    glcd_setpage(y_page_lower);
    for (i = x_pos - 3; i < x_pos + 5; i++)
    {
        glcd_setcolumn(i);
        glcd_data(0x00);
    }
    SysCtlDelay(1000);

}

void adc_init(void)
{
    //SysCtlPeripheralDisable(SYSCTL_PERIPH_GPIOD);

    //SysCtlDelay(2000);

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

    //Joystick_read();

    // tasks end

    TimerEnable(TIMER0_BASE, TIMER_A);
    IntMasterEnable();
}

int main()
{
    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);


    /* Enable all the peripherals */
    /* PORTS A,E,F,C,D,B */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

     // Unlock pin PF0
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK)= GPIO_LOCK_KEY;    // unlocking sw2 switch
    HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01;
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK)= 0;

    /* Configure Enable (PF0) pin as output */
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_0);

    /*This ensures buzzer remains OFF, since PC4 when logic 0 turns ON buzzer */
    GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_4);
    GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_4,16);

    /* Configure PE5 (RST), PE0 to PE3 (D0 to D3) and PB4 to PB7(D4 to D7) as output pins */
    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
    GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
    GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_5);

        /* Configure RS (PC6) as output */
    GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_6);

        /* Configure CS1(Generated as NOT of CS2 on the Game Console Board or CS2 (PD3) as output */
    GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_3);

        /*initialize glcd*/
    glcd_init();

    // Configure ADC
    adc_init();
    //timer0_init();
    glcd_cleardisplay();
    SysCtlDelay(1000);
    while(1)
    {
        //glcd_DisplayLineP1();
        Joystick_read();
        //glcd_cleardisplay();
        //SysCtlDelay(1000);
        // Select a page and display lines on it from column 0 to 127
        Dot_Pos_x = (4095-JoyStick_Horizontal_ADC_Value)/32;
        Dot_Pos_y = (4095-JoyStick_Vertical_ADC_Value)/64;
        if(Dot_Pos_x < 3)
        {
            Dot_Pos_x = 3;
        }
        if(Dot_Pos_x > 123)
        {
            Dot_Pos_x = 123;
        }
        if(Dot_Pos_y < 3)
        {
            Dot_Pos_y = 3;
        }
        if(Dot_Pos_y > 58)
        {
            Dot_Pos_y = 58;
        }




        glcd_DisplayBlock(Dot_Pos_x, Dot_Pos_y);
        //SysCtlDelay(100000);

        //glcd_cleardisplay();
        //SysCtlDelay(1000);

    }

}
