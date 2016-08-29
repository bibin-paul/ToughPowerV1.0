/**************************************************************************************************

    FILE NAME:   Led.h


**************************************************************************************************

    FILE NAME:   Led.h

    Header file for LED control declarations

****************************************************************************************************/

/*****************************************************************************
        INCLUDE FILES
*****************************************************************************/
#include "Constants.h"

/****************************************************************************
        CONSTANTS
*****************************************************************************/



#define LED_OFF                       0                //bit is Clear=OFF
#define LED_ON                        1                //bit is set=ON
#define LED_BLINK                     2                //flash LED

#define GREEN                         1                //for Green
#define RED                           2                //for RED
#define YELLOW                        3                //for orange
#define OFF                           0                //for blank



#define  LCD_BACKLIGHT       LATE3_bit
#define  LCD_ON_OFF          LATE1_bit
#define  LCD_BACKLIGHT_DIR   TRISE3_bit
#define  LCD_ON_OFF_DIR      TRISE1_bit


#define  FAN_CONTROL_DIR              TRISB1_bit
#define  INVERTER_RELAY_DIR           TRISB0_bit
#define  LED_STRIP_DIR                TRISC6_bit//TRISB3_bit
#define  LED_RED_DIR                  TRISB6_bit    //RED and GREEN leds are on PGD and PGC pins, will not work when programmer is connected
#define  LED_GREEN_DIR                TRISB7_bit



#define  FAN_CONTROL                  LATB1_bit
#define  INVERTER_RELAY               LATB0_bit
#define  LED_STRIP                    LATC6_bit//LATB3_bit
#define  LED_RED                      LATB6_bit
#define  LED_GREEN                    LATB7_bit



/****************************************************************************
        GLOBAL VARIABLES
*****************************************************************************/

/****************************************************************************
        FUNCTION PROTOTYPES
*****************************************************************************/


void Init_Leds(void);
void Set_Output(unsigned char led_no,unsigned char state);
void Set_StatusLed(unsigned char colour,unsigned char state);