
/*****************************************************************************

    FILE NAME:     Main.c

    DESCRIPTION:   Main functipon
    AUTHOR:        Bibin Paul
    Property of Intelliquip
*****************************************************************************/


/*****************************************************************************
        INCLUDE FILES
*****************************************************************************/
#include "Constants.h"
#include "my_string.h"
#include "LTC2943.h"
#include "Relay.h"
#include "Led.h"
#include "MCP9700.h"
#include "Charger.h"
#include "Display.h"
#include "Switch.h"
#include "Interrupt.h"


/****************************************************************************
        LOCAL MACROS
*****************************************************************************/

// Glcd module connections
char GLCD_DataPort at PORTD;

sbit GLCD_CS1 at LATB5_bit;
sbit GLCD_CS2 at LATB4_bit;
sbit GLCD_RS  at LATE7_bit;
sbit GLCD_RW  at LATE6_bit;
sbit GLCD_EN  at LATE4_bit;
sbit GLCD_RST at LATE5_bit;

sbit GLCD_CS1_Direction at TRISB5_bit;
sbit GLCD_CS2_Direction at TRISB4_bit;
sbit GLCD_RS_Direction  at TRISE7_bit;
sbit GLCD_RW_Direction  at TRISE6_bit;
sbit GLCD_EN_Direction  at TRISE4_bit;
sbit GLCD_RST_Direction at TRISE5_bit;
// End Glcd module connections



// Touch Panel module connections
sbit DriveA at LATA4_bit;
sbit DriveB at LATC1_bit;
sbit DriveA_Direction at TRISA4_bit;
sbit DriveB_Direction at TRISC1_bit;
sbit ReadX_Direction at TRISA0_bit;
sbit ReadY_Direction at TRISA1_bit;
#define ReadX_ANALOG_SEL ANSEL0_bit     //AN1 and AN0 pins as analog input for touchscreen
#define ReadY_ANALOG_SEL ANSEL1_bit

/*sbit DriveB at LATA4_bit;
sbit DriveA at LATC1_bit;
sbit DriveB_Direction at TRISA4_bit;
sbit DriveA_Direction at TRISC1_bit;
sbit ReadY_Direction at TRISA0_bit;
sbit ReadX_Direction at TRISA1_bit;
#define ReadX_ANALOG_SEL ANSEL0_bit     //AN1 and AN0 pins as analog input for touchscreen
#define ReadY_ANALOG_SEL ANSEL1_bit*/
// End Touch Panel module connections

/****************************************************************************
        GLOBAL VARIABLES
*****************************************************************************/

    bit          write_erase;
    char         pen_size;
    char write_msg[] = "WRITE";   // GLCD menu messages
    char clear_msg[] = "CLEAR";
    char erase_msg[] = "ERASE";
    unsigned int x_coord, y_coord;

    unsigned int count=0,x;
    unsigned int x_min=0, y_min=0, x_max=0, y_max=0;

    unsigned char ax=0,bx=0,cx=0,dx=0,ex=0,fx=0,gx=0,hx=0,ix=0,jx=0,kx=0,lx=0;
    unsigned char tick=10;

    float Rsense=0;
    float ChargeConst=0;
    float Rmultiplier=0;
    unsigned long int QLSB;
    unsigned long int mAH;


    unsigned char ChargeFlashTimer=4;
    unsigned char ChargeFlashDelay=4;
    
    unsigned char MainState=0;
    unsigned char MenuState=0;
    extern unsigned char LowBattStartup;
/****************************************************************************
        Function Prototypes
*****************************************************************************/
unsigned char BatteryControl(void);
void SourceControl(void);
void UserSetup();
void MasterSetup();
void GetCalibrationData(void);
void CalibrateShunt(void);
void DisplayAllData(void);
void InitializePins();
void InitTimer2();
void CalibrateScreen() ;
 
/****************************************************************************
        Main Function
*****************************************************************************/

void main() {


    unsigned char receive=0;       //serial receive data
    unsigned char Expert=0;
    unsigned char ExpertModeTimer=EXPERT_MODE_DELAY;
    //unsigned char ChargeVoltFlag=0;


    unsigned char err=0;

    unsigned char level=5;
    int w=0,x=0,y=0,z=0;
    unsigned int refresh_timer=10;
    unsigned char ret=0;
    float lValue=0;

    unsigned int V=0;
    unsigned long L=0;
    unsigned int BatteryControlWait;
    

    
    LTC2943.BattPercent=0;

     
    Init_Relays();
    
    Init_Leds();                    //Moved up here to make sure LED strip and Inverter dont turn on at startup
    InitializePins();               //Initialize pins for LCD, Touch, backlight etc

    Init_Relays();
    Init_Charger();
   // InitLTC2943();                 //Initialize the Battery monitor   moved further down, wait for latching relay ON
    InitMCP9700();
    DriveA=0;
    DriveB=0;
    DriveA_Direction=OUTPUT;
    DriveB_Direction=OUTPUT;
    CHARGER.SourceState=0;
    
    // LCD_BACKLIGHT       =OFF;

    Glcd_Fill(0);
    //Relay_Set(RELAY_ON);   moved below so that BATTERY is not turned On before confirmation

          //Glcd_Write_Text(" LOW BATTERY 1",20,0,1);
    Glcd_Write_Text("LB V1.00.002",30,2,1);

    
    BATTERY.Capacity=CAPACITY_DEFAULT;
    BATTERY.Type=TYPE_DEFAULT;
    BATTERY.DischargeConstant=DISCHARGE_CONST_DEFAULT; //(CHARGECONSTANT*100)/BATTERY.capacity;
    BATTERY.ChargeConstant=CHARGE_CONST_DEFAULT;       //(CHARGECONSTANT*100)/BATTERY.capacity;
    BATTERY.ChargingVoltage=CHARGE_VOLTAGE_DEFAULT;
    BATTERY.EmptyVoltage=EMPTY_VOLTAGE_DEFAULT;
    BATTERY.LowVoltage=LOW_VOLTAGE_DEFAULT;
    BATTERY.NormalVoltage=NORMAL_VOLTAGE_DEFAULT;
    BATTERY.CutOffDelay=BATTERY_CUT_OFF_DELAY_TIME;
    BATTERY.LoadOffDelay=LOAD_OFF_DELAY_TIME;
    
        
    InitLTC2943();                 //Initialize the Battery monitor
    LED_STRIP=OFF;
    

    

    
    Interval=10;
    PercentLED=0;
    PercentLCD=10;
    CHARGER.LED=OFF;
    InitTimer2();
    

    Delay_ms(500);
    UserSetup();

    Glcd_Fill(0);
    

    Relay_Set(RELAY_ON); //Turn on Relay after all intitialization is done
    Delay_ms(100);
    
        //Added to set the coloumb counter to default value
    err=UpdateLTC2943();
    if(err){
        InitLTC2943();
    }
    UpdateLTC2943();

    if((LTC2943.BatteryVoltage<BATT_MINIMUM_VOLTAGE)||(LTC2943.BatteryVoltage>BATT_MAXIMUM_VOLTAGE)){
      Glcd_Fill(0);
      Glcd_Write_Text("CHECK BATTERY ",25,3,1);
      Delay_ms(5000);
      LowBattStartup=1;
      BATTERY.CutOffDelay=0;
      CHARGER.Load=0;
    }


    V=VoltageToPercentage();
    LTC2943.PrevAccumulatedCharge=LTC2943.AccumulatedCharge;
    LTC2943.BattPercent=V/10;
    DisplayBattery();


    LTC2943.BatteryAh=(LTC2943.BattPercent*BATTERY.capacity)/100;

    
    
    VOLT_14_6             = 0; //OFF state by default
    VOLT_15_3             = 0; //OFF state by default
    CHARGE_MODE =CHARGER_MODE_LEADACID;   //Mode set to Lead acid by default
    
    if(BATTERY.Type==TYPE_LITHIUM){
        CHARGE_MODE =CHARGER_MODE_LITHIUM;   //Mode set to Lithium
        VOLT_15_3             = 0; //OFF state by default
        VOLT_14_6             = 1; //ON state
    }else if(BATTERY.Type==TYPE_AGM){
        VOLT_15_3             = 0; //OFF state by default
        VOLT_14_6             = 1; //ON State
    }else if(BATTERY.Type==TYPE_CALCIUM){
        VOLT_14_6             = 0; //OFF state by default
        VOLT_15_3             = 1; //ON State
    }
    


      x=0;
      cx=1;
      LTC2943.PrevBattLevel=0xff;
      LTC2943.Direction=DISCHARGING;

      Charger.ChargerState= 5;
      DisplaySource();                                  // Display the charger power source
      DisplayBattery();
      UpdateAnalogInputs();
      UpdateTemperature();
      Z=100;
      x=0;
      Expert=0;

      BatteryControlWait=1;
      ChargeSize=0;
      PercentLED=0;
      refresh_timer=SOURCE_REFRESH_DELAY;
      while(1){

             switch(MainState){

               case 0:
                   if(y++>3){
                    err=UpdateLTC2943();
                    if(err){
                        InitLTC2943();
                    }
                    BatteryControlWait=0;
                     y=0;
                   }
                   MainState++;

               break;

               case 1:

                     UpdateAnalogInputs();
                     z++;
                     SourceControl();

                     MainState++;
               break;

               case 2:
                  if(BatteryControlWait==0){
                       BatteryControl();
                       BatteryControlWait=1;
                  }
                  if(x++>200){
                       UpdateTemperature();
                         if(MenuState==0){
                               Value2Parameter(txt,Temperature1,5, 'C');
                               Glcd_Write_Text((unsigned char*)txt,65,6,1);
                               Value2Parameter(txt,Temperature2,5, 'C');
                               Glcd_Write_Text((unsigned char*)txt,98,6,1);

                                Long2AsciiPad(txt,CHARGER.SourceState,1,'0');
                                Glcd_Write_Text((unsigned char*)txt,55,6,1);
                             
                             
                           }

                       x=0;
                  }
                  MainState++;
               break;

               case 3:
                       if(z++>20){

                       }
                       MainState++;
                       break;
                         
                    case 4:
                        if(Z++>10){

                             LTC2943.BattLevel=LTC2943.BattPercent/10;

                             if(MenuState==0){
                                if(PercentVoltFlag==0){
                                   DiplayPercentageVolts(LTC2943.BattPercent,0);
                                }else{
                                  DiplayPercentageVolts(LTC2943.BatteryVoltage,1);
                                }

                                DisplaySource();                                  // Display the charger power source
                                DisplayBattery();                                 // Display battery symbol and charge percentage


                            }else if(MenuState==10){
                                if(refresh_timer==0){
                                    DisplaySourceDetails(MenuState);
                                    refresh_timer=SOURCE_REFRESH_DELAY;
                                }else{
                                    refresh_timer--;
                                }
                            }
                           z=0;
                       }
                   MainState++;


                 break;

                 case 5:
                  // DisplayCharging();                                 // Display charging symbol(animated)
                     // If data is ready, read it:
                     V=VoltageToPercentage();
                     if(V>90){
                         V=90;
                     } 
                     if(CHARGER.Load==1){
                        if(V<10){
                           V=10;
                        }
                     }else{
                         V=0;
                     }

                     LTC2943.BattPercent=V;
                     
                               
                    if (UART1_Data_Ready() == 1) {
                       receive = UART1_Read();

                       if((receive!=0x55)&&(receive!=0)){

                             for(x=0;x<6;x++){
                               if((receive&STATUS_MASK)==STATUS_CHART[x]){
                                  break;
                               }
                             }
                             if(x<=5){
                                //Glcd_Write_Text((unsigned char*)STATUS_MSG[x],85,1,1);
                                Charger.ChargerState= x;
                             }
                             if(Charger.ChargerState==0x04){
                                LTC2943.BattPercent=100;
                                LTC2943.BatteryAh=BATTERY.capacity;
                             }else{
                             //Moved outside the loop

                             }
                       }
                   }
                   MainState++;
                   
               break;


                default:
                   MainState=0;
               break;
             }
             if(FlashTimer==0){
                FlashTimer=20;
                if(MenuState==0){
                  if((Charger.ChargerState<0x04)&&(CHARGER.Source!=SOURCE_OFF)){
                       if(ChargeFlashTimer==0){
                          if(ChargeSize){
                              ChargeSize=0;
                          }else{
                              ChargeSize=1;
                          }
                          DisplayCharging();

                          
                          if(CHARGER.Iin>750){
                              ChargeFlashDelay=1;
                          }else if(CHARGER.Iin>500){
                              ChargeFlashDelay=2;
                          }else if(CHARGER.Iin>250){
                              ChargeFlashDelay=4;
                          }else{
                              ChargeFlashDelay=8;
                          }

                         ChargeFlashTimer=ChargeFlashDelay;
                       }else{
                         ChargeFlashTimer--;
                       }

                    }else{
                       ChargeSize=0;
                       //DisplayCharging();
                    }
                }
             }else{
               FlashTimer--;
             }

            if(USR_SWITCH==0){
                Delay_ms(100);
                if(USR_SWITCH==0){
                  if(jx==0){
                      jx=1;
                      CHARGER.Inverter=ON;
                      Set_Output(FAN,ON);
                      Set_Output(INVERTER,ON);

                   }else{
                      jx=0;
                      CHARGER.Inverter=OFF;
                      Set_Output(INVERTER,OFF);
                      Set_Output(FAN,OFF);
                   }
                   Delay_ms(500);
                }

             }

            if(INVERTER_SWITCH==0){
                Delay_ms(100);
                if(INVERTER_SWITCH==0){
                  if(jx==0){
                   jx=1;
                   CHARGER.Inverter=ON;
                   Set_Output(FAN,ON);
                   Set_Output(INVERTER,ON);

                  }else{
                    jx=0;
                    CHARGER.Inverter=OFF;
                    Set_Output(INVERTER,OFF);
                    Set_Output(FAN,OFF);
                  }
               //DisplayAllData();

               Delay_ms(500);
               }
            }
           if (TP_Press_Detect()&& (CHARGER.Load==ON)){


              // After a PRESS is detected read X-Y and convert it to 128x64 space
              

              
              if (TP_Get_Coordinates(&x_coord, &y_coord) == 0) {
                if(MenuState==0){

                  if((x_coord >POS_INFO_X_START) && (x_coord <POS_INFO_X_END) && (y_coord >POS_INFO_Y_START)&& (y_coord <POS_INFO_Y_END)) { 

                    MenuState=10;
                    DisplaySourceDetails(MenuState);
                    Delay_ms(1000);
                    
                  }
                  else if((x_coord >POS_PERCENT_X_START) && (x_coord <POS_PERCENT_X_END) && (y_coord >POS_PERCENT_Y_START)&& (y_coord <POS_PERCENT_Y_END)) {
                     if(PercentVoltFlag){
                        PercentVoltFlag=0;
                        DiplayPercentageVolts(LTC2943.BattPercent,0);
                     }else{
                        PercentVoltFlag=1;
                        DiplayPercentageVolts(LTC2943.BatteryVoltage,1);
                     }
                     Delay_ms(500);
                  }
                  else if((x_coord >POS_LED_X_START) && (x_coord <POS_LED_X_END) && (y_coord >POS_LED_Y_START)&& (y_coord <POS_LED_Y_END)) {
                     if((CHARGER.LED==OFF)&&(CHARGER.Load==ON)){
                        CHARGER.LED=ON;
                        Set_Output(LED_LIGHT,ON);
                        CHARGER.LedLevel=DEFAULT_LED_LEVEL;
                        PercentLED=5;         //starting at 50%
                        Interval=LED_PWM_INTERVAL;
                        //InitTimer2();
                        Glcd_PartialImage(POS_UPDN_X_START,POS_UPDN_Y_START, IMAGE_UPDN_WIDTH, IMAGE_UPDN_HEIGHT, IMAGE_UPDN_WIDTH, IMAGE_UPDN_HEIGHT, &UPDN16x32);
                     }else{
                        CHARGER.LED=OFF;
                        PercentLED=0;
                        Glcd_PartialImage(POS_UPDN_X_START,POS_UPDN_Y_START, IMAGE_UPDN_WIDTH, IMAGE_UPDN_HEIGHT, IMAGE_UPDN_WIDTH, IMAGE_UPDN_HEIGHT, &BLANK_UPDN16x32);
                        //StopTimer2();
                        Set_Output(LED_LIGHT,OFF);
                     }
                     LTC2943.PrevBattLevel=0xFF; //Just to refresh the display
                     Delay_ms(500);
                  }else if(CHARGER.LED==ON){
                      if((x_coord >POS_UPDN_X_START) && (x_coord <POS_UPDN_X_END) && (y_coord >POS_UPDN_Y_START)&& (y_coord <POS_UPDN_Y_START+16)) {
                         CHARGER.LedLevel++;
                         if(CHARGER.LedLevel>MAX_LED_LEVEL){
                            CHARGER.LedLevel=MAX_LED_LEVEL;
                         }
                         PercentLED=PercentLedPWM[CHARGER.LedLevel];
                         Delay_ms(300);
                      }else if((x_coord >POS_UPDN_X_START) && (x_coord <POS_UPDN_X_END) && (y_coord >POS_UPDN_Y_START+24)&& (y_coord <POS_UPDN_Y_END)) {
                         CHARGER.LedLevel--;
                         if(CHARGER.LedLevel<MIN_LED_LEVEL){
                            CHARGER.LedLevel=MIN_LED_LEVEL;
                         }
                         PercentLED=PercentLedPWM[CHARGER.LedLevel];
                         Delay_ms(300);
                      }
                  }
                  
                }else if(MenuState==10){
                       DisplayHomeScreen();
                       MenuState=0;
                       Delay_ms(1000);
                }
              }
            }
            
     }   //while(1)

}


/****************************************************************************
FUNCTION: BatteryControl

DESCRIPTION:
    Controls the charging and discharging of the battery depending on battery voltage,temperature,load etc

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
unsigned char BatteryControl(){
unsigned int Temp;
/*Long2AsciiPad(txt,BATTERY.CutOffDelay,3,'0');
   Glcd_Write_Text((unsigned char*)txt,0,6,1);
   Long2AsciiPad(txt,BATTERY.LoadOffDelay,3,'0');
   Glcd_Write_Text((unsigned char*)txt,0,7,1);
   */
   if(CHARGER.Load==ON){
         if(LTC2943.Direction==DISCHARGING){
              if(LTC2943.BatteryCurrent!=0){
                  Temp=LTC2943.BatteryCurrent>>10;     //divide by 1000
              }
              BATTERY.EmptyVoltage=EMPTY_VOLTAGE_DEFAULT-Temp;
              if(BATTERY.EmptyVoltage<EMPTY_VOLTAGE_MINIMUM){
                    BATTERY.EmptyVoltage=EMPTY_VOLTAGE_MINIMUM;
              }

         }else{
              BATTERY.EmptyVoltage=EMPTY_VOLTAGE_DEFAULT;
         }

         if(LTC2943.BatteryVoltage <= BATTERY.EmptyVoltage){
             if(BATTERY.CutOffDelay){
                 BATTERY.CutOffDelay--;
             }else{
                //remove in actual code, this is for test code only!!!!!!!!!!!!!!!!!!!!!!
                //Make changes to low voltage cutoff logic rather than simple low voltage level. Need fix for invertr ON
                Set_Output(INVERTER,OFF);       //cutoff everything. Turn off even if they are already off(saves code)
                CHARGER.Inverter=OFF;
                CHARGER.SourceState=0;
              //  Glcd_Write_Text("Low batt:InvOff ",50,6,1);
                Delay_ms(2000);


                Relay_Set(RELAY_OFF);     //Turn off all the loads
                Set_Output(FAN,OFF);
                Set_Output(LED_STRIP,LED_OFF);
                Glcd_PartialImage(POS_UPDN_X_START,POS_UPDN_Y_START, IMAGE_UPDN_WIDTH, IMAGE_UPDN_HEIGHT, IMAGE_UPDN_WIDTH, IMAGE_UPDN_HEIGHT, &BLANK_UPDN16x32);

                CHARGER.Load=OFF;
                CHARGER.LED=OFF;
                PercentLED=0;

                LTC2943.BattPercent=0;
                LTC2943.BattLevel=0;
                LTC2943.PrevBattLevel=0xFF;
                LTC2943.BatteryAh=0;
                LTC2943.BatteryCurrent=0;

                UpdateLTC2943();
                UpdateAnalogInputs();
                Set_StatusLed(RED,ON);

                BATTERY.CutOffDelay=BATTERY_CUT_OFF_DELAY_TIME;
                CHARGER.SourceState=0;
                

             }
         }
         else if(LTC2943.BatteryVoltage >= BATTERY.NormalVoltage){

            LowBattStartup=0;

            //CHARGER.Load=ON;
            //Relay_Set(RELAY_ON);
            BATTERY.CutOffDelay=BATTERY_CUT_OFF_DELAY_TIME;
            BATTERY.LoadOffDelay=LOAD_OFF_DELAY_TIME;

               if((Temperature1>TEMP_LOAD_OFF)||(Temperature2>TEMP_LOAD_OFF)){
                  Set_Output(FAN,ON);
                  SHUT_DN_U1=CHARGER_OFF;
                  Set_StatusLed(RED,ON);
               }else{
                   Set_StatusLed(GREEN,ON);
                   if((Temperature1<TEMP_LOAD_ON)&&(Temperature2<TEMP_LOAD_ON)){
                       SHUT_DN_U1=CHARGER_ON;
                   }
                   if((Temperature1>TEMP_FAN_ON)||(Temperature2>TEMP_FAN_ON)){
                      Set_Output(FAN,ON);
                   }else if((Temperature1<TEMP_FAN_OFF)&&(Temperature2<TEMP_FAN_OFF)){
                      if(CHARGER.Inverter==OFF ){ //Turn off fan only if inverter is off
                         Set_Output(FAN,OFF);
                      }

                   }
               }


         }
     }else{   //If load is OFF
        if(CHARGER.SourceState >= 11){
            Glcd_Fill(0);
            Long2AsciiPad(txt,CHARGER.SourceState,2,'0');
            Glcd_Write_Text((unsigned char*)txt,40,6,1);
            //Glcd_Write_Text("ON",100,6,1);
            CHARGER.Load=ON;
            Relay_Set(RELAY_ON);
            Delay_ms(500);
            SourceControl();
            Delay_ms(500);
            CHARGER.Delay=0;
            SourceControl();

        }else{
            CHARGER.Load=OFF;

        }
     
     
     }
}

/****************************************************************************
FUNCTION: SourceControl

DESCRIPTION:
    Controls the input voltage source. controls MPPT for solar

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
 void SourceControl(){
 
   switch(CHARGER.SourceState){
         case 0:
             CHARGER.Source=SOURCE_OFF;
             SHUT_DN_U2 =INPUT_OFF;
             SHUT_DN_U1=CHARGER_OFF;
             ax=0;    //remove in actual code
             bx=0;    //remove in actual code
             dx=0;
             MPPT_CONTROL=MPPT_OFF;
             CHARGER.Delay=10;
             CHARGER.SourceState=1;
             LTC2943.PrevDirection==0xff;
             if(MenuState==0){
                 ClearSourceDisplay();
             }
        break;
        case 1:
             if(CHARGER.DCWall>VOLT_MAINS_LOW){
                 CHARGER.SourceState=10;
             }else{
                CHARGER.SourceState=2;
             }
         break;
         case 2:  //For Car and Solar inputs, U2 must be enabled before measuring input voltage
            SHUT_DN_U2 =INPUT_ON;
            bx=1;    //remove in actual code
            CHARGER.SourceState=3;
            CHARGER.Delay=10;
         break;
         
         case 3: //Add a delay here for input voltage to settle( also all samples are not read in one go)
            if (CHARGER.Delay==0){
                 CHARGER.SourceState=4;
            }else{
                 CHARGER.Delay--;
            }


         break;
        
         case 4:
           if(CHARGER.Vin>VOLT_SOLAR_LOW){
                CHARGER.SourceState=20;
                MPPT_CONTROL=MPPT_ON;
                dx=1;
                CHARGER.Source=SOURCE_SOLAR;
           }else{
               CHARGER.SourceState=5;
           }
         
         break;
         
         
         case 5:
            if(CHARGER.Vin>VOLT_CAR_LOW){  //If voltage is above voltage level switch to car mode
              CHARGER.Delay=10;
              CHARGER.SourceState=31;
            } else{
              CHARGER.SourceState=0;
            }
         
         break;

         //Mains wall adaptor mode
         case 10:   //Add a delay here for input voltage to settle
             if (CHARGER.Delay==0){
                SHUT_DN_U2=INPUT_ON;
                CHARGER.Delay=5;
                bx=1;    //remove in actual code
                CHARGER.SourceState=11;
             }else{
                 CHARGER.Delay--;
            }
         break;
         case 11:
              if (CHARGER.Delay==0){
                 SHUT_DN_U1=CHARGER_ON;
                 CHARGER.Delay=10;
                 ax=1;                  //remove in atual code
                 CHARGER.SourceState=12;
                 CHARGER.Source=SOURCE_MAINS;
               }else{
                CHARGER.Delay--;
               }
         break;

         case 12:
              if(CHARGER.DCWall<VOLT_MAINS_LOW){
                CHARGER.Source=SOURCE_OFF;
                CHARGER.SourceState=0;
              }

         break;
         
         
         //solar mode
         case 20:    //Add a delay here for input voltage to settle
             if (CHARGER.Delay==0){
                 CHARGER.SourceState=21;
                 SHUT_DN_U1=CHARGER_ON;
                 CHARGER.Delay=30;
                 ax=1;                  //remove in atual code
             }else{
                 CHARGER.Delay--;
                 if(CHARGER.DCWall>VOLT_MAINS_LOW){   //If mains is available switch to mains
                    CHARGER.SourceState=0;
                 }
            }
         
         break;
         case 21:
           if (CHARGER.Delay==0){
                 CHARGER.SourceState=22;
                 MPPT_CONTROL=MPPT_ON;
                 CHARGER.Delay=250;
             }else{
                 CHARGER.Delay--;
                 if(CHARGER.DCWall>VOLT_MAINS_LOW){   //If mains is available switch to mains
                    CHARGER.SourceState=0;
                 }
            }
         break;
         
         
         
         case 22:
              if(CHARGER.DCWall>VOLT_MAINS_LOW){   //If mains is available switch to mains
                    CHARGER.SourceState=0;
              }else if(CHARGER.Iin<AMPS_SOLAR_LOW){
                    if(CHARGER.Delay==0){;
                        CHARGER.SourceState=0;
                    } else{
                          CHARGER.Delay--;
                    }
              }else{
                    CHARGER.Delay=250;
              }
         break;
         
            //Car charger mode
         case 30:
            CHARGER.SourceState=31;
         break;
         case 31:
          if (CHARGER.Delay==0){
                CHARGER.SourceState=32;
                 SHUT_DN_U1=CHARGER_ON;
                 CHARGER.Source=SOURCE_CAR;
                 CHARGER.Delay=100;
                 ax=1;                  //remove in atual code
             }else{
                 CHARGER.Delay--;
                 if(CHARGER.DCWall>VOLT_MAINS_LOW){
                      CHARGER.SourceState=10;
                 }
            }
         break;
         case 32:
            if(CHARGER.Delay==0){
              CHARGER.SourceState=33;
            }else{
                 CHARGER.Delay--;
                 if(CHARGER.DCWall>VOLT_MAINS_LOW){
                      CHARGER.SourceState=10;
                 }
            }
         
         break;
         case 33:
            if(CHARGER.DCWall>VOLT_MAINS_LOW){   //If mains is available switch to mains
              CHARGER.SourceState=0;
            }else if(CHARGER.Vin<VOLT_CAR_LOW){  //If car input voltage is too low, check for other inputs
              CHARGER.SourceState=0;
            }

         break;

         default:
             CHARGER.SourceState=0;
         break;

   }


 }

void UserSetup(){
      unsigned char Capacity=100;
      unsigned long UserTimeout=620;
     //Retrieve calibration data from memory
      GetCalibrationData();



       Glcd_Fill(0);
       Glcd_PartialImage(10,0, 108, 16, 108, 16, &UsrSetup_108x16);
       Glcd_Write_Text((unsigned char*)"Did you change the ",0,3,1);
       Glcd_Write_Text((unsigned char*)"Battery? ",0,4,1);
       Glcd_PartialImage(10,48, 24, 16, 24, 16, &No_24x16);
       Glcd_PartialImage(94,48, 24, 16, 24, 16, &Yes_24x16);
       while(1){
         if (TP_Press_Detect()) {

            // After a PRESS is detected read X-Y and convert it to 128x64 space
            if (TP_Get_Coordinates(&x_coord, &y_coord) == 0) {                                          //NO
                if((x_coord >10) && (x_coord <(10+24)) && (y_coord >48)&& (y_coord <(48+16))) {
                  Glcd_Fill(0);
                  Glcd_PartialImage(10,48, 24, 16, 24, 16, &No_24x16);
                  return;
                }else if((x_coord >94) && (x_coord <(94+24)) && (y_coord >48)&& (y_coord <(48+16))) {   //Yes
                  Glcd_Fill(0);
                  Glcd_PartialImage(94,48, 24, 16, 24, 16, &Yes_24x16);
                  break;
                }
             }

        }else{ 
              Delay_ms(5);
              if(UserTimeout){
                     UserTimeout--;
              }else{
                  Glcd_Fill(0);
                  Glcd_PartialImage(10,48, 24, 16, 24, 16, &No_24x16);
                  return;
              }
        }

      }
      Delay_ms(500);
     
     
     Glcd_Fill(0);
     Glcd_PartialImage(10,0, 108, 16, 108, 16, &Type_108x16);

     Glcd_PartialImage(10,16, 44, 15, 44, 15, &AGM_44x15);
     Glcd_PartialImage(73,16, 44, 15, 44, 15, &GEL_44x15);
     Glcd_PartialImage(10,32, 44, 15, 44, 15, &LeadAcid_44x15);
     Glcd_PartialImage(73,32, 44, 15, 44, 15, &Calcium_44x15);
     Glcd_PartialImage(10,48, 44, 15, 44, 15, &Lithium_44x15);
     Glcd_PartialImage(73,48, 44, 15, 44, 15, &Default_44x15);
     
     while(1){
     if (TP_Press_Detect()) {

        // After a PRESS is detected read X-Y and convert it to 128x64 space
        if (TP_Get_Coordinates(&x_coord, &y_coord) == 0) {                                          //AGM
            if((x_coord >10) && (x_coord <(10+44)) && (y_coord >16)&& (y_coord <(16+15))) {
              Glcd_Fill(0);
              Glcd_PartialImage(10,16, 44, 15, 44, 15, &AGM_44x15);
              BATTERY.type=TYPE_AGM;
              break;
            }else if((x_coord >73) && (x_coord <(73+44)) && (y_coord >16)&& (y_coord <(16+15))) {      //GEL
              Glcd_Fill(0);
              Glcd_PartialImage(73,16, 44, 15, 44, 15, &GEL_44x15);
              BATTERY.type=TYPE_GEL;
              break;
            }else if((x_coord >10) && (x_coord <(10+44)) && (y_coord >32)&& (y_coord <(32+15))) {       //Lead Acid
              Glcd_Fill(0);
              Glcd_PartialImage(10,32, 44, 15, 44, 15, &LeadAcid_44x15);
              BATTERY.type=TYPE_LEAD;
              break;
            }else if((x_coord >73) && (x_coord <(73+44)) && (y_coord >32)&& (y_coord <(32+15))) {      //Calcium
              Glcd_Fill(0);
              Glcd_PartialImage(73,32, 44, 15, 44, 15, &Calcium_44x15);
              BATTERY.type=TYPE_CALCIUM;
              break;
            }if((x_coord >10) && (x_coord <(10+44)) && (y_coord >48)&& (y_coord <(48+15))) {            //Lithium
              Glcd_Fill(0);
              Glcd_PartialImage(10,48, 44, 15, 44, 15, &Lithium_44x15);
              BATTERY.type=TYPE_LITHIUM;
              break;
            }else if((x_coord >73) && (x_coord <(73+44)) && (y_coord >48)&& (y_coord <(48+15))) {      //Default
              Glcd_Fill(0);
              Glcd_PartialImage(73,48, 44, 15, 44, 15, &Default_44x15);
              BATTERY.type=TYPE_LEAD;
              break;
            }
            
        }
     }
     
     }
     
     
     
     

     Delay_ms(500);
     
     Glcd_Fill(0);
     Glcd_PartialImage(10,0, 108, 16, 108, 16, &Capacity_108x16);
     Glcd_PartialImage(27,27, 16, 16, 16, 16, &Minus_16x16);
     Glcd_PartialImage((27+16+5),27, 32, 16, 32, 16, &AHbox_32x16);
     Glcd_PartialImage((27+16+5+32+5),27, 16, 16, 16, 16, &Plus_16x16);
     
     Glcd_PartialImage(10,48, 44, 15, 44, 15, &Default_44x15);
     Glcd_PartialImage(73,48, 44, 15, 44, 15, &Save_44x15);
     Long2AsciiPad(txt,Capacity,3,'0');
     Glcd_Write_Text((unsigned char*)txt,55,4,2);
     
     while(1){
     if (TP_Press_Detect()) {

        // After a PRESS is detected read X-Y and convert it to 128x64 space
        if (TP_Get_Coordinates(&x_coord, &y_coord) == 0) {                                          //Plus
            if((x_coord >27) && (x_coord <(27+16)) && (y_coord >24)&& (y_coord <(24+16))) {

               Capacity-=10;
               if(Capacity<60)Capacity=60;
               Long2AsciiPad(txt,Capacity,3,'0');
               Glcd_Write_Text((unsigned char*)txt,55,4,1);
               Delay_ms(500);
            }else if((x_coord >(27+16+5+32+5)) && (x_coord <(27+16+5+32+5+16)) && (y_coord >24)&& (y_coord <(24+16))) {      //Minus
               Capacity+=10;
               if(Capacity>160)Capacity=160;
               Long2AsciiPad(txt,Capacity,3,'0');
               Glcd_Write_Text((unsigned char*)txt,55,4,1);
               Delay_ms(500);

            }else if((x_coord >10) && (x_coord <(10+44)) && (y_coord >48)&& (y_coord <(48+15))) {       //Default
               Capacity=CAPACITY_DEFAULT;
               Long2AsciiPad(txt,Capacity,3,'0');
               Glcd_Write_Text((unsigned char*)txt,55,4,1);
               Delay_ms(500);
               //Glcd_Fill(0);
               //Glcd_PartialImage(10,48, 44, 15, 44, 15, &Default_44x15);
               //Delay_ms(500);
              // break;
            }else if((x_coord >73) && (x_coord <(73+44)) && (y_coord >48)&& (y_coord <(48+15))) {      //Save
              Glcd_Fill(0);
              Glcd_PartialImage(73,48, 44, 15, 44, 15, &Save_44x15);
              Delay_ms(500);
              

              break;
            }

        }
     }

     }

     BATTERY.Capacity=Capacity;
    /*BATTERY.DischargeConstant=(CHARGECONSTANT*100)/BATTERY.Capacity;
     BATTERY.ChargeConstant=(CHARGECONSTANT*100)/BATTERY.Capacity;*/
     
     
     //Delay_ms(4000);
     
}

void MasterSetup(){

    //Retrieve calibration data from memory
    GetCalibrationData();
    //select battery type next

}

void GetCalibrationData(){

  unsigned char ret=0;
  float fValue=0;
  unsigned long   lValue=0;
   ret=EEPROM_Read(0x0000);
    if(ret!=0xBB){
       CalibrateScreen();
       TP_Get_Calibration_Consts(&x_min, &x_max, &y_min, &y_max);

       ret=x_min>>8;
       EEPROM_Write(0x0010,ret);
       ret=(x_min&0x00FF);
       EEPROM_Write(0x0011,ret);

       ret=x_max>>8;
       EEPROM_Write(0x0012,ret);
       ret=(x_max&0x00FF);
       EEPROM_Write(0x0013,ret);

       ret=y_min>>8;
       EEPROM_Write(0x0014,ret);
       ret=(y_min&0x00FF);
       EEPROM_Write(0x0015,ret);

       ret=y_max>>8;
       EEPROM_Write(0x0016,ret);
       ret=(y_max&0x00FF);
       EEPROM_Write(0x0017,ret);

       EEPROM_Write(0x0000,0xBB);


    }else{
        ret=EEPROM_Read(0x0010);
        x_min=ret;
        x_min<<=8;
        ret=EEPROM_Read(0x0011);
        x_min|=ret;


        ret=EEPROM_Read(0x0012);
        x_max=ret;
        x_max<<=8;
        ret=EEPROM_Read(0x0013);
        x_max|=ret;

        ret=EEPROM_Read(0x0014);
        y_min=ret;
        y_min<<=8;
        ret=EEPROM_Read(0x0015);
        y_min|=ret;

        ret=EEPROM_Read(0x0016);
        y_max=ret;
        y_max<<=8;
        ret=EEPROM_Read(0x0017);
        y_max|=ret;
        TP_Set_Calibration_Consts(x_min, x_max, y_min, y_max);
    }
    
     ret=EEPROM_Read(0x0030);
    if(ret!=0x55){
        CalibrateShunt();

    }else{
        ret=EEPROM_Read(0x0020);
        Rsense=ret;
        Rsense*=0xff;
        ret=EEPROM_Read(0x0021);
        Rsense+=ret;
        
        
        Rmultiplier=60000000.0/Rsense;
        Rmultiplier*=10;
        fValue=(5000000/Rsense);
        fValue*=0.340;
        fValue/=4;
        BATTERY.chargeconstant= (unsigned int) (1000/fValue);
        BATTERY.dischargeconstant= BATTERY.chargeconstant;
        
        
        ret=EEPROM_Read(0x0028);
        CHARGER.InverterType=ret;
        
        
    }
}



  void CalibrateShunt(){
  unsigned char bytes;
  float fValue=0;
  unsigned long   lValue=0;
  
    Glcd_Fill(0);
    
    Glcd_PartialImage(10,0, 108, 16, 108, 16, &InverterSetup_108x16);
    Glcd_Write_Text((unsigned char*)"Select Inverter",0,3,1);
    Glcd_Write_Text((unsigned char*)"Set to 1000Watts? ",0,4,1);
    Glcd_PartialImage(10,48, 24, 16, 24, 16, &No_24x16);
    Glcd_PartialImage(94,48, 24, 16, 24, 16, &Yes_24x16);
    while(1){
    if (TP_Press_Detect()) {

        // After a PRESS is detected read X-Y and convert it to 128x64 space
        if (TP_Get_Coordinates(&x_coord, &y_coord) == 0) {                                          //NO
            if((x_coord >10) && (x_coord <(10+24)) && (y_coord >48)&& (y_coord <(48+16))) {
              Glcd_Fill(0);
              Glcd_PartialImage(10,48, 24, 16, 24, 16, &No_24x16);
               CHARGER.InverterType=0x01;
               EEPROM_Write(0x0028,0x01);
               break;

            }else if((x_coord >94) && (x_coord <(94+24)) && (y_coord >48)&& (y_coord <(48+16))) {   //Yes
              Glcd_Fill(0);
              Glcd_PartialImage(94,48, 24, 16, 24, 16, &Yes_24x16);
              CHARGER.InverterType=0x02;
              EEPROM_Write(0x0028,0x02);
              break;
            }
         }
     }
   }
  
  
  
    Glcd_Fill(0);
    Glcd_PartialImage(10,0, 108, 16, 108, 16, &UsrSetup_108x16);
    Glcd_Write_Text((unsigned char*)"Set load to 10.0Amps ",0,3,1);
    Glcd_Write_Text((unsigned char*)"Start calibration? ",0,4,1);
    Glcd_PartialImage(10,48, 24, 16, 24, 16, &No_24x16);
    Glcd_PartialImage(94,48, 24, 16, 24, 16, &Yes_24x16);
    while(1){
    if (TP_Press_Detect()) {

        // After a PRESS is detected read X-Y and convert it to 128x64 space
        if (TP_Get_Coordinates(&x_coord, &y_coord) == 0) {                                          //NO
            if((x_coord >10) && (x_coord <(10+24)) && (y_coord >48)&& (y_coord <(48+16))) {
              Glcd_Fill(0);
              Glcd_PartialImage(10,48, 24, 16, 24, 16, &No_24x16);


              Rsense=SHUNT;
              Rmultiplier=R_MULTIPLER;



              return;
            }else if((x_coord >94) && (x_coord <(94+24)) && (y_coord >48)&& (y_coord <(48+16))) {   //Yes
              Glcd_Fill(0);
              Glcd_PartialImage(94,48, 24, 16, 24, 16, &Yes_24x16);
              break;
            }
         }
     }
   }

    Delay_ms(1000);
    UpdateLTC2943();
    Delay_ms(1000);
    UpdateLTC2943();

    //iBATT=(60mV/RSENSE)*(iHEX/32767)    ->    RSENSE= (60/10Amps)* LTC2943.Current/32767
    fValue= LTC2943.Current;                   //1039  for 10Amps
    fValue*=600000;                             //Multiplied by 60 and then by 1000 to clear decimal points
    fValue/=32767;
    Rsense=fValue;    //(6000000.0/fValue);
    //lValue=(6000000.0/Rsense)*(LTC2943.Current);
     Rmultiplier=60000000.0/fValue;
     Rmultiplier*=10;


    //qLSB =0.340mA*(50mOhm/RSENSE)*(M/4096)

      lValue= (unsigned long) Rsense;

      Long2AsciiPad(txt,lValue,5,'0');
      Glcd_Write_Text((unsigned char*)"R= ",0,0,1);
      Glcd_Write_Text((unsigned char*)txt,30,0,1);

      fValue=(5000000/Rsense);

      lValue= (unsigned long) fValue;

      Long2AsciiPad(txt,lValue,5,'0');
      Glcd_Write_Text((unsigned char*)"5m/R=",0,1,1);
      Glcd_Write_Text((unsigned char*)txt,30,1,1);

      fValue*=0.340;

      lValue= (unsigned long) fValue;

      Long2AsciiPad(txt,lValue,5,'0');
      Glcd_Write_Text((unsigned char*)"x3.4=",0,2,1);
      Glcd_Write_Text((unsigned char*)txt,30,2,1);

       fValue/=4;

      lValue= (unsigned long) fValue;

      Long2AsciiPad(txt,lValue,5,'0');
      Glcd_Write_Text((unsigned char*)"QLSB=",0,3,1);
      Glcd_Write_Text((unsigned char*)txt,30,3,1);



      BATTERY.chargeconstant= (unsigned int) (1000/fValue);
      BATTERY.dischargeconstant= BATTERY.chargeconstant;

      Long2AsciiPad(txt,BATTERY.chargeconstant,5,'0');
      Glcd_Write_Text((unsigned char*)"CONT=",0,4,1);
      Glcd_Write_Text((unsigned char*)txt,30,4,1);


    Long2AsciiPad(txt,LTC2943.Current,5,'0');
    Glcd_Write_Text((unsigned char*)txt,0,5,1);



    lValue= (unsigned long) Rsense;

    Long2AsciiPad(txt,lValue,5,'0');
    Glcd_Write_Text((unsigned char*)txt,60,5,1);   //3153


    Delay_ms(5000);
    bytes=lValue>>8;
    EEPROM_Write(0x0020,bytes);
    bytes=(lValue&0x00FF);
    EEPROM_Write(0x0021,bytes);

    lValue= (unsigned long) Rmultiplier;
    bytes=lValue>>8;
    EEPROM_Write(0x0022,bytes);
    bytes=(lValue&0x00FF);
    EEPROM_Write(0x0023,bytes);








    EEPROM_Write(0x0030,0x55);
    Delay_ms(5000);
  }

 void CalibrateScreen() {

    Glcd_Dot(0,63,1);                                 // Draw bottom left dot
    Glcd_Write_Text("TOUCH BOTTOM LEFT",12,3,1);

    TP_Calibrate_Bottom_Left();                      // Calibration of bottom left corner
    Delay_ms(1000);

    Glcd_Dot(0,63,0);                                // Clear bottom left dot
    Glcd_Dot(127,0,1);                               // Draw upper right dot
    Glcd_Write_Text("                 ",12,3,1);
    Glcd_Write_Text("TOUCH UPPER RIGHT",12,4,1);
    TP_Calibrate_Upper_Right();                      // Calibration of upper right corner

    Delay_ms(1000);
}




void InitializePins() {

      ANCON0=0;   //All Analog pins are configured as Analog at startup, change all to digital.
      ANCON1=0;
      ANCON2=0;

      // Configure touchscreen read pins(AN1 and AN0) as analog and as input pins
      ReadX_ANALOG_SEL= ANALOG;  //Analog  IN
      ReadY_ANALOG_SEL= ANALOG;  //Analog  IN

      ReadX_Direction = INPUT;   // Configure AN0 and AN1 pins as input
      ReadY_Direction = INPUT;

      DriveA_Direction=OUTPUT;    //Set drive pins as output pins
      DriveB_Direction=OUTPUT;

      //Set LCD Power and LCD Backlight pins as output, Turn On lcd backlight and lcd power
      LCD_BACKLIGHT_DIR   =OUTPUT;
      LCD_ON_OFF_DIR      =OUTPUT;

      TRISB3_bit=0;

      LCD_BACKLIGHT       =ON;         //LCD backlight and power ON
      LCD_ON_OFF          =ON;
      GLCD_RST_Direction=0;
      GLCD_RST=0;
      Delay_ms(100);
      GLCD_RST=1;


    //  LCD_BACKLIGHT_DIR=1;
     // PWM9_Init(1000);                 //Init CCP9 in pwm mode
    //  CCP9MD_bit=1;                    //Enable CCP9 module
     // CCP9OD_bit=0;                    //disable open drain configuration on CCP9(PWM9-LCD backlight)
     // PWM1_Set_Duty(0);
     // PWM9_Start();


      InitSwitch();

      Glcd_Init();                                     // Initialize GLCD
      Glcd_Fill(0);                                    // Clear GLCD

      CON_bit = 0;
      ADC_Init();                                      // Initialize ADC

      ADCON1=0x00;   //0x30                                  //Change the Positive reference voltage to internal4.96V
      ADCON2=0xBE;
      TP_Init(128, 64, 1, 0);      //TP_Init(128, 64, 1, 0);                         // Initialize touch panel
      TP_Set_ADC_Threshold(4000);                       // Set touch panel ADC threshold



     TRISA2_bit=0;         //voltage selection pins
     TRISA3_bit=0;


}


void delay2S(){                                    // 2 seconds delay function
  Delay_ms(2000);
}



//Place/Copy this part in declaration section
void InitTimer2(){
  T2CON          = 0x3C;
  TMR2IE_bit     = 1;
  PR2            = 249;
  INTCON         = 0xC0;
}

void StopTimer2(){
   TMR2ON_bit=0;
}

