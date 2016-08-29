
/*****************************************************************************

    FILE NAME:     Interrupt.c

    DESCRIPTION:   Interrupt handler functions
    AUTHOR:        Bibin Paul
    Property of Intelliquip
*****************************************************************************/

/*****************************************************************************
        INCLUDE FILES
*****************************************************************************/
#include "Interrupt.h"
 #include "Led.h"
/****************************************************************************
        GLOBAL VARIABLES
*****************************************************************************/
volatile unsigned int counter=0;
volatile unsigned char PercentLED,PercentLCD,Interval;
unsigned int FlashTimer;

const unsigned char PercentLedPWM[MAX_LED_LEVEL+1]={0,2,4,6,10};//{0,2,5,7,10};

/****************************************************************************
       LOCAL CONSTANTS
*****************************************************************************/







void interrupt() {
  if (RC1IF_bit) {
    counter++;
  }
  if (TMR2IF_bit){
    TMR2IF_bit = 0;
    //Enter your code here
    if(Interval){
       Interval--;
    }else{
       Interval=10;
    }
    LATB3_bit=0;
    LATC6_bit=0;
    if(PercentLED>Interval){
        LATB3_bit=1;
        LATC6_bit=1;
    }
    //LCD_BACKLIGHT=OFF;
    //FAN_CONTROL=OFF;
    //if(PercentLCD>=Interval){
    //    LCD_BACKLIGHT=ON;
        //FAN_CONTROL=ON;
    //}
     if(FlashTimer){
        FlashTimer--;
     }
    

  }

}