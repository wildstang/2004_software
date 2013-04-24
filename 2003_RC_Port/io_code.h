/*******************************************************************************
* FILE NAME: io_code.h
*
* DESCRIPTION:
*  This is the include file which corresponds to io_code.c
*
*  It contains some aliases and function prototypes used in those files.
*
* USAGE:
*  If you add your own routines to those files, this is a good place to add
*  your custom macros (aliases), type definitions, and function prototypes.
*******************************************************************************/

#ifndef __io_code_h_
#define __io_code_h_

/******************************* TYPEDEFS *************************************/

/******************************** MACROS **************************************/

/***************************** DEFINITIONS ************************************/



/**************************************************************
 * Inputs
 **************************************************************/

/***** RC Analog Inputs *****/
#define Analog_in_left_pot        rc_ana_in01
#define Analog_in_right_pot       rc_ana_in02
#define Analog_in_03              rc_ana_in03
#define Analog_in_04              rc_ana_in04
#define Analog_in_05              rc_ana_in05
#define Analog_in_06              rc_ana_in06
#define Analog_in_07              rc_ana_in07
#define Analog_in_08              rc_ana_in08
#define Analog_in_09              rc_ana_in09
#define Analog_in_10              rc_ana_in10
#define Analog_in_11              rc_ana_in11
#define Analog_in_12              rc_ana_in12
#define Analog_in_13              rc_ana_in13
#define Analog_in_14              rc_ana_in14
#define Analog_in_15              rc_ana_in15
#define Analog_in_16              rc_ana_in16


/***** RC Digital Inputs *****/
#define Digital_in_pressure       rc_dig_in01
#define Digital_in_02             rc_dig_in02
#define Digital_in_03             rc_dig_in03
#define Digital_in_04             rc_dig_in04
#define Digital_in_05             rc_dig_in05
#define Digital_in_06             rc_dig_in06
#define Digital_in_07             rc_dig_in07
#define Digital_in_08             rc_dig_in08
#define Digital_in_09             rc_dig_in09
#define Digital_in_10             rc_dig_in10
#define Digital_in_11             rc_dig_in11
#define Digital_in_12             rc_dig_in12
#define Digital_in_13             rc_dig_in13
#define Digital_in_14             rc_dig_in14
#define Digital_in_15             rc_dig_in15
#define Digital_in_16             rc_dig_in16
#define Digital_in_17             rc_dig_in17
#define Digital_in_18             rc_dig_in18


/***** Joystick Analog Inputs *****/
#define Oi_drive_x                p3_x
#define Oi_drive_y                p3_y
#define Oi_crab_x                 p1_x
#define Oi_crab_y                 p1_y


/***** Joystick Digital Inputs *****/
#define Oi_sw_90_deg              p1_sw_top
#define Oi_sw_crab_manu_right     p1_sw_top
#define Oi_sw_crab_manu_left      p1_sw_trig
#define Oi_sw_turbo               p3_sw_top
#define Oi_sw_foot                p3_sw_trig


/***** Button Box Analog Inputs *****/


/***** Button Box Analog Inputs *****/


/**************************************************************
 * Outputs
 **************************************************************/
/***** RC Digital Outputs *****/
#define Rc_relay_pump_on          relay1_fwd   /* none         */
#define Rc_relay_pump_not_used    relay1_rev
#define Rc_relay_foot_down        relay2_fwd   /* red/black    */
#define Rc_relay_foot_up          relay2_rev
#define Rc_relay3_fwd             relay3_fwd   /* purple/black */
#define Rc_relay3_rev             relay3_rev
#define Rc_relay4_fwd             relay4_fwd   /* brown/black  */
#define Rc_relay4_rev             relay4_rev
#define Rc_relay5_fwd             relay5_fwd   /* orange/black */
#define Rc_relay5_rev             relay5_rev
#define Rc_relay6_fwd             relay6_fwd   /* yellow/black */
#define Rc_relay6_rev             relay6_rev
#define Rc_relay7_fwd             relay7_fwd   /* grey/black   */
#define Rc_relay7_rev             relay7_rev
#define Rc_relay8_fwd             relay8_fwd   /* blue/black   */
#define Rc_relay8_rev             relay8_rev


/***** RC Analog Outputs *****/
#define Rc_analog_out_lf_wheel    pwm01   /* red    */
#define Rc_analog_out_pwm02       pwm02   /* purple */
#define Rc_analog_out_pwm03       pwm03   /* brown  */
#define Rc_analog_out_rf_wheel    pwm04   /* orange */
#define Rc_analog_out_pwm05       pwm05   /* yellow */
#define Rc_analog_out_pwm06       pwm06   /* grey   */
#define Rc_analog_out_lb_wheel    pwm07   /* green  */
#define Rc_analog_out_rb_wheel    pwm08   /* blue   */
#define Rc_analog_out_pwm09       pwm09   /* white  */
#define Rc_analog_out_pwm10       pwm10   /* black  */
#define Rc_analog_out_pwm11       pwm11   /* ?      */
#define Rc_analog_out_pwm12       pwm12   /* ?      */
#define Rc_analog_out_pwm13       pwm13   /* ?      */
#define Rc_analog_out_pwm14       pwm14   /* ?      */
#define Rc_analog_out_left_crab   pwm15   /* ?      */
#define Rc_analog_out_right_crab  pwm16   /* ?      */





/***** Input Scaling *****/
/* Right pot reading for 0 brads and 255 brads (180 degrees to right & left) */
#define POT_RCRAB_RIGHT_180       12
#define POT_RCRAB_LEFT_180        243

/* Left pot reading for 0 brads and 255 brads (180 degrees to right & left) */
#define POT_LCRAB_RIGHT_180       12
#define POT_LCRAB_LEFT_180        243

#define CRAB_STICK_MIN_VALUE      0
#define CRAB_STICK_MAX_VALUE      254
#define CRAB_STICK_TRAVEL_LOW     (127 - CRAB_STICK_MIN_VALUE)
#define CRAB_STICK_TRAVEL_HI      (CRAB_STICK_MAX_VALUE - 127)




/****************************** STRUCTURES ************************************/

/************************* FUNCTION PROTOTYPES ********************************/
extern void assign_outputs_slow(MotorValsType *);
extern void assign_outputs_fast(MotorValsType *);
extern void io_print_oi_inputs(void);
extern void io_print_rc_inputs(void);


#endif /* __io_code_h_ */

