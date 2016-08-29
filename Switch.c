/*****************************************************************************

    FILE NAME:     Switch.c

    DESCRIPTION:   User switch(Digital input ) functions.
    AUTHOR:        Bibin Paul
    Property of Intelliquip
*****************************************************************************/

/*****************************************************************************
        INCLUDE FILES
*****************************************************************************/
#include "Switch.h"

/****************************************************************************
        GLOBAL VARIABLES
*****************************************************************************/

/****************************************************************************
        LOCAL MACROS
*****************************************************************************/
//NONE

/****************************************************************************
        LOCAL FUNCTIONS
*****************************************************************************/
//NONE



/****************************************************************************
FUNCTION: InitSwitch

DESCRIPTION:
    Initialize the digital input on pin RB2 to
    1. work as a digital input to control LED light strip
    2. wake up the display (and controller) from sleep

ARGUMENTS:

         Argument                     Description                                                Type
---------------------------------------------------------------------------------------------
    NONE
RETURN VALUE:

    NONE


PRE-CONDITIONS:
   NONE

POST-CONDITIONS:
    NONE

IMPORTANT NOTES:
   None

*****************************************************************************/

void InitSwitch(){
   CTMUEN_bit=0;
   RBPU_bit=0;
   //USR_SWITCH_DIR=INPUT;      //MCLR pin is used as User Button, no direction(TRIS) on MCLR
   LATB2_bit=1;
   
   INVERTER_SWITCH_DIR =INPUT;


}