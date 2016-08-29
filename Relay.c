/*****************************************************************************

    FILE NAME: Relay.c

    DESCRIPTION: Relay On/Off functions
  *****************************************************************************/

/*****************************************************************************
        INCLUDE FILES
*****************************************************************************/
#include "Relay.h"
#include "CHARGER.h"
#include "Constants.h"

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
void Init_Relays(void){
      /*
      initializes RELAY port as an out put port
      initialize Latch enable pins as Output pins
      */

      RLY1_LE1_DIR=OUTPUT;
      RLY1_LE1=OFF;
}


/****************************************************************************
FUNCTION: Relay_Set

DESCRIPTION:
    LATCH RELAY ON/OFF ROUTINE

ARGUMENTS:

        Argument                     Description                                Type
--------------------------------------------------------------------------------
        relay_no                        1   for latching and               unsigned char
                                            2-4 for EM_Relay

        State                                RLY_ON -> Ralay ON            unsigned char
                                             RLY_OFF -> Relay OFF

RETURN VALUE:
    RLY_SUCCESS                                Relay operation is successful
    RLY_FAIL                        Invalid Relay Type, Relay Number or Relay State passed

PRE-CONDITIONS:
    NONE.

POST-CONDITIONS:
    NONE.

IMPORTANT NOTES:
    Code modidfied to accomodate new relay based battery protection circuit. Only one pin RC5 is used to control the relay.
    The RC6/TXEN pin cannot be used(though its free) for any other application.
*****************************************************************************/


unsigned char Relay_Set(unsigned char state){

       //Serial port and relay shares the same pins, Disable serial port before writing to relay

         if(state == RELAY_ON){                                        //See if the state is ON/OFF/invalid
           RLY1_LE1=1;
           CHARGER.Load=ON;
          }
          else if(state == RELAY_OFF){
             RLY1_LE1=0;
             CHARGER.Load=OFF;
          }
          else{
              return(RLY_FAIL);        //If relay STATE is other than 0 or 1
          }
         Delay_ms(1500);

         return(RLY_SUCCESS);            //successful relay operation
}
