 /**************************************************************************************************


    FILE NAME:   Display.h

    Header file for graphic display interface

****************************************************************************************************/

/*****************************************************************************
        INCLUDE FILES
*****************************************************************************/
#include "Constants.h"

/****************************************************************************
        CONSTANTS
*****************************************************************************/
 #define IMAGE_BATTERY_SIZE    200
 #define IMAGE_BATTERY_WIDTH   51//74//50
 #define IMAGE_BATTERY_HEIGHT  14//20 //32
 #define POS_BATTERY_X_START   (39)

 #define POS_BATTERY_Y_START   (40-IMAGE_BATTERY_HEIGHT)      //(64-IMAGE_BATTERY_HEIGHT)
 #define POS_BATTERY_X_END     (POS_BATTERY_X_START+IMAGE_BATTERY_WIDTH)
 #define POS_BATTERY_Y_END     (POS_BATTERY_Y_START+IMAGE_BATTERY_HEIGHT)
 
 #define W_BATT_MENU           104
 #define H_BATT_MENU           35
 #define POS_BATT_MENU_X_START  (POS_BATTERY_X_START-9 )
 #define POS_BATT_MENU_X_END    (POS_BATT_MENU_X_START+W_BATT_MENU)
 #define POS_BATT_MENU_Y_END    (POS_BATTERY_Y_END-7) //(POS_BATT_MENU_Y_START+H_BATT_MENU)
 #define POS_BATT_MENU_Y_START  (POS_BATT_MENU_Y_END-H_BATT_MENU)

 #define POS_BATT_MENU_TXT_START  POS_BATT_MENU_X_START+65
 

#define  POS_BAR_X_START     POS_BATTERY_X_START+5//16
#define  POS_BAR_Y_START     POS_BATTERY_Y_START+3


 #define POS_BAR_START      POS_BATTERY_X_START+5//7
 #define POS_BAR_END        POS_BATTERY_X_START+IMAGE_BATTERY_WIDTH-7
 #define POS_BAR_BOTTOM     (64-4)
 
 
 #define IMAGE_SOURCE_SIZE    62
 #define IMAGE_SOURCE_WIDTH   31
 #define IMAGE_SOURCE_HEIGHT  16
 #define POS_SOURCE_X_START   2
 #define POS_SOURCE_Y_START   0
 #define POS_SOURCE_X_END     POS_SOURCE_X_START+IMAGE_SOURCE_WIDTH
 #define POS_SOURCE_Y_END     POS_SOURCE_Y_START+IMAGE_SOURCE_HEIGHT
 #define IMAGE_MPPT_WIDTH     24
 
 #define IMAGE_MPPT_WIDTH     31
 #define IMAGE_MPPT_HEIGHT    12
 #define POS_MPPT_X_START     POS_SOURCE_X_START
 #define POS_MPPT_Y_START     POS_SOURCE_Y_END+1
 
 #define POS_LOAD_X_START     0
 #define POS_LOAD_Y_START     56
 #define IMAGE_LOAD_WIDTH     38
 #define IMAGE_LOAD_HEIGHT    8
 
 
 #define W_SOURCE_MENU   112
 #define H_SOURCE_MENU   48
 #define POS_SOURCE_MENU_X_START   POS_SOURCE_X_START
 #define POS_SOURCE_MENU_Y_START   POS_SOURCE_Y_START+2
 #define POS_SOURCE_MENU_X_END     POS_SOURCE_X_START+W_SOURCE_MENU
 #define POS_SOURCE_MENU_Y_END     POS_SOURCE_Y_START+H_SOURCE_MENU
 
 
 #define IMAGE_BATT_SIZE    62
 #define IMAGE_BATT_WIDTH   31
 #define IMAGE_BATT_HEIGHT  16
 #define POS_BATT_X_START   2
 #define POS_BATT_Y_START   (64-IMAGE_BATT_HEIGHT)
 #define POS_BATT_X_END     (POS_BATT_X_START+IMAGE_BATT_WIDTH)
 #define POS_BATT_Y_END     (POS_BATT_Y_START+IMAGE_BATT_HEIGHT)

 #define POS_VOLTAGE_X_START (64-20)
 #define POS_PERCENT_X_START (64-20)
 //#define POS_PERCENT_X_START (128-40)
 //#define POS_PERCENT_Y_START 8
 #define POS_PERCENT_Y_START (POS_BATTERY_Y_START-16)
 #define POS_PERCENT_X_END   (POS_PERCENT_X_START+40)
 #define POS_PERCENT_Y_END   (POS_PERCENT_Y_START+20)


 #define IMAGE_INFO_HEIGHT   16
 #define IMAGE_INFO_WIDTH    16
 #define POS_INFO_X_START    6
 #define POS_INFO_Y_START    POS_SOURCE_Y_END+10 //64-IMAGE_INFO_HEIGHT
 #define POS_INFO_Y_END      POS_INFO_Y_START+IMAGE_INFO_WIDTH
 #define POS_INFO_X_END      POS_INFO_X_START+IMAGE_INFO_HEIGHT
 
 
 
 #define IMAGE_LED_WIDTH   31
 #define IMAGE_LED_HEIGHT  16
 #define POS_LED_X_START   128-IMAGE_LED_WIDTH
 #define POS_LED_Y_START   0
 #define POS_LED_X_END     POS_LED_X_START+IMAGE_LED_WIDTH
 #define POS_LED_Y_END     POS_LED_Y_START+IMAGE_LED_HEIGHT


 #define IMAGE_UPDN_WIDTH   16
 #define IMAGE_UPDN_HEIGHT  32
 #define POS_UPDN_X_START   128-IMAGE_UPDN_WIDTH
 #define POS_UPDN_Y_START   POS_LED_Y_END
 #define POS_UPDN_X_END     POS_UPDN_X_START+IMAGE_UPDN_WIDTH
 #define POS_UPDN_Y_END     POS_UPDN_Y_START+IMAGE_UPDN_HEIGHT
 
 
 
 


 
/****************************************************************************
        GLOBAL VARIABLES
*****************************************************************************/
extern unsigned char ChargeSize;
extern unsigned char txt[16];

extern const code char Sourceinfo_112x48[672];
extern const code char Battinfo[520];
extern const code char Time_18x24[54];
extern const code char Guage_106x48[742];
extern const code char battery_level[1024];
extern const code char BattBar_4x12[8];
extern const code char BattBar1_4x12[8];


extern const code char LargeFont80x16[11][16];


extern const code char BL1[80];
extern const code char BL2[80];
extern const code char BL3[80];
extern const code char BL4[80];
extern const code char BL5[80];
extern const code char BL6[80];
extern const code char BL7[80];
extern const code char BL8[80];
extern const code char BL9[80];
extern const code char BL10[80];

 extern const code char BLx[11][80] ;

  
 //User battery steup
 extern const code char Yes_24x16[48];
 extern const code char No_24x16[48];
 const code char InverterSetup_108x16[216];
 extern const code char UsrSetup_108x16[216];
  
  //battery types
  

  extern const code char Type_108x16[216];
  
  extern const code char AGM_44x15[];
  extern const code char LeadAcid_44x15[];
  extern const code char Lithium_44x15[];
  extern const code char GEL_44x15[];
  extern const code char Calcium_44x15[];
  extern const code char Default_44x15[];
  extern const code char Save_44x15[88];

  //Battery capacity
  extern const code char Capacity_108x16[216];
  extern const code char Plus_16x16[32];
  extern const code char Minus_16x16[32];
  extern const code char AHbox_32x16[64];
  
  extern  const code char UPDN16x32[64];
  extern  const code char BLANK_UPDN16x32[64];
  
  extern const code char LCD_DUMMY[1024];

  extern PrevMenuState;
  extern  unsigned char ChargeSize;
  extern  unsigned char ChargeFlashTimer,ChargeFlashDelay;
  extern unsigned char PercentVoltFlag;
/****************************************************************************
        FUNCTION PROTOTYPES
*****************************************************************************/


void DisplayBattery(void);
void DiplayPercentageVolts(unsigned int value,unsigned char parameter );
void DisplayBatteryDetails(unsigned char MenuState);
void DisplaySource(void);
void DisplaySourceDetails(unsigned char MenuState);
void DisplayCharging(void);
void DisplayHomeScreen(void);
void ClearSourceDisplay(void);