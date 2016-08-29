 /**************************************************************************************************


    FILE NAME:   Interrupt.h

    Header file for gInterrupt handler

****************************************************************************************************/

/*****************************************************************************
        INCLUDE FILES
*****************************************************************************/
#include "Constants.h"

/****************************************************************************
        CONSTANTS
*****************************************************************************/

extern volatile unsigned int counter;
extern volatile unsigned char PercentLED,PercentLCD,Interval;
extern const unsigned char PercentLedPWM[MAX_LED_LEVEL+1]={0,2,4,6,10};
extern unsigned int FlashTimer;