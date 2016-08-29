/*****************************************************************************

    FILE NAME: Led.c

    DESCRIPTION: LED On/Off functions
  *****************************************************************************/

/*****************************************************************************
        INCLUDE FILES
*****************************************************************************/
#include "Led.h"


/****************************************************************************
        GLOBAL VARIABLES
*****************************************************************************/
 //None

/****************************************************************************
        GLOBAL FUNCTIONS
*****************************************************************************/

 /****************************************************************************
FUNCTION: Init_Relay

DESCRIPTION:
    Initialize the relay port
*****************************************************************************/
void Init_Leds(void){
      /*
      initializes RELAY port as an out put port
      initialize Latch enable pins as Output pins
      */

      CCP1MD_bit=0;
      CCP2MD_bit=0;
      CCP3MD_bit=0;
      FAN_CONTROL_DIR=OUTPUT;
      INVERTER_RELAY_DIR=OUTPUT;
      LED_STRIP_DIR=OUTPUT;
      LED_RED_DIR=OUTPUT;   //RED and GREEN leds are on PGD and PGC pins, will not work when programmer is connected
      LED_GREEN_DIR=OUTPUT;

      FAN_CONTROL=OFF;
      INVERTER_RELAY=OFF;
      LED_STRIP=OFF;
      LED_RED=ON;
      LED_GREEN=ON;
      
}


/****************************************************************************
FUNCTION: Set_Output

DESCRIPTION:
    LED ON/OFF ROUTINE

ARGUMENTS:

        Argument                     Description                                                                        Type
--------------------------------------------------------------------------------
        led_no                              1   for FAN                         unsigned char
                                            2-3 for LED

        State                                LED_ON -> Ralay ON/LED On                        unsigned char
                                             LED_OFF -> Relay OFF/LED Off

RETURN VALUE:
    NONE
    
PRE-CONDITIONS:
    NONE.

POST-CONDITIONS:
    NONE.

IMPORTANT NOTES:
    NONE
*****************************************************************************/


void Set_Output(unsigned char led_no,unsigned char state){


    if(state == LED_ON){                //See if the state is ON/OFF/invalid
      switch(led_no){
      
       case 1:
            FAN_CONTROL=ON;
       break;

       case 3:
            LED_STRIP=ON;
       break;

       case 2:
            INVERTER_RELAY=ON;            //CHARGER.Inverter=ON;
       break;
       
       default:
       break;

      }

    }
    else if(state == LED_OFF){
     switch(led_no){

       case 1:
            FAN_CONTROL=OFF;
       break;

       case 3:
            LED_STRIP=OFF;
       break;
       case 2:
            INVERTER_RELAY=OFF;    //            CHARGER.Inverter=OFF;
       break;

       default:
       break;

      }
    }
    else{

    }


}

 /****************************************************************************
FUNCTION: Set_StatusLed

DESCRIPTION:
    Status LED ON/OFF/BLINK ROUTINEs

ARGUMENTS:

        Argument                     Description                            Type
--------------------------------------------------------------------------------
        Colour                              1  for Green                     unsigned char
                                            2  for RED
                                            3  for orange
                                            0  for blank


        State                                0 LED_OFF                       unsigned char
                                             1 LED_ON
                                             2 LED BLINK

RETURN VALUE:
    NONE

PRE-CONDITIONS:
    NONE.

POST-CONDITIONS:
    NONE.

IMPORTANT NOTES:
    //RED and GREEN leds are on PGD and PGC pins, will not work when programmer is connected
*****************************************************************************/


void Set_StatusLed(unsigned char colour,unsigned char state){


    LED_RED=ON;           //Clear both LEDs to default(for code optimization)
    LED_GREEN=ON;         //LEDs are driven with active low signal, so ON->led off

    if(colour==GREEN){
      LED_GREEN=OFF;
    }else if(colour==RED){
      LED_RED=OFF;
    }else if(colour==YELLOW){
      LED_GREEN=OFF;
      LED_RED=OFF;
    }
}