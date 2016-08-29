/**************************************************************************************************

    FILE NAME:   LTC2943.h
    DESCRIPTION:   LTC2943 Battery monitor Driver header file
    AUTHOR:        Bibin Paul
    Property of Intelliquip
 *****************************************************************************/
 
 
/*****************************************************************************
        INCLUDE FILES
*****************************************************************************/
#include "Constants.h"

/****************************************************************************
        LTC2943        MACROS
*****************************************************************************/
#define ADD_LTC2943                     0xC8
#define CMD_LTC2943_READ                0xC9      //Device address+W\ bit=1
#define CMD_LTC2943_WRITE               0xC8      //Device address+W\ bit=0

#define COUNT_LTC2943_BYTES             0x18      //Number of bytes in LTC2943 chip

//Register Address
#define ADD_STATUS_REG                  0x00
#define ADD_CONTROL_REG                 0x01
#define ADD_CHARGE_REG                  0x02
#define ADD_VOLTAGE_REG                 0x08
#define ADD_CURRENT_REG                 0x0E
#define ADD_TEMPERATURE_REG             0x14



#define I2C_INPUT                        1        /* data direction input */
#define I2C_OUTPUT                        0        /* data direction output */

//STATUS register bit definitions
#define CURRENT_ALERT                   0x40
#define CHARGE_REGISTER_ALERT           0x20
#define TEMPERATURE_ALERT               0x10
#define CHARGE_HIGH_ALERT               0x08
#define CHARGE_LOW_ALERT                0x04
#define VOLTAGE_ALERT                   0x02
#define UNDER_VOLTAGE_LOCKOUT           0x01

 //CONTROL register bit definitions
 #define SHUTDOWN                       0x01
 
 #define ALCC_ALERT                     0x04
 #define ALCC_CHARGE_COMPLETE           0x02
 #define ALCC_OFF                       0x00
 
 #define PRESCALE_1                     0x00
 #define PRESCALE_4                     0x08
 #define PRESCALE_16                    0x10
 #define PRESCALE_64                    0x18
 #define PRESCALE_256                   0x20
 #define PRESCALE_1024                  0x28
 #define PRESCALE_4096                  0x30

 #define ADCMODE_AUTO                   0xC0
 #define ADCMODE_SCAN                   0x80
 #define ADCMODE_SLEEP                  0x00

 #define SUCCESS                        0x00
 #define FAIL                           0x01
 /****************************************************************************
 Data Structures
*****************************************************************************/
 //Structure containg the register definitions of the Battery manager chip LTC2943
typedef struct{
  unsigned char Status;
  unsigned char Control;
  unsigned int AccumulatedCharge;
  unsigned int ChargeThresholdHigh;
  unsigned int ChargeThresholdLow;
  unsigned int Voltage;
  unsigned int VoltageThresholdHigh;
  unsigned int VoltageThresholdLow;
  unsigned int Current;
  unsigned int CurrentThresholdHigh;
  unsigned int CurrentThresholdLow;
  unsigned int Temperature;
  unsigned int BatteryVoltage;
  unsigned int BatteryCurrent;
  unsigned char TemperatureThresholdHigh;
  unsigned char TemperatureThresholdLow;
  unsigned int  PrevAccumulatedCharge;
  unsigned int BattPercent;
  unsigned int PrevBattPercent;
  unsigned int BatteryAh;
  unsigned char Direction;
  unsigned char PrevDirection;
  unsigned char ChargeRate;
  unsigned char BattLevel;
  unsigned char PrevBattLevel;
}_LTC2943;


typedef struct{

  unsigned int Capacity;                             //x10AH
  unsigned int ChargeConstant;
  unsigned int DischargeConstant;
  unsigned int ChargingVoltage;
  unsigned int NormalVoltage;
  unsigned int LowVoltage;
  unsigned int EmptyVoltage;
  unsigned int CutOffDelay;
  unsigned int LoadOffDelay;
  unsigned char Type;
}_BATTERY;

extern  _LTC2943 LTC2943;
extern  _BATTERY BATTERY;
extern float Rsense;
extern float ChargeConst;
extern float Rmultiplier;
extern unsigned char PercentVoltFlag;
/****************************************************************************
        FUNCTION PROTOTYPES
*****************************************************************************/

void InitLTC2943(void);
void ReadStatusLTC2943(void);
unsigned char UpdateLTC2943(void);
//**********************************************************************************