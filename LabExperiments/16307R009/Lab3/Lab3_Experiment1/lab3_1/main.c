/*

* Author: Texas Instruments

* Edited by: Abhijit Divekar, AjayKumar Maurya, Punit Jain
          Electrical Engineering Department, IIT Bombay

* Description: This code structure will assist you in completing Lab 3
* Filename: main.c

* Functions: setup(), config(),Timer0IntHandler(), main(),
    ManualRedController(),ManualBlueController(),ManualGreenController(),AutomaticMode(),detectKey1Press(),detectKey2Press()

*/
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/debug.h"
#include "driverlib/pwm.h"
#include "driverlib/pin_map.h"
#include "inc/hw_gpio.h"
#include "driverlib/rom.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"
#include "inc/hw_ints.h"
#include <time.h>


#define PWM_FREQUENCY 55

enum State {Automatic, Manual, ManualR, ManualG, ManualB};
enum State currState=Automatic;
enum States {IdleState1, PressState1, ReleaseState1, IdleState2, PressState2, ReleaseState2};
enum States currentState=IdleState1;
enum States currentState1=IdleState2;

float delayCount=1;
int flag=0, flag1=0;
int count1=0;
int count2=0;
int sw1shortPress=0;
int sw1longPress=0;
int sw2shortPress=0;
int sw2longPress=0;
int sw1shortPressCounter=0;
int counterForClearing=0;

volatile uint32_t ui32Load;
volatile uint32_t ui32PWMClock;
volatile uint8_t ui8AdjustR =254;
volatile uint8_t ui8AdjustG =10;
volatile uint8_t ui8AdjustB =10;

uint32_t ui32Period;

void ManualRedController()
{PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, ui8AdjustR*ui32Load/1000);
    }

void ManualBlueController()
{PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, ui8AdjustB*ui32Load/1000);
    }
void ManualGreenController()
{PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, ui8AdjustG*ui32Load/1000);
    }


void AutomaticMode(void)
{PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, ui8AdjustR*ui32Load/1000);
PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, ui8AdjustG*ui32Load/1000);
PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, ui8AdjustB*ui32Load/1000);
if (ui8AdjustR==254 && ui8AdjustG < 254 && ui8AdjustB==10)
{   ui8AdjustG++;
    SysCtlDelay(delayCount*100000);}
if (ui8AdjustR > 10 && ui8AdjustG == 254 && ui8AdjustB==10)
{   ui8AdjustR--;
    SysCtlDelay(delayCount*100000);}
if (ui8AdjustR ==10 && ui8AdjustG == 254 && ui8AdjustB < 254)
{   ui8AdjustB++;
    SysCtlDelay(delayCount*100000);}
if (ui8AdjustR ==10 && ui8AdjustG > 10 && ui8AdjustB == 254)
{   ui8AdjustG--;
    SysCtlDelay(delayCount*100000);}
if (ui8AdjustR < 254 && ui8AdjustG == 10 && ui8AdjustB == 254)
{   ui8AdjustR++;
    SysCtlDelay(delayCount*100000);}
if (ui8AdjustR == 254 && ui8AdjustG == 10 && ui8AdjustB > 10)
{   ui8AdjustB--;
    SysCtlDelay(delayCount*100000);}
}


unsigned char detectKey1Press()
{switch (currentState)
{case IdleState1: {
                 sw1shortPress=0;
                 sw1longPress=0;
                 if(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4)==0)
                 {currentState = PressState1;
                 count1=0;
                 }
                 else
                 {currentState = IdleState1;
                 }
                 break;
                 }
case PressState1: {if(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4)==0)
                 {currentState = ReleaseState1;
                 flag =1;
                 if (sw2longPress ==1)
                 {sw1shortPressCounter+=1;}
                 }
                 else
                 {currentState = IdleState1;
                 }
                 break;
                 }
case ReleaseState1:{if(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4)==0)
                  {currentState = ReleaseState1;
                  count1++;
                  if (count1 > 100)
                  {
                      sw1longPress =1;
                  }
                  }
                  else
                  {currentState = IdleState1;
                  flag =0;
                  sw1shortPress =1;

                  delayCount=delayCount/2;
                  }
                  break;
                  }
}return flag;
}

unsigned char detectKey2Press()
{switch (currentState1)
{case IdleState2: {sw2longPress=0;
                 sw2shortPress=0;
                 if(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0)==0)
                 {currentState1 = PressState2;
                 count2=0;
                 }
                 else
                 {currentState1 = IdleState2;
                 }
                 break;
                 }
case PressState2: {if(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0)==0)
                 {currentState1 = ReleaseState2;
                 flag1 =1;
                 }
                 else
                 {currentState1 = IdleState2;
                 }
                 break;
                 }
case ReleaseState2:{if(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0)==0)
                  {currentState1 = ReleaseState2;
                  count2++;
                  if (count2 > 100)
                  {
                      sw2longPress=1;
                  }
                  }
                  else
                  {currentState1 = IdleState2;
                  flag1 =0;
                  sw2shortPress =1;
                  delayCount=delayCount*2;
                  }
                  break;
                  }
}return flag1;
}

int main(void) {

    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);
    SysCtlPWMClockSet(SYSCTL_PWMDIV_64);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);

    GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_1| GPIO_PIN_2| GPIO_PIN_3);
    GPIOPinConfigure(GPIO_PF1_M1PWM5);
    GPIOPinConfigure(GPIO_PD0_M0PWM6);
    GPIOPinConfigure(GPIO_PF3_M1PWM7);

    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01;
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;
    GPIODirModeSet(GPIO_PORTF_BASE, GPIO_PIN_4|GPIO_PIN_0, GPIO_DIR_MODE_IN);
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4|GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    ui32PWMClock = SysCtlClockGet() / 64;
    ui32Load = (ui32PWMClock / PWM_FREQUENCY) - 1;
    PWMGenConfigure(PWM1_BASE, PWM_GEN_2, PWM_GEN_MODE_DOWN);
    PWMGenConfigure(PWM1_BASE, PWM_GEN_3, PWM_GEN_MODE_DOWN);
    PWMGenPeriodSet(PWM1_BASE, PWM_GEN_2, ui32Load);
    PWMGenPeriodSet(PWM1_BASE, PWM_GEN_3, ui32Load);

    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, ui8AdjustR * ui32Load / 1000);
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, ui8AdjustG * ui32Load / 1000);
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, ui8AdjustB * ui32Load / 1000);
    PWMOutputState(PWM1_BASE, PWM_OUT_5_BIT|PWM_OUT_6_BIT|PWM_OUT_7_BIT, true);
    PWMGenEnable(PWM1_BASE, PWM_GEN_2);
    PWMGenEnable(PWM1_BASE, PWM_GEN_3);


    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
    //unlock PF0 based on requirement
    //Timer delay calculation
    ui32Period = (SysCtlClockGet() / 100) ;
    TimerLoadSet(TIMER0_BASE, TIMER_A, ui32Period -1);
    //Timer Enable
    IntEnable(INT_TIMER0A);
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    IntMasterEnable();
    TimerEnable(TIMER0_BASE, TIMER_A);


    while(1)
    {
        switch(currState)
                {case Automatic:{
                                AutomaticMode();
                                if (sw2longPress ==1 && sw1shortPressCounter==1)
                                {
                                    currState= ManualR;
                                    sw1shortPressCounter=0;
                                }
                                if (sw2longPress ==1 && sw1shortPressCounter==2)
                                {currState= ManualB;
                                sw1shortPressCounter=0;}
                                if (sw2longPress ==1 && sw1longPress ==1)
                                {currState= ManualG;
                                sw1shortPressCounter=0;}

                                break;}

                case ManualR:{delayCount=1;
                              ManualRedController();
                              if (sw2longPress ==1 && sw1shortPressCounter==2)
                               {currState= ManualB;
                               sw1shortPressCounter=0;}
                              if (sw2longPress ==1 && sw1longPress ==1)
                               {currState= ManualG;
                               sw1shortPressCounter=0;}

                              break;}

                case ManualB:{ManualBlueController();
                            if (sw2longPress ==1 && sw1shortPressCounter==1)
                            {currState= ManualR;
                            sw1shortPressCounter=0;}
                            if (sw2longPress ==1 && sw1longPress ==1)
                            {currState= ManualG;
                            sw1shortPressCounter=0;}
                            break;}
                case ManualG:{ManualGreenController();
                            if (sw2longPress ==1 && sw1shortPressCounter==1)
                            {currState= ManualR;
                            sw1shortPressCounter=0;}
                            if (sw2longPress ==1 && sw1shortPressCounter==2)
                            {currState= ManualB;
                            sw1shortPressCounter=0;}
                            break;}
                }

    }
    return 0;
}


void Timer0IntHandler(void)
{// Clear the timer interrupt
TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
// Read the current state of the GPIO pin and
// write back the opposite state
detectKey1Press();
detectKey2Press();
if (currState == ManualR && sw1shortPress==1)
{ui8AdjustR += 5;
if (ui8AdjustR > 245)
{ui8AdjustR=15;}
}
if (currState == ManualR && sw2shortPress==1)
{ui8AdjustR -= 5;
if (ui8AdjustR <15)
{ui8AdjustR=245;}
}
if (currState == ManualB && sw1shortPress==1)
{ui8AdjustB += 5;
if (ui8AdjustB >245)
{ui8AdjustB=15;}}
if (currState == ManualB && sw2shortPress==1)
{ui8AdjustB -= 5;
if (ui8AdjustB <15)
{ui8AdjustB=245;}}
if (currState == ManualG && sw1shortPress==1)
{ui8AdjustG += 5;
if (ui8AdjustG >245)
{ui8AdjustG=15;}}
if (currState == ManualG && sw2shortPress==1)
{ui8AdjustG -= 5;
if (ui8AdjustG <15)
{ui8AdjustG=245;}}


}




