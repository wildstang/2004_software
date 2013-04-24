/*******************************************************************************
* FILE NAME: ws_io.h
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

#ifndef __ws_io_h_
#define __ws_io_h_

/******************************* TYPEDEFS *************************************/

/******************************** MACROS **************************************/

/***************************** DEFINITIONS ************************************/
#define OI_SUSPENSION_ENABLE   0
#define OI_SUSPENSION_DISABLE  1
#define OI_CRAB_ENABLE         1
#define OI_CRAB_DISABLE        0

#define OI_CALIBRATE_POTS       50
#define OI_CALIBRATE_JOYSTICKS  200

#define OI_CRAB_FEEDBACK_SELECTOR 0
#define OI_CRAB_MANUAL_SELECTOR   1

/**************************************************************
 * Inputs
 **************************************************************/

/***** RC Analog Inputs *****/
#define Analog_in_pressure_sensor        rc_ana_in01
#define Analog_in_crab_pot               rc_ana_in02
#define Analog_in_big_arm_elbow_pot      rc_ana_in03
#define Analog_in_big_arm_shoulder_pot   rc_ana_in04
#define Analog_in_small_arm_shoulder_pot rc_ana_in05
#define Analog_in_06                     rc_ana_in06
#define Analog_in_07                     rc_ana_in07
#define Analog_in_08                     rc_ana_in08
#define Analog_in_09                     rc_ana_in09
#define Analog_in_10                     rc_ana_in10
#define Analog_in_11                     rc_ana_in11
#define Analog_in_12                     rc_ana_in12
#define Analog_in_13                     rc_ana_in13
#define Analog_in_14                     rc_ana_in14
#define Analog_in_15                     rc_ana_in15
#define Analog_in_16                     rc_ana_in16


/***** RC Digital Inputs *****/
#if 0
#define Dig_in_pressure           rc_dig_in01
#define Dig_in_02                 rc_dig_in02
#else
#define Dig_in_01                 rc_dig_in01
#define Dig_in_pressure           rc_dig_in02
#endif
#define Dig_in_03                 rc_dig_in03
#define Dig_in_04                 rc_dig_in04
#define Dig_in_field_side         rc_dig_in05
#define Dig_in_06                 rc_dig_in06
#define Dig_in_07                 rc_dig_in07
#define Dig_in_08                 rc_dig_in08
#define Dig_in_09                 rc_dig_in09
#define Dig_in_10                 rc_dig_in10
#define Dig_in_plat_fl_wheel_f    rc_dig_in11
#define Dig_in_plat_fl_wheel_b    rc_dig_in12
#define Dig_in_plat_fr_wheel_f    rc_dig_in13
#define Dig_in_plat_fr_wheel_b    rc_dig_in14
#define Dig_in_plat_bl_wheel_f    rc_dig_in15
#define Dig_in_plat_bl_wheel_b    rc_dig_in16
#define Dig_in_plat_br_wheel_f    rc_dig_in17
#define Dig_in_plat_br_wheel_b    rc_dig_in18


/***** Drive Joystick Analog Inputs *****/
#define Oi_drive_x                p3_wheel
#define Oi_drive_y                p3_aux
#define Oi_crab_x                 p3_x
#define Oi_crab_y                 p3_y


/***** Drive Joystick Digital Inputs *****/
#define Oi_sw_ascend              p3_sw_top  /* crab stick top */
#define Oi_sw_manu_susp_back      p3_sw_trig /* crab stick trigger */
#define Oi_sw_manu_susp_front     p3_sw_aux1 /* drive stick trigger */
#define Oi_sw_turbo               p3_sw_aux2 /* drive stick top */
#define Oi_sw_cal_crab            p3_sw_trig


/***** Aux Joystick Analog Inputs *****/
#define Oi_big_arm_shoulder       p1_y
#define Oi_big_arm_elbow          p1_x
#define Oi_big_arm_auto           p1_aux
#define Oi_small_arm_auto         p1_aux
#define Oi_platform_selector      p1_aux
#define Oi_small_arm_shoulder     p2_y
#define Oi_winch                  p2_aux

/***** Aux Joystick Digital Inputs *****/
#define Oi_sw_accumulator_rev     p1_sw_top
#define Oi_sw_accumulator_fwd     p1_sw_trig
#define Oi_sw_bumper_front        p1_sw_aux1
#define Oi_sw_auton_lock1         p1_sw_top
#define Oi_sw_auton_lock2         p1_sw_trig

#define Oi_sw_squeeze             p2_sw_trig
#define Oi_sw_bumper_back         p2_sw_aux1

#define Oi_sw_cal_b_arm_shoulder  p1_sw_trig
#define Oi_sw_cal_b_arm_elbow     p1_sw_top
#define Oi_sw_cal_s_arm_shoulder  p2_sw_trig


/***** Button Box Inputs *****/
#define Oi_auton_selector         p4_x
#define Oi_calibrate              p4_y

#define Oi_sw_suspension_enable   p4_sw_top
#define Oi_sw_drive_straight      p4_sw_trig
#define Oi_sw_crab_manu_feed_sel  p4_sw_aux1
#define Oi_sw_crab_manu_move      p4_sw_aux2

/**************************************************************
 * Outputs
 **************************************************************/
/***** RC Digital Outputs *****/
#define Rc_relay_accumulator_fwd     relay1_fwd   /* green/black  */
#define Rc_relay_accumulator_rev     relay1_rev
#define Rc_relay_2_fwd               relay2_fwd   /* blue/black   */
#define Rc_relay_2_rev               relay2_rev
#define Rc_relay_susp_front          relay3_fwd   /* red/black    */
#define Rc_relay_susp_back           relay3_rev
#define Rc_relay_s_arm_squeezer_out  relay4_fwd   /* orange/black */
#define Rc_relay_s_arm_not_used      relay4_rev
#define Rc_relay_bumper_front        relay5_fwd   /* yellow/black */
#define Rc_relay_bumper_back         relay5_rev
#define Rc_relay_pump_on             relay6_fwd   /* grey/black   */
#define Rc_relay_pump_not_used       relay6_rev
#define Rc_relay_7_fwd               relay7_fwd   /* white/black  */
#define Rc_relay_7_rev               relay7_rev
#define Rc_relay_8_fwd               relay8_fwd   /* black/black  */
#define Rc_relay_8_rev               relay8_rev


/***** RC Analog Outputs *****/
#define Rc_analog_out_lf_wheel            pwm01   /* green  */
#define Rc_analog_out_rf_wheel            pwm02   /* blue   */
#define Rc_analog_out_lb_wheel            pwm03   /* red    */
#define Rc_analog_out_rb_wheel            pwm04   /* orange */
#define Rc_analog_out_big_arm_shoulder    pwm05   /* yellow */
#define Rc_analog_out_winch               pwm06   /* grey   */
#define Rc_analog_out_arm_brake           pwm07   /* white  */
#define Rc_analog_out_winch_brake         pwm08   /* black  */
#define Rc_analog_out_small_arm_shoulder  pwm09   /* ?      */
#define Rc_analog_out_big_arm_elbow       pwm10   /* ?      */
#define Rc_analog_out_pwm11               pwm11   /* ?      */
#define Rc_analog_out_pwm12               pwm12   /* ?      */
#define Rc_analog_out_crab                pwm13   /* purple */
#define Rc_analog_out_pwm14               pwm14   /* ?      */
#define Rc_analog_out_pwm15               pwm15   /* ?      */
#define Rc_analog_out_pwm16               pwm16   /* ?      */


/***** Input Scaling *****/
#define CRAB_STICK_MIN_VALUE      0
#define CRAB_STICK_MAX_VALUE      254
#define CRAB_STICK_TRAVEL_LOW     (127 - CRAB_STICK_MIN_VALUE)
#define CRAB_STICK_TRAVEL_HI      (CRAB_STICK_MAX_VALUE - 127)

/* drive joystick scaling constants */
#define DRIVE_STICK_X_MIN        26
#define DRIVE_STICK_X_MIDDLE     127
#define DRIVE_STICK_X_MAX        254
#define DRIVE_STICK_Y_MIN        28
#define DRIVE_STICK_Y_MIDDLE     127
#define DRIVE_STICK_Y_MAX        254

/* crab joystick scaling constants */
#define CRAB_STICK_X_MIN         17
#define CRAB_STICK_X_MIDDLE      127
#define CRAB_STICK_X_MAX         254
#define CRAB_STICK_Y_MIN         11
#define CRAB_STICK_Y_MIDDLE      127
#define CRAB_STICK_Y_MAX         253

#define SC_CALIB_STICK_DEADZONE   5


/****************************** STRUCTURES ************************************/

/************************* FUNCTION PROTOTYPES ********************************/
extern void assign_outputs_slow(MotorValsType *);
extern void assign_outputs_fast(MotorValsType *);
extern void joystick_scaling(UINT8 *, UINT8, UINT8, UINT8);
extern void io_print_oi_inputs(void);
extern void io_print_rc_inputs(void);


#endif /* __ws_io_h_ */

