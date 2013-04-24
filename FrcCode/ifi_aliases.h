/********************************************************************************
* FILE NAME: ifi_aliases.h <FRC VERSION>
*
* DESCRIPTION: 
*  This file contains common macros (known as aliases in PBASIC) for the 
*  I/O pins of the Robot Controller and elements in the data sent between the 
*  User processor and the Master processor.
*  
*  If you want to create your own macros, do so in the designated section of 
*  the user_routines.h file.
*  
*  DO NOT EDIT THIS FILE!
********************************************************************************/

#ifndef __ifi_aliases_h_
#define __ifi_aliases_h_

#include <adc.h>

/* PWM Type Definitions used in Setup_PWM_Output_Type(...) */
#define IFI_PWM   0     /* Standard IFI PWM output generated with Generate_PWM(...) */
#define USER_CCP  1     /* User can use PWM pin as digital I/O or CCP pin. */

/*
 *-----------------------------------------------------------------------------------------------------
 *---------- Aliases for each OI analog input ---------------------------------------------------------
 *-----------------------------------------------------------------------------------------------------
 *  Below are aliases for the analog inputs located on the Operator Interface.
 */
#define p1_y        rxdata.oi_analog01
#define p2_y        rxdata.oi_analog02
#define p3_y        rxdata.oi_analog03
#define p4_y        rxdata.oi_analog04
#define p1_x        rxdata.oi_analog05
#define p2_x        rxdata.oi_analog06
#define p3_x        rxdata.oi_analog07
#define p4_x        rxdata.oi_analog08
#define p1_wheel    rxdata.oi_analog09
#define p2_wheel    rxdata.oi_analog10
#define p3_wheel    rxdata.oi_analog11
#define p4_wheel    rxdata.oi_analog12
#define p1_aux      rxdata.oi_analog13
#define p2_aux      rxdata.oi_analog14
#define p3_aux      rxdata.oi_analog15
#define p4_aux      rxdata.oi_analog16

/*
 *-----------------------------------------------------------------------------------------------------
 *---------- Aliases for each OI switch input ---------------------------------------------------------
 *-----------------------------------------------------------------------------------------------------
 *  Below are aliases for the digital inputs located on the Operator Interface.
 *  Ports 1 & 3 have their inputs duplicated in ports 4 & 2 respectively.  The 
 *  inputs from ports 1 & 3 may be disabled via the 'Disable' dip switch 
 *  located on the Operator Interface.  See Users Manual for details.
 */
#define p1_sw_trig  rxdata.oi_swA_byte.bitselect.bit0 /* Joystick Trigger Button, same as Port4 pin5 */
#define p1_sw_top   rxdata.oi_swA_byte.bitselect.bit1 /* Joystick Top Button,     same as Port4 pin8 */
#define p1_sw_aux1  rxdata.oi_swA_byte.bitselect.bit2 /* Aux input,               same as Port4 pin9 */
#define p1_sw_aux2  rxdata.oi_swA_byte.bitselect.bit3 /* Aux input,               same as Port4 pin15*/

#define p3_sw_trig  rxdata.oi_swA_byte.bitselect.bit4 /* Joystick Trigger Button, same as Port2 pin5 */
#define p3_sw_top   rxdata.oi_swA_byte.bitselect.bit5 /* Joystick Top Button,     same as Port2 pin8 */
#define p3_sw_aux1  rxdata.oi_swA_byte.bitselect.bit6 /* Aux input,               same as Port2 pin9 */
#define p3_sw_aux2  rxdata.oi_swA_byte.bitselect.bit7 /* Aux input,               same as Port2 pin15*/

#define p2_sw_trig  rxdata.oi_swB_byte.bitselect.bit0 /* Joystick Trigger Button*/
#define p2_sw_top   rxdata.oi_swB_byte.bitselect.bit1 /* Joystick Top Button*/
#define p2_sw_aux1  rxdata.oi_swB_byte.bitselect.bit2 /* Aux input*/
#define p2_sw_aux2  rxdata.oi_swB_byte.bitselect.bit3 /* Aux input*/

#define p4_sw_trig  rxdata.oi_swB_byte.bitselect.bit4 /* Joystick Trigger Button*/
#define p4_sw_top   rxdata.oi_swB_byte.bitselect.bit5 /* Joystick Top Button*/
#define p4_sw_aux1  rxdata.oi_swB_byte.bitselect.bit6 /* Aux input*/
#define p4_sw_aux2  rxdata.oi_swB_byte.bitselect.bit7 /* Aux input*/

/*
 *-----------------------------------------------------------------------------------------------------
 *---------- Aliases for DIGITAL IN/OUT connectors ----------------------------------------------------
 *-----------------------------------------------------------------------------------------------------
 *  Below are aliases for the digital I/O located on the Robot Controller.
 *  They can be configured to be either Inputs or Outputs.
 */

#define INPUT   1
#define OUTPUT  0

/* Used in User_Initialization routine in user_routines.c file. */
/* Used to set pins as digital INPUTS or digital OUTPUTS. */
#define digital_io_01   TRISBbits.TRISB2
#define digital_io_02   TRISBbits.TRISB3
#define digital_io_03   TRISBbits.TRISB4
#define digital_io_04   TRISBbits.TRISB5
#define digital_io_05   TRISBbits.TRISB6
#define digital_io_06   TRISBbits.TRISB7
#define digital_io_07   TRISHbits.TRISH0
#define digital_io_08   TRISHbits.TRISH1
#define digital_io_09   TRISHbits.TRISH2
#define digital_io_10   TRISHbits.TRISH3
#define digital_io_11   TRISJbits.TRISJ1
#define digital_io_12   TRISJbits.TRISJ2
#define digital_io_13   TRISJbits.TRISJ3
#define digital_io_14   TRISCbits.TRISC0
#define digital_io_15   TRISJbits.TRISJ4
#define digital_io_16   TRISJbits.TRISJ5
#define digital_io_17   TRISJbits.TRISJ6
#define digital_io_18   TRISJbits.TRISJ7

/* Aliases used to read the pins when used as INPUTS. */
#define rc_dig_in01     PORTBbits.RB2
#define rc_dig_in02     PORTBbits.RB3
#define rc_dig_in03     PORTBbits.RB4
#define rc_dig_in04     PORTBbits.RB5
#define rc_dig_in05     PORTBbits.RB6
#define rc_dig_in06     PORTBbits.RB7
#define rc_dig_in07     PORTHbits.RH0
#define rc_dig_in08     PORTHbits.RH1
#define rc_dig_in09     PORTHbits.RH2
#define rc_dig_in10     PORTHbits.RH3
#define rc_dig_in11     PORTJbits.RJ1
#define rc_dig_in12     PORTJbits.RJ2
#define rc_dig_in13     PORTJbits.RJ3
#define rc_dig_in14     PORTCbits.RC0
#define rc_dig_in15     PORTJbits.RJ4
#define rc_dig_in16     PORTJbits.RJ5
#define rc_dig_in17     PORTJbits.RJ6
#define rc_dig_in18     PORTJbits.RJ7

/* Aliases used to drive the pins when used as OUTPUTS. */
#define rc_dig_out01    LATBbits.LATB2
#define rc_dig_out02    LATBbits.LATB3
#define rc_dig_out03    LATBbits.LATB4
#define rc_dig_out04    LATBbits.LATB5
#define rc_dig_out05    LATBbits.LATB6
#define rc_dig_out06    LATBbits.LATB7
#define rc_dig_out07    LATHbits.LATH0
#define rc_dig_out08    LATHbits.LATH1
#define rc_dig_out09    LATHbits.LATH2
#define rc_dig_out10    LATHbits.LATH3
#define rc_dig_out11    LATJbits.LATJ1
#define rc_dig_out12    LATJbits.LATJ2
#define rc_dig_out13    LATJbits.LATJ3
#define rc_dig_out14    LATCbits.LATC0
#define rc_dig_out15    LATJbits.LATJ4
#define rc_dig_out16    LATJbits.LATJ5
#define rc_dig_out17    LATJbits.LATJ6
#define rc_dig_out18    LATJbits.LATJ7

/*
 *-----------------------------------------------------------------------------------------------------
 *---------- Aliases for PWM OUTPUTS ------------------------------------------------------------------
 *-----------------------------------------------------------------------------------------------------
 *  Below are aliases for the PWM OUTPUTS located on the Robot Controller.  
 */
#define pwm01           txdata.rc_pwm01
#define pwm02           txdata.rc_pwm02
#define pwm03           txdata.rc_pwm03
#define pwm04           txdata.rc_pwm04
#define pwm05           txdata.rc_pwm05
#define pwm06           txdata.rc_pwm06
#define pwm07           txdata.rc_pwm07
#define pwm08           txdata.rc_pwm08
#define pwm09           txdata.rc_pwm09
#define pwm10           txdata.rc_pwm10
#define pwm11           txdata.rc_pwm11
#define pwm12           txdata.rc_pwm12
#define pwm13           txdata.rc_pwm13
#define pwm14           txdata.rc_pwm14
#define pwm15           txdata.rc_pwm15
#define pwm16           txdata.rc_pwm16

/*
 *-----------------------------------------------------------------------------------------------------
 *---------- Aliases for RELAY OUTPUTS connectors ----------------------------------------------------
 *-----------------------------------------------------------------------------------------------------
 *  Below are aliases for the relay outputs located on the Robot Controller.  
 */
#define relay1_fwd      LATDbits.LATD0
#define relay1_rev      LATEbits.LATE0
#define relay2_fwd      LATDbits.LATD1
#define relay2_rev      LATEbits.LATE1
#define relay3_fwd      LATDbits.LATD2
#define relay3_rev      LATEbits.LATE2
#define relay4_fwd      LATDbits.LATD3
#define relay4_rev      LATEbits.LATE3
#define relay5_fwd      LATDbits.LATD4
#define relay5_rev      LATEbits.LATE4
#define relay6_fwd      LATDbits.LATD5
#define relay6_rev      LATEbits.LATE5
#define relay7_fwd      LATDbits.LATD6
#define relay7_rev      LATEbits.LATE6
#define relay8_fwd      LATDbits.LATD7
#define relay8_rev      LATJbits.LATJ0

/*
 *-----------------------------------------------------------------------------------------------------
 *---------- Aliases for ANALOG INPUTS connectors -----------------------------------------------------
 *-----------------------------------------------------------------------------------------------------
 *  Below are aliases for reading an analog voltage on the ANALOG INPUTS
 *  using the Get_Analog_Value() function.
 */
#define rc_ana_in01     ADC_CH0
#define rc_ana_in02     ADC_CH1
#define rc_ana_in03     ADC_CH2
#define rc_ana_in04     ADC_CH3
#define rc_ana_in05     ADC_CH4
#define rc_ana_in06     ADC_CH5
#define rc_ana_in07     ADC_CH6
#define rc_ana_in08     ADC_CH7
#define rc_ana_in09     ADC_CH8
#define rc_ana_in10     ADC_CH9
#define rc_ana_in11     ADC_CH10
#define rc_ana_in12     ADC_CH11
#define rc_ana_in13     ADC_CH12
#define rc_ana_in14     ADC_CH13
#define rc_ana_in15     ADC_CH14
#define rc_ana_in16     ADC_CH15

#define SIXTEEN_ANALOG      ADC_16ANA   /* All analog */

/*
 *-----------------------------------------------------------------------------------------------------
 *---------- Aliases for the ROBOT FEEDBACK LEDs ------------------------------------------------------
 *-----------------------------------------------------------------------------------------------------
 *  Below are aliases for the ROBOT FEEDBACK LEDs located on the Operator Interface.  
 */
#define User_Mode_byte txdata.LED_byte1.data

#define Pwm1_green      txdata.LED_byte1.bitselect.bit0
#define Pwm1_red        txdata.LED_byte1.bitselect.bit1 
#define Pwm2_green      txdata.LED_byte1.bitselect.bit2
#define Pwm2_red        txdata.LED_byte1.bitselect.bit3 
#define Relay1_red      txdata.LED_byte1.bitselect.bit4 
#define Relay1_green    txdata.LED_byte1.bitselect.bit5
#define Relay2_red      txdata.LED_byte1.bitselect.bit6 
#define Relay2_green    txdata.LED_byte1.bitselect.bit7

#define Switch1_LED     txdata.LED_byte2.bitselect.bit0
#define Switch2_LED     txdata.LED_byte2.bitselect.bit1
#define Switch3_LED     txdata.LED_byte2.bitselect.bit2

#define User_Byte1      txdata.user_byte1
#define User_Byte2      txdata.user_byte2
#define User_Byte3      txdata.user_byte3
#define User_Byte4      txdata.user_byte4
#define User_Byte5      txdata.user_byte5
#define User_Byte6      txdata.user_byte6

/*
 *-----------------------------------------------------------------------------------------------------
 *---------- Aliases for Battery Voltage bytes --------------------------------------------------------
 *-----------------------------------------------------------------------------------------------------
 * Formula to calculate voltage in volts: 
 *     current_voltage = battery_voltage * 0.038 + 0.05; 
 */

#define battery_voltage     rxdata.rc_main_batt*15.64/256
#define backup_voltage      rxdata.rc_backup_batt*15.64/256

/*
 *-----------------------------------------------------------------------------------------------------
 *---------- Aliases for User Modes  ------------------------------------------------------------------
 *-----------------------------------------------------------------------------------------------------
 * These bits tell you what mode your robot is currently in.
 */

#define user_display_mode   rxdata.rc_mode_byte.mode.user_display
#define autonomous_mode     rxdata.rc_mode_byte.mode.autonomous
#define competition_mode    rxdata.rc_mode_byte.mode.disabled

#endif

/*---------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------*/
