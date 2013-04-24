/*******************************************************************************
* FILE NAME: user_routines.c <FRC VERSION>
*
* DESCRIPTION:
*  This file contains the default mappings of inputs  
*  (like switches, joysticks, and buttons) to outputs on the RC.  
*
* USAGE:
*  You can either modify this file to fit your needs, or remove it from your 
*  project and replace it with a modified copy. 
*
*******************************************************************************/

#ifndef CYGWIN_IFI_SIMULATOR
#include "ifi_aliases.h"
#include "ifi_default.h"
#include "ifi_utilities.h"
#include "ws_includes.h"
#include "ws_autonomous.h"
#include "ws_io.h"
#include "user_routines.h"
#include "printf_lib.h"
#else
#include "RC_Simulator.h"
#include "ifi_aliases.h"
#include "ifi_default.h"
#include "ws_includes.h"
#include "ws_autonomous.h"
#include "ws_io.h"
#include "user_routines.h"
#include "stdio.h"
#endif

#include "eeprom.h"
#include "ws_drive_input.h"
#include "ws_crab_input.h"
#include "ws_crab_feedback.h"
#include "ws_general.h"
#include "ws_calibrate.h"
#include "ws_arms.h"
#include "ws_rtc.h"
#include "ws_serial2.h"
#include "ws_cc.h"


/*** DEFINE USER VARIABLES AND INITIALIZE THEM HERE ***/
/* EXAMPLES: (see MPLAB C18 User's Guide, p.9 for all types)
unsigned char wheel_revolutions = 0; (can vary from 0 to 255)
unsigned int  delay_count = 7;       (can vary from 0 to 65,535)
int           angle_deviation = 142; (can vary from -32,768 to 32,767)
unsigned long very_big_counter = 0;  (can vary from 0 to 4,294,967,295)
*/


/*******************************************************************************
* FUNCTION NAME: Limit_Mix
* PURPOSE:       Limits the mixed value for one joystick drive.
* CALLED FROM:   Default_Routine, this file
* ARGUMENTS:     
*     Argument             Type    IO   Description
*     --------             ----    --   -----------
*     intermediate_value    int    I    
* RETURNS:       unsigned char
*******************************************************************************/
unsigned char Limit_Mix (int intermediate_value)
{
  static int limited_value;
  
  if (intermediate_value < 2000)
  {
    limited_value = 2000;
  }
  else if (intermediate_value > 2254)
  {
    limited_value = 2254;
  }
  else
  {
    limited_value = intermediate_value;
  }
  return (unsigned char) (limited_value - 2000);
}


/*******************************************************************************
* FUNCTION NAME: User_Initialization
* PURPOSE:       This routine is called first (and only once) in the Main function.  
*                You may modify and add to this function.
* CALLED FROM:   main.c
* ARGUMENTS:     none
* RETURNS:       void
*******************************************************************************/
void User_Initialization (void)
{
  rom const	char *strptr = "IFI User Processor Initialized ...";

  Set_Number_of_Analog_Channels(SIXTEEN_ANALOG);    /* DO NOT CHANGE! */

/* FIRST: Set up the I/O pins you want to use as digital INPUTS. */
  digital_io_01 = digital_io_02 = digital_io_03 = digital_io_04 = INPUT;
  digital_io_05 = digital_io_06 = digital_io_07 = digital_io_08 = INPUT;
  digital_io_09 = digital_io_10 = digital_io_11 = digital_io_12 = INPUT;
  digital_io_13 = digital_io_14 = digital_io_15 = digital_io_16 = INPUT;
  digital_io_17 = digital_io_18 = INPUT;
    /* 
     Note: digital_io_01 = digital_io_02 = ... digital_io_04 = INPUT; 
           is the same as the following:

           digital_io_01 = INPUT;
           digital_io_02 = INPUT;
           ...
           digital_io_04 = INPUT;
    */

/* SECOND: Set up the I/O pins you want to use as digital OUTPUTS. */
  /*
  digital_io_17 = OUTPUT;
  */

/* THIRD: Initialize the values on the digital outputs. */
  /*
  rc_dig_out17 = 0;
  */

/* FOURTH: Set your initial PWM values.  Neutral is 127. */
  pwm01 = pwm02 = pwm03 = pwm04 = pwm05 = pwm06 = pwm07 = pwm08 = 127;
  pwm09 = pwm10 = pwm11 = pwm12 = pwm13 = pwm14 = pwm15 = pwm16 = 127;

/* FIFTH: Set your PWM output types for PWM OUTPUTS 13-16. */
  /*   Choose from these parameters for PWM 13-16 respectively:               */
  /*     IFI_PWM  - Standard IFI PWM output generated with Generate_Pwms(...) */
  /*     USER_CCP - User can use PWM pin as digital I/O or CCP pin.           */
  Setup_PWM_Output_Type(IFI_PWM,IFI_PWM,IFI_PWM,IFI_PWM);

  /* 
     Example: The following would generate a 40KHz PWM with a 50% duty cycle on the CCP2 pin:

         CCP2CON = 0x3C;
         PR2 = 0xF9;
         CCPR2L = 0x7F;
         T2CON = 0;
         T2CONbits.TMR2ON = 1;

         Setup_PWM_Output_Type(USER_CCP,IFI_PWM,IFI_PWM,IFI_PWM);
  */

  /* Add any other initialization code here. */

  Initialize_Serial_Comms();
  Initialize_Serial_Comms2();

  Putdata(&txdata);             /* DO NOT CHANGE! */

  printf("%s\n", strptr);       /* Optional - Print initialization message. */

#if 1
  autonomous_init();            /* Initialize the autonomous code */
#endif
#if 0
  initRtc();                    /* Init the real-time clock */
#endif

  User_Proc_Is_Ready();         /* DO NOT CHANGE! - last line of User_Initialization */
}

/*******************************************************************************
* FUNCTION NAME: Process_Data_From_Master_uP
* PURPOSE:       Executes every 26.2ms when it gets new data from the master 
*                microprocessor.
* CALLED FROM:   main.c
* ARGUMENTS:     none
* RETURNS:       void
*******************************************************************************/
void Process_Data_From_Master_uP(MotorValsType *motor_vals,
                                 CrabTargetsType *crab_targets,
                                 CalibrationValsType *calibration_vals,
                                 int pot_state)
{
  int rtc1, rtcd1, rtc2, rtcd2;

  Getdata(&rxdata);   /* Get fresh data from the master microprocessor. */

  /*
  rtc1 = getRtc();
  rtcd1 = getRtcDelta(0);
  rtc2 = getRtc();
  rtcd2 = getRtcDelta(0);

  printf("MAS-RTC=0x%8X Delta=0x%8X\n",(int)rtc1, (int)rtcd1);
  printf("MA2-RTC=0x%8X Delta=0x%8X\n",(int)rtc2, (int)rtcd2);
  */

  io_print_oi_inputs();
  io_print_rc_inputs();

  if (Oi_calibrate < OI_CALIBRATE_POTS)
  {
    processEEQueue();
    calibrate_pots();
    display_calibration();

    if (checkEEQueue() == EE_EMPTY)
    {
      retrieve_calibration(calibration_vals);
    }

    motor_vals->lf_wheel = 0;
    motor_vals->rf_wheel = 0;
    motor_vals->lb_wheel = 0;
    motor_vals->rb_wheel = 0;
    motor_vals->crab = 0;
    motor_vals->big_arm_shoulder = 0;
    motor_vals->big_arm_elbow = 0;
    motor_vals->small_arm_shoulder = 0;
    motor_vals->winch = 0;
    motor_vals->arm_brake = 127;
    motor_vals->winch_brake = 127;

    motor_vals->pump = PUMP_OFF;
  }
  else
  {
    if (Oi_calibrate > OI_CALIBRATE_JOYSTICKS)
    {
      display_calibration();
    }

    if (competition_mode == ROBOT_DISABLED)
    {
      auto_chooser();
    }

    pump_control(motor_vals);

#if 0
    /* scale drive & crab joysticks */
    joystick_scaling(&Oi_crab_y, CRAB_STICK_Y_MIN, CRAB_STICK_Y_MIDDLE,
                     CRAB_STICK_Y_MAX);
#endif
    joystick_scaling(&Oi_drive_x, DRIVE_STICK_X_MIN, DRIVE_STICK_X_MIDDLE,
                     DRIVE_STICK_X_MAX);
    joystick_scaling(&Oi_drive_y, DRIVE_STICK_Y_MIN, DRIVE_STICK_Y_MIDDLE,
                     DRIVE_STICK_Y_MAX);
    joystick_scaling(&Oi_crab_x, CRAB_STICK_X_MIN, CRAB_STICK_X_MIDDLE,
                     CRAB_STICK_X_MAX);

    drive_stick_input(motor_vals);

    if ((pot_state == POTS_GOOD) &&
        (Oi_sw_crab_manu_feed_sel == OI_CRAB_FEEDBACK_SELECTOR))
    {
      crab_stick_input(crab_targets);
      /*
      crab_system(crab_targets, motor_vals, calibration_vals);
      */
    }
    else
    {
      crab_manu_override(motor_vals);
    }

    big_arm_control(motor_vals, calibration_vals);
    winch_control(motor_vals);
    small_arm_control(motor_vals, calibration_vals);

    /*
    motor_vals->big_arm_shoulder = 0;
    motor_vals->big_arm_elbow = 0;
    motor_vals->arm_brake = 127;
    motor_vals->accumulator = ACCUMULATOR_STOP;
    motor_vals->winch = 0;
    motor_vals->winch_brake = 127;
    motor_vals->small_arm_shoulder = 0;
    motor_vals->small_arm_squeeze = SQUEEZE_IN;
    */

    scale_bupod_speed(motor_vals, calibration_vals);

    if (Oi_sw_suspension_enable == OI_SUSPENSION_ENABLE)
    {
      suspension_control(motor_vals);
      bumper_control(motor_vals);
    }

    /*
    motor_vals->front_bumper = BUMPER_IN;
    motor_vals->back_bumper = BUMPER_IN;
    */
  }

  assign_outputs_fast(motor_vals);
  Generate_Pwms(pwm13,pwm14,pwm15,pwm16);

  assign_outputs_slow(motor_vals);
  Putdata(&txdata);             /* DO NOT CHANGE! */

  display_auto_data();

  {
    UINT8 big_arm_val;
    big_arm_val = GET_ANALOG_VALUE_SHIFT(Analog_in_big_arm_shoulder_pot);

    big_arm_val = 100+(big_arm_val-BIG_ARM_HANG_TARGET);
    display_oi_data(big_arm_val, DISPLAY_DATA_PSI);
  }
  /* display air pressure in PSI on the OI */
  /*display_oi_data((((int)GET_ANALOG_VALUE_SHIFT(Analog_in_pressure_sensor) *
                    (int)120) / (int)PUMP_HI_VAL), DISPLAY_DATA_PSI);*/
#if 0
  {
     /* Display heading on OI */
     UINT8 curr_theta;
     cc_get_heading(&curr_theta);
     display_oi_data(curr_theta, DISPLAY_DATA_PSI);
  }
#endif
}


