
/*****************************************************************************

    FILE NAME:     MCP9700.c

    DESCRIPTION:   MCP9700 Temperature sensor Driver functions
    AUTHOR:        Bibin Paul
    Property of Intelliquip
*****************************************************************************/

/*****************************************************************************
        INCLUDE FILES
*****************************************************************************/
#include "MCP9700.h"

/****************************************************************************
        GLOBAL VARIABLES
*****************************************************************************/
unsigned int  Temperature1,Temperature2;

/****************************************************************************
        LOCAL MACROS
*****************************************************************************/
//NONE

/****************************************************************************
        LOCAL FUNCTIONS
*****************************************************************************/
//NONE



/****************************************************************************
FUNCTION: InitMCP9700

DESCRIPTION:
    Initialize the ADC for the temeperature inputs

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
void InitMCP9700(){

  TEMP1_PIN_DIR=INPUT;    //Set AN4 pin as input
  TEMP2_PIN_DIR=INPUT;    //Set AN5 pin as input

  TEMP1_PIN_ANALOG_SEL  = ANALOG;   // Configure analog function on AN4 and AN5 pins
  TEMP2_PIN_ANALOG_SEL  = ANALOG;

}
/****************************************************************************
FUNCTION: UpdateTemperature

DESCRIPTION:
    Read the analog values from temperature ports and convert it to temperature
    Update global register for access from other functions
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
            Add a runnung average here to compensate for noise
*****************************************************************************/

void UpdateTemperature(){
  unsigned long Temp=0;
  //Delay_ms(100);

  Temp = ADC_Get_Sample(TEMP_CHANNEL1);    // read analog value from Temperature sensor 1
  if(Temp>400){
       Temp-=400;
       Temp*=100;
       Temp/=195;
       Temperature1=(unsigned int)Temp;
  }else{
        Temperature1=0;
  }
  //Delay_ms(10);

  Temp  = ADC_Get_Sample(TEMP_CHANNEL2);    // read analog value from Temperature sensor 2

  if(Temp>400){
       Temp-=400;
       Temp*=100;
       Temp/=195;
       Temperature2=(unsigned int)Temp;
  }else{
        Temperature2=0;
  }



}