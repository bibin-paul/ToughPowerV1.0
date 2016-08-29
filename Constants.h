/**************************************************************************************************


    FILE NAME:   Constants.h

    Header file for constants(macros)

****************************************************************************************************/


/****************************************************************************
        CONSTANTS
*****************************************************************************/

 #define INPUT                               1 //Pin is selected as input by setting it to 1
 #define OUTPUT                              0 //Pin is selected as output by setting it to 0
 #define ANALOG                              1 //Analog selection bit is set to 1 for Analog and 0 for digital
 #define DIGITAL                             0 //Set as digital in or out pin
 
 #define MPPT_ON                             0 //Enable the MPPT function on the charger IC
 #define MPPT_OFF                            1 //Disable the MPPT function
 
 #define CHARGER_ON                          0 //Enable the charger
 #define CHARGER_OFF                         1 //Disable the charger
 
 #define SOURCE_OFF                          0 //Power source is Off, not charging
 #define SOURCE_MAINS                        1 //Power source is wall adaptor
 #define SOURCE_SOLAR                        2 //Power source is Solar panel
 #define SOURCE_CAR                          3 //Power source is 12V car adaptor

 
 #define INPUT_ON                            0 //Enable the input voltage to Charger
 #define INPUT_OFF                           1 //Disable the input voltage to Charger
 
 
 #define SHUNT                               2000 //1761   //1287 //0.2mOhm=200uOhm
 #define R_MULTIPLER                         19904  //Default R multiplier value
 #define LTC5AMPS                            3      //5amp equivalent of LTC hex value 5*0xFFFF/60
 #define CHARGECONSTANT                      113     //qLSB =0.34mAh•50mÙ/RSENSE•M/4096   1000/qLSB=113
 
 
 #define CHARGER_MODE_LITHIUM                0 //Charger configured to charge Lithium-ion batteries (2 stage)
 #define CHARGER_MODE_LEADACID               1 //Charger configured to charge Lead-Acid batteries   (3 Stage)

 #define TYPE_LEAD                           0 //Battery type is Lead Acid
 #define TYPE_AGM                            1 //Battery type is AGM
 #define TYPE_GEL                            2 //Battery type is GEL
 #define TYPE_LITHIUM                        3 //Battery type is LITHIUM
 #define TYPE_CALCIUM                        4 //Battery type is Calcium
 #define TYPE_DEFAULT                        TYPE_LEAD  //Battery type is Laed acid if its not set(default)
 
 #define CHARGE_VOLTAGE_LEAD                 138       //Charging voltage : x10Volts
 #define CHARGE_VOLTAGE_AGM                  138       //Charging voltage : x10Volts
 #define CHARGE_VOLTAGE_GEL                  138       //Charging voltage : x10Volts
 #define CHARGE_VOLTAGE_LITHIUM              138       //Charging voltage : x10Volts
 #define CHARGE_VOLTAGE_CALCIUM              138       //Charging voltage : x10Volts

 #define NORMAL_VOLTAGE_LEAD                 123       //Charging voltage : x10Volts
 #define NORMAL_VOLTAGE_AGM                  123       //Charging voltage : x10Volts
 #define NORMAL_VOLTAGE_GEL                  123       //Charging voltage : x10Volts
 #define NORMAL_VOLTAGE_LITHIUM              123       //Charging voltage : x10Volts
 #define NORMAL_VOLTAGE_CALCIUM              123       //Charging voltage : x10Volts

 #define EMPTY_VOLTAGE_LEAD                  108        //Low cut of voltage : x10Volts
 #define EMPTY_VOLTAGE_AGM                   108        //Low cut of voltage : x10Volts
 #define EMPTY_VOLTAGE_GEL                   108        //Low cut of voltage : x10Volts
 #define EMPTY_VOLTAGE_LITHIUM               108        //Low cut of voltage : x10Volts
 #define EMPTY_VOLTAGE_CALCIUM               108        //Low cut of voltage : x10Volts
 
 
 #define LOW_VOLTAGE_LEAD                  112        //Low cut of voltage : x10Volts
 #define LOW_VOLTAGE_AGM                   112        //Low cut of voltage : x10Volts
 #define LOW_VOLTAGE_GEL                   112        //Low cut of voltage : x10Volts
 #define LOW_VOLTAGE_LITHIUM               112        //Low cut of voltage : x10Volts
 #define LOW_VOLTAGE_CALCIUM               112        //Low cut of voltage : x10Volts

 
 
 #define CAPACITY_DEFAULT                    100        //100AH by default
 #define DISCHARGE_CONST_DEFAULT             ((CHARGECONSTANT*100)/CAPACITY_DEFAULT)   //(CHARGECONSTANT*100)/BATTERY.capacity;
 #define CHARGE_CONST_DEFAULT                ((CHARGECONSTANT*100)/CAPACITY_DEFAULT)   //(CHARGECONSTANT*100)/BATTERY.capacity;
 
 #define EMPTY_VOLTAGE_MINIMUM               95
 #define CHARGE_VOLTAGE_DEFAULT              CHARGE_VOLTAGE_LEAD
 #define NORMAL_VOLTAGE_DEFAULT              NORMAL_VOLTAGE_LEAD
 #define LOW_VOLTAGE_DEFAULT                 LOW_VOLTAGE_LEAD
 #define EMPTY_VOLTAGE_DEFAULT               EMPTY_VOLTAGE_LEAD

  
 #define RELAY_ON                            1 //Turn On Relay
 #define RELAY_OFF                           0 //Turn Off Relay

 #define ON                                  1 //Turn On
 #define OFF                                 0 //Turn Off
 
 
 #define DISCHARGING                         0 //Battery Current direction: Discharging(when In current<Out current)
 #define CHARGING                            1 //Battery Current direction: Charging(when In current>Out current)
 
 #define FAN                                 1 //FAN out put number:Internal fan is driven from driver o/p 1
 #define INVERTER                            2 //Inverter power on/off
 #define LED_LIGHT                           3 //LED light Strip out put number:Front light is driven from driver o/p 3


 
 #define TEMP_FAN_ON                         400    //temperature high value, Cooling fan starts to operate at this temperature
 #define TEMP_FAN_OFF                        350    //temperature Normal upper value, Cooling fan stops on or below this temperature
 #define TEMP_LOAD_OFF                       650    //Shutoff temperature for charger/inverter
 #define TEMP_LOAD_ON                        600    //Normal Temperature for charger/inverter ON
 #define TEMP_LCD_OFF                        350    //Shutoff temperature for LCD
 #define TEMP_LCD_ON                         330    //LCD operating temperatureupper value
 
 #define BATTERY_CUT_OFF_DELAY_TIME          10;//500;  //Cycles before the low voltage will cut off everything
 #define LOAD_OFF_DELAY_TIME                 50;  //Cycles before the low voltage will cut off the load
 
 
 #define VOLT_MAINS_LOW                      1100   //Minimum acceptable voltage from DC wall adaptor     (x100 Volts)
 #define VOLT_CAR_LOW                         700   //Minimum acceptable voltage from car adaptor         (x100 Volts)
 #define VOLT_SOLAR_LOW                      1430   //Minimum voltage to detect a solar panel-without load(x100 Volts)
 #define AMPS_SOLAR_LOW                      2     //Cutoff current level for solar panel: 0.5 Amps minimum current(*10 Amps)
 
 #define BATT_MINIMUM_VOLTAGE                105      //Voltage levels for valid LTC2943 readouts
 #define BATT_MAXIMUM_VOLTAGE                160
 
 #define EXPERT_MODE_DELAY                   10     //10*200mSec
 #define SOURCE_REFRESH_DELAY                2
 
 #define LED_PWM_INTERVAL                     10    //PWM time for LED strip
 #define DEFAULT_LED_LEVEL                     2    //level 2 = 50%
 #define MIN_LED_LEVEL                         1    //25%
 #define MAX_LED_LEVEL                         4    //100%