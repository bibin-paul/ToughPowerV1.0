
/*****************************************************************************

    FILE NAME:     LTC2943.c

    DESCRIPTION:   LTC2943 Battery monitor Driver functions
    AUTHOR:        Bibin Paul
    Property of Intelliquip
*****************************************************************************/

/*****************************************************************************
        INCLUDE FILES
*****************************************************************************/
#include "LTC2943.h"

/****************************************************************************
        GLOBAL VARIABLES
*****************************************************************************/
 _LTC2943  LTC2943;    //Structure holding all the parameters of the Battery manager
 _BATTERY  BATTERY;
/****************************************************************************
        LOCAL MACROS
*****************************************************************************/
//NONE

/****************************************************************************
        LOCAL FUNCTIONS
*****************************************************************************/
//NONE



/****************************************************************************
FUNCTION: InitLTC2943

DESCRIPTION:
    Initialize the battery monitor chip, Initialize the I2C channel, Initialize the registers

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
void InitLTC2943(){
unsigned short Err=0;

  I2C1_Init(100000);                     //LTC2943 is on I2C channel 1, Initialize I2C channel 1

  I2C1_Start();                          // issue I2C start signal
  Err=I2C1_Wr(CMD_LTC2943_WRITE);           // send byte via I2C(device address + W)
  if(Err !=0){
    return;
  }
  I2C1_Wr(ADD_CONTROL_REG);               // send byte (register address)
  I2C1_Wr(ADCMODE_AUTO|PRESCALE_256|ALCC_OFF);               // send byte (control register to automode)
  //I2C1_Wr(ADCMODE_AUTO|PRESCALE_1024|ALCC_OFF);               // send byte (control register to automode)
  I2C1_Stop();

}
/****************************************************************************
FUNCTION: ReadStatusLTC2943

DESCRIPTION:
    Read the status register of LTC2943 and check for alarms

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
 void ReadStatusLTC2943(){
 unsigned char x=0,Err=0;
  LTC2943.Voltage =0;
  I2C1_Start();                          // issue I2C start signal
  Err=I2C1_Wr(CMD_LTC2943_WRITE);            // send byte via I2C(device address + W)
  if(Err !=0){
    return;
  }
  I2C1_Wr(ADD_VOLTAGE_REG);            // send byte via I2C(device address + W)
  I2C1_Repeated_Start();                 // issue I2C signal repeated start
  I2C1_Wr(CMD_LTC2943_READ);             // send byte (device address + R)
  x= I2C1_Rd(1u);          // Read the data (Acknowledge)
  LTC2943.Voltage = x;
  LTC2943.Voltage <<=8;
  x= I2C1_Rd(0u);          // Read the data (Acknowledge)
  LTC2943.Voltage |= x;
  
  I2C1_Stop();                           // issue I2C stop signal


/*I2C1_Start();                          // issue I2C start signal
  I2C1_Wr(CMD_LTC2943_WRITE);            // send byte via I2C(device address + W)

  I2C1_Wr(ADD_VOLTAGE_REG+1);               // send byte (register address)
  I2C1_Repeated_Start();                 // issue I2C signal repeated start
  I2C1_Wr(CMD_LTC2943_READ);             // send byte (device address + R)
  LTC2943.Voltage<<=8;
  LTC2943.Status |= I2C1_Rd(0u);          // Read the data (NO acknowledge)
  I2C1_Stop();*/               // issue I2C stop signal

 }
 
/****************************************************************************
FUNCTION: UpdateLTC2943

DESCRIPTION:
    Read the all  registers of LTC2943 and update internal registers

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
unsigned char UpdateLTC2943(){
 unsigned char byte=0,Err=0;
 unsigned int Temp=0;
  unsigned long  lValue;
  
  
  
 LTC2943.Temperature=0x0000;
 LTC2943.Voltage=0x0000;
 LTC2943.Current=0x0000;
 
  I2C1_Start();                          // issue I2C start signal
  Err=I2C1_Wr(CMD_LTC2943_WRITE);        // send byte via I2C(device address + W)
  if(Err !=0){
    return Err;
  }
  I2C1_Wr(ADD_CHARGE_REG);               // send byte via I2C(device address + W)
  I2C1_Repeated_Start();                 // issue I2C signal repeated start
  I2C1_Wr(CMD_LTC2943_READ);             // send byte (device address + R)

  Temp= I2C1_Rd(1u);                     // Read the data (Acknowledge)
  Temp<<=8;
  Temp |= I2C1_Rd(1u);                   // Read the data (Acknowledge)
  LTC2943.AccumulatedCharge=Temp;
  I2C1_Stop();                           // issue I2C stop signal

  Delay_ms(50);

  I2C1_Start();                          // issue I2C start signal
  Err=I2C1_Wr(CMD_LTC2943_WRITE);        // send byte via I2C(device address + W)
  if(Err !=0){
    return Err;
  }
  I2C1_Wr(ADD_VOLTAGE_REG);               // send byte via I2C(device address + W)
  I2C1_Repeated_Start();                 // issue I2C signal repeated start
  I2C1_Wr(CMD_LTC2943_READ);             // send byte (device address + R)

  Temp= I2C1_Rd(1u);                     // Read the data (Acknowledge)
  Temp<<=8;
  Temp |= I2C1_Rd(1u);                   // Read the data (Acknowledge)
  LTC2943.Voltage =Temp;
  I2C1_Stop();
  

  //lValue=(236.0* LTC2943.Voltage)/0xFFFF;
  //LTC2943.BatteryVoltage=(unsigned int)lValue;
  
  //Added 0.12V to compensate for voltage measurement error of 0.12V with actual BATTERY voltage
  LTC2943.Voltage+=333; //333= 1.2*0xFFFF/236;
  lValue=(236.0* LTC2943.Voltage)/0xFFFF;
  LTC2943.BatteryVoltage=(unsigned int)lValue;

  

  Temp=0;
  Delay_ms(50);
  I2C1_Start();                          // issue I2C start signal
  Err=I2C1_Wr(CMD_LTC2943_WRITE);        // send byte via I2C(device address + W)
  if(Err !=0){
    //LTC2943.Current=0x0000;
    return Err;
  }
  Err=I2C1_Wr(ADD_CURRENT_REG);               // send byte via I2C(device address + W)
  if(Err !=0){
    //LTC2943.Current=0x0000;
    return Err;
  }
  I2C1_Repeated_Start();                 // issue I2C signal repeated start
  Err=I2C1_Wr(CMD_LTC2943_READ);             // send byte (device address + R)
  if(Err !=0){
    //LTC2943.Current=0x0000;
    return Err;
  }
  Temp= I2C1_Rd(1u);                     // Read the data (Acknowledge)
  Temp<<=8;
  Temp |= I2C1_Rd(1u);                   // Read the data (Acknowledge)
  LTC2943.Current=Temp;
  I2C1_Stop();

  if(LTC2943.Current>0x7FFF){
    LTC2943.Direction=CHARGING;
    LTC2943.Current-=0x7FFF;
    if(LTC2943.Current<20){
        LTC2943.Direction=DISCHARGING;
    }
    
  }else{
    LTC2943.Direction=DISCHARGING;
    LTC2943.Current=0x7FFF-LTC2943.Current;
  }
  lValue=(Rmultiplier)*(LTC2943.Current);
  lValue=(lValue/0x7FFF);
  LTC2943.BatteryCurrent=(unsigned int)lValue;


  Temp=0;
  Delay_ms(50);
  I2C1_Start();                          // issue I2C start signal
  Err=I2C1_Wr(CMD_LTC2943_WRITE);        // send byte via I2C(device address + W)
  if(Err !=0){
    //LTC2943.Temperature=0x0000;
    return Err;
  }
  I2C1_Wr(ADD_TEMPERATURE_REG);               // send byte via I2C(device address + W)
  I2C1_Repeated_Start();                 // issue I2C signal repeated start
  I2C1_Wr(CMD_LTC2943_READ);             // send byte (device address + R)

  Temp= I2C1_Rd(1u);                     // Read the data (Acknowledge)
  Temp<<=8;
  Temp |= I2C1_Rd(1u);                   // Read the data (Acknowledge)
  LTC2943.Temperature=Temp;
  I2C1_Stop();


/*I2C1_Start();                          // issue I2C start signal
  I2C1_Wr(CMD_LTC2943_WRITE);            // send byte via I2C(device address + W)

  I2C1_Wr(ADD_VOLTAGE_REG+1);               // send byte (register address)
  I2C1_Repeated_Start();                 // issue I2C signal repeated start
  I2C1_Wr(CMD_LTC2943_READ);             // send byte (device address + R)
  LTC2943.Voltage<<=8;
  LTC2943.Status |= I2C1_Rd(0u);          // Read the data (NO acknowledge)
  I2C1_Stop();*/               // issue I2C stop signal
   return SUCCESS;
 }