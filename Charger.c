
/*****************************************************************************

    FILE NAME:     Charger.c

    DESCRIPTION:   Charger board initialization,measurements and control functions
    AUTHOR:        Bibin Paul
    Property of Intelliquip
*****************************************************************************/

/*****************************************************************************
        INCLUDE FILES
*****************************************************************************/
#include "Charger.h"
#include "LTC2943.h"
#include "my_string.h"
#include "Display.h"
/****************************************************************************
        GLOBAL VARIABLES
*****************************************************************************/
 _CHARGER CHARGER;
 unsigned char AdcState=0;
/****************************************************************************
        LOCAL MACROS
*****************************************************************************/
//NONE
unsigned int VOLTAGE_CHART[10]= {
115,117,118,119,121,122,124,125,126,127
};

/*const unsigned int VOLTAGE_CHART[10]= {
1151,1166,1181,1196,1210,1224,1237,1250,1262,1273
};*/


const unsigned char STATUS_MASK=0b00111000;
const unsigned char FAULT_MASK =0b00000111;

unsigned char STATUS_CHART[6]={
  0b00000000,
  0b00001000,
  0b00010000,
  0b00100000,
  0b00011000,
  0b00101000
  } ;
unsigned char FAULT_CHART[5]={
  0b00000000,
  0b00000001,
  0b00000010,
  0b00000011,
  0b00000100,
  } ;

  unsigned char* STATUS_MSG[6]={
  "TRICKLE",
  "BULK   ",
  "BOOST  ",
  "FLOAT  ",
  "FULL   ",
  "TESTING"
  };
   unsigned char* FAULT_MSG[5]={
  "NoFlts",
  "NoBatt",
  "InvTmp",
  "TmrFlt",
  "UndrVt"
  };


/****************************************************************************
        LOCAL FUNCTIONS
*****************************************************************************/
//NONE



/****************************************************************************
FUNCTION: Init_Charger

DESCRIPTION:
    Initialize the pins and registers for charger and protection ic interface

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
void Init_Charger(){


//Initialize the control and measurement pins from U1(LT8490-charger)

/*CCP5CON                   =0x00;            //Disable CCP5 as it shares MPPT pin
  RTCCFG                    =0x00;            //Disable RTCC as it shares MPPT pin
  T7CON                     =0x00;             //disable Timer7
  CM3CON                    =0x00;            //Disable comparator
  T5GCON                    =0x00;
  T5CON                     =0x00;
  RTCCMD_bit=0;
  ODCON1=0x00;
  ODCON2=0x00;
   ODCON3=0x00;*/
  
  SHUT_DN_U2                =INPUT_OFF;               //Input Off at startup
  SHUT_DN_U1                =CHARGER_OFF;             //Charger OFF
  CHARGE_MODE               =CHARGER_MODE_LEADACID;    //Mode set to Lead Acid
  MPPT_CONTROL              =MPPT_OFF;                //MPPT function OFF(check this)
  
  
  VOLT_14_6             = OFF; //OFF state by default
  VOLT_15_3             = OFF; //OFF state by default
  DIR_VOLT_14_6             = OUTPUT; //Digital OUT
  DIR_VOLT_15_3             = OUTPUT; //Digital OUT
  
  IIN_DIR                   =INPUT;  //Analog In
  IOUT__DIR                 =INPUT;  //Analog In
  MPPT_CONTROL_DIR          =OUTPUT; //Digital OUT
  SHUT_DN_U1_DIR            =OUTPUT; //Digital OUT
  FAULT_U1_DIR              =INPUT;  //Digital IN

  CHARGE_MODE_DIR           =OUTPUT; //Digital OUT

  IIN_ANALOG_SEL            =ANALOG; //Analog  IN
  IOUT_ANALOG_SEL           =ANALOG; //Analog  IN


//control and measurement pins from U1(LTC4365-Protection)
  DC_WALL_DIR               =INPUT;  //Analog In
  VIN1_DIR                  =INPUT;  //Analog In
  SHUT_DN_U2_DIR            =OUTPUT; //output
  FAULT_U2_DIR              =INPUT;  //Input

  DC_WALL_ANALOG_SEL        =ANALOG; //Analog  IN
  VIN1_ANALOG_SEL           =ANALOG; //Analog  IN

  
  
  STATUS_U1_DIR             =INPUT;  //Digital IN-UART
  UART1_Init(2400);
  //TXSTA1=0;                          //TX pin is not used
 }
/****************************************************************************
FUNCTION: UpdateAnalogInputs

DESCRIPTION:
    Reads the analog inputs for Vin,Vout Iin and Iout from the CHARGER chip LTC8490
    All parameters are updated into corresponding structures.
    

ARGUMENTS:

         Argument                     Description                                                Type
---------------------------------------------------------------------------------------------
    NONE
RETURN VALUE:

    NONE

PRE-CONDITIONS:
  Analog port init must be called before using this function
POST-CONDITIONS:
    NONE

IMPORTANT NOTES:
   THis function must be called from the main loop to update all parameters. Only one parameter is updated per call.

*****************************************************************************/
void UpdateAnalogInputs(){
unsigned long L;
 switch(AdcState){
      case 0:
        L=ADC_Get_Sample(DC_WALL_CHANNEL);
        L*=VIN_MULTIPLIER;
        L>>=7;       //L/VIN_DIVIDER
        //Glcd_Write_Text((unsigned char*)txt,0,6,1);
        CHARGER.DCWall=(unsigned int) L;
        AdcState++;
      break;
      
      
      case 1:
        L=ADC_Get_Sample(VIN1_CHANNEL);
        L*=VIN_MULTIPLIER;
        L>>=7;       //L/VIN_DIVIDER
        //Long2AsciiPad(txt,L,6,'0');
        //Glcd_Write_Text((unsigned char*)txt,70,6,1);
        CHARGER.Vin=(unsigned int) L;
        AdcState++;
      break;
 

      case 2:
        L=ADC_Get_Sample(IIN_CHANNEL);

        if(L>= IIN_OFFSET){
          L-= IIN_OFFSET;
          L*=IIN_MULTIPLIER;
          L>>=10;       //L/IIN_DIVIDER
        }else{
          L=0;
        }
        CHARGER.Iin=(unsigned int) L;
        
        //Value2Parameter(txt,L,5, 'A');
        //Glcd_Write_Text((unsigned char*)txt,0,5,1);
        AdcState++;
      break;
 
 
      case 3:
        L=ADC_Get_Sample(IOUT_CHANNEL);
        if(L>= IIN_OFFSET){
          L-= IIN_OFFSET;
          L*=IIN_MULTIPLIER;
         // This has be changed as the observed current value at Iout is double the measured current.
         //Need to check the Charger chip Iout calculations

          L>>=9;        //L/IIN_DIVIDER
        }else{
          L=0;
        }
        CHARGER.Iout=(unsigned int) L;

        AdcState=0;
      break;
      
      
      default:
        AdcState=0;
      break;
 
 }

}

/****************************************************************************
FUNCTION: VoltageTOPercentage

DESCRIPTION:
    Calculate the battery charge percentage based on the Battery Voltage at No load(Noload=~100mA)

ARGUMENTS:

         Argument                     Description                                                Type
---------------------------------------------------------------------------------------------
    NONE
RETURN VALUE:

    The battery percentage multiplied by 10 (unsigned int)


PRE-CONDITIONS:
   The battery percentage is calculated on the basis of voltage only, its necessary to let the battery voltage settle.
   UpdateLTC2943() must be called before calling this function.
POST-CONDITIONS:
    NONE

IMPORTANT NOTES:
   None

*****************************************************************************/

unsigned int VoltageToPercentage(){
unsigned int x=0;
unsigned int I=0;
  x=0;
  

  if((LTC2943.BatteryVoltage<BATT_MINIMUM_VOLTAGE)||(LTC2943.BatteryVoltage>BATT_MAXIMUM_VOLTAGE)){
     return 0;// Return 0% if the voltage is invlid
  }

  
     while( LTC2943.BatteryVoltage>VOLTAGE_CHART[x]){
       x++;
     }
     x*=10;
     return x;
}