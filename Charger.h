 /**************************************************************************************************


    FILE NAME:   Charger.h

    Header file for charger board measurements and control

****************************************************************************************************/

/*****************************************************************************
        INCLUDE FILES
*****************************************************************************/
#include "Constants.h"

/****************************************************************************
        CONSTANTS
*****************************************************************************/

//control and measurement pins from U1(LT8490-charger)
#define IIN_DIR                              TRISF6_bit //Analog In
#define IOUT__DIR                            TRISF5_bit //Analog In
#define MPPT_CONTROL_DIR                     TRISG4_bit //Digital OUT
#define SHUT_DN_U1_DIR                       TRISG3_bit //Digital OUT
#define FAULT_U1_DIR                         TRISC0_bit //Digital IN
#define STATUS_U1_DIR                        TRISC7_bit //Digital IN-UART
#define CHARGE_MODE_DIR                      TRISC2_bit //Digital IN

#define MPPT_CONTROL                         LATG4_bit  //Digital OUT
#define SHUT_DN_U1                           LATG3_bit  //Digital OUT
#define FAULT_U1                             PORTC0_bit //Digital IN
#define STATUS_U1                            PORTC7_bit //Digital IN-UART
#define CHARGE_MODE                          LATC2_bit  //Digital OUT

 //Sets voltage levels for CHARGER output
#define DIR_VOLT_14_6                        TRISG0_bit
#define DIR_VOLT_15_3                        TRISE0_bit

#define VOLT_14_6                            LATG0_bit
#define VOLT_15_3                            LATE0_bit

#define IIN_ANALOG_SEL                       ANSEL11_bit //Analog  IN
#define IOUT_ANALOG_SEL                      ANSEL10_bit //Analog  IN
#define DC_WALL_ANALOG_SEL                   ANSEL6_bit  //Analog  IN
#define VIN1_ANALOG_SEL                      ANSEL7_bit  //Analog  IN




/*//These pins are not connected at the moment
#define SRVO_FBIN_U1                                    //Digital IN
#define SRVO_FBOUT_U1                                   //Digital IN
#define SRVO_IIN_U1                                     //Digital IN
#define SRVO_IOUT_U1                                    //Digital IN*/

//control and measurement pins from U1(LTC4365-Protection)
#define DC_WALL_DIR                          TRISF1_bit  //Analog In
#define VIN1_DIR                             TRISF2_bit  //Analog In
#define SHUT_DN_U2_DIR                       TRISF3_bit  //output
#define FAULT_U2_DIR                         TRISF4_bit  //Input
#define SHUT_DN_U2                           LATF3_bit   //output
#define FAULT_U2                             PORTF4_bit  //Input

//ADC Channel number for charger board analog signals
#define IIN_CHANNEL                           11 //Input current
#define IOUT_CHANNEL                          10 //Output current
#define DC_WALL_CHANNEL                       6  //DC voltage from wall adapter
#define VIN1_CHANNEL                          7  //Voltage at charger input

//Vin Ratio: voltage divider ratio( 21k/199k), ADC ration(5000mv/4095) =11.5676~  185/16=11.5625
#define VIN_DIVIDER                            128
#define VIN_MULTIPLIER                         148

#define IIN_MULTIPLIER                         119
#define IIN_DIVIDER                            1024
#define IIN_OFFSET                             120

extern  unsigned int VOLTAGE_CHART[10];
/****************************************************************************
        GLOBAL VARIABLES
*****************************************************************************/
extern  unsigned char AdcState;

/****************************************************************************
 Data Structures
*****************************************************************************/

 //Structure containg the register definitions of the Battery manager chip LTC2943
typedef struct{

  unsigned int DCWall;
  unsigned int Vin;
  unsigned int Iin;
  unsigned int Iout;
  unsigned int BatteryVoltage;                  //Actual battery voltage(x10 Volts)
  //unsigned int BatteryLowVoltage;               //Battery low cutoff voltage(x10 Volts)
  //unsigned int BatteryNormalVoltage;            //Battery normal voltage=Battery low cutoff voltage+Hysterisis(x10 Volts)
  unsigned char InverterType;
  unsigned char Source;                         //Status of the voltage source selected for charging
  unsigned char Load;                          //Status of the Load control relay ON or OFF
  unsigned char Inverter;                       //Status of inverter
  unsigned char SourceState;                 //source control state machine
  unsigned char ChargerState;
  unsigned char LED;
  unsigned char Delay;
  unsigned char LedLevel;
}_CHARGER;

extern  _CHARGER CHARGER;


extern const unsigned char STATUS_MASK;
extern const unsigned char FAULT_MASK ;
extern unsigned char STATUS_CHART[7];
extern unsigned char FAULT_CHART[5];
extern unsigned char* STATUS_MSG[6];
extern unsigned char* FAULT_MSG[5];



/****************************************************************************
        FUNCTION PROTOTYPES
*****************************************************************************/


void Init_Charger(void);
void UpdateAnalogInputs(void);
unsigned int VoltageToPercentage();