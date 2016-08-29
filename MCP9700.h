/**************************************************************************************************

    FILE NAME:     MCP9700.h
    DESCRIPTION:   Temperature sensing through built in ADC.
    AUTHOR:        Bibin Paul
    Property of Intelliquip
*****************************************************************************/
/*****************************************************************************
        INCLUDE FILES
*****************************************************************************/
#include "Constants.h"

/****************************************************************************
             MACROS
*****************************************************************************/
 //ADC channel number for analog temperature sensors
#define TEMP_CHANNEL1    5
#define TEMP_CHANNEL2    4

//Pin definitions for analog temperature sensor inputs
#define TEMP1_PIN_DIR           TRISA5_bit
#define TEMP2_PIN_DIR           TRISF7_bit

#define TEMP1_PIN_ANALOG_SEL    ANSEL5_bit   //Analog IN
#define TEMP2_PIN_ANALOG_SEL    ANSEL4_bit   //Analog IN


extern unsigned int  Temperature1,Temperature2;


/****************************************************************************
        FUNCTION PROTOTYPES
*****************************************************************************/

void InitMCP9700();
void UpdateTemperature();