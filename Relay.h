/**************************************************************************************************

    FILE NAME:   RELAY.h


**************************************************************************************************

    FILE NAME:   RELAY.h

    Header file for RELAY control declarations

****************************************************************************************************/

/*****************************************************************************
        INCLUDE FILES
*****************************************************************************/
#include "Constants.h"

/****************************************************************************
        CONSTANTS
*****************************************************************************/


#define RLY_FAIL                                1
#define RLY_SUCCESS                             0


#define RLY1_LE1_DIR                            TRISC5_bit
//#define RLY1_LE2_DIR                            TRISC6_bit
#define RLY1_LE1                                LATC5_bit
//#define RLY1_LE2                                LATC6_bit

/****************************************************************************
        GLOBAL VARIABLES
*****************************************************************************/

/****************************************************************************
        FUNCTION PROTOTYPES
*****************************************************************************/


void Init_Relays(void);
unsigned char Relay_Set(unsigned char state);