/*******************************************************************************
* FILE NAME: io_code.c
*
* DESCRIPTION:
*
* USAGE:
*
*******************************************************************************/

#ifndef CYGWIN_IFI_SIMULATOR
#include "ifi_aliases.h"
#include "ifi_default.h"
#include "ifi_utilities.h"
#include "ws_code.h"
#include "io_code.h"
#include "user_routines.h"
#include "printf_lib.h"
#include "delays.h"       /*defined locally*/
#else
#include "RC_Simulator.h"
#include "ifi_aliases.h"
#include "ifi_default.h"
#include "ws_code.h"
#include "io_code.h"
#include "stdio.h"
#endif

/*******************************************************************************
* FUNCTION NAME: assign_outputs_slow
* PURPOSE:       assign motor speeds to pwm outputs
* CALLED FROM:   
* ARGUMENTS:     motor_vals - speeds of motors
* RETURNS:       none
*******************************************************************************/
void assign_outputs_slow(MotorValsType *motor_vals)
{
  pwm01 = 127;
  pwm02 = 127;
  pwm03 = 127;
  pwm04 = 127;
  pwm05 = 127;
  pwm06 = 127;
  pwm07 = 127;
  pwm08 = 127;
  pwm09 = 127;
  pwm10 = 127;
  pwm11 = 127;
  pwm12 = 127;

  Rc_analog_out_lf_wheel = motor_vals->lf_wheel;
  Rc_analog_out_rf_wheel = motor_vals->rf_wheel;
  Rc_analog_out_lb_wheel = motor_vals->lb_wheel;
  Rc_analog_out_rb_wheel = motor_vals->rb_wheel;


  /*
  printf(" wheels: lf %d rf %d lb %d rb %d\n", (int)motor_vals->lf_wheel,
         (int)motor_vals->rf_wheel, (int)motor_vals->lb_wheel,
         (int)motor_vals->rb_wheel);
  */

  Rc_relay3_fwd = 0;
  Rc_relay3_rev = 0;
  Rc_relay4_fwd = 0;
  Rc_relay4_rev = 0;
  Rc_relay5_fwd = 0;
  Rc_relay5_rev = 0;
  Rc_relay6_fwd = 0;
  Rc_relay6_rev = 0;
  Rc_relay7_fwd = 0;
  Rc_relay7_rev = 0;
  Rc_relay8_fwd = 0;
  Rc_relay8_rev = 0;


  if (motor_vals->pump == RELAY_FWD)
  {
    Rc_relay_pump_on = 1;
    Rc_relay_pump_not_used = 0;
  }
  else
  {
    Rc_relay_pump_on = 0;
    Rc_relay_pump_not_used = 0;
  }

  if (motor_vals->foot == RELAY_FWD)
  {
    Rc_relay_foot_down = 1;
    Rc_relay_foot_up = 0;
  }
  else if (motor_vals->foot == RELAY_REV)
  {
    Rc_relay_foot_down = 0;
    Rc_relay_foot_up = 1;
  }
  else
  {
    Rc_relay_foot_down = 0;
    Rc_relay_foot_up = 0;
  }

  return;
}


/*******************************************************************************
* FUNCTION NAME: assign_outputs_fast
* PURPOSE:       assign motor speeds to pwm outputs
* CALLED FROM:   
* ARGUMENTS:     motor_vals - speeds of motors
* RETURNS:       none
*******************************************************************************/
void assign_outputs_fast(MotorValsType *motor_vals)
{
  pwm13 = 127;
  pwm14 = 127;
  pwm15 = 127;
  pwm16 = 127;

  Rc_analog_out_left_crab = motor_vals->left_crab;
  Rc_analog_out_right_crab = motor_vals->right_crab;

  return;
}




void io_print_oi_inputs()
{
  /*
  printf(" driveX %d", (int)Oi_drive_x);
  printf(" driveY %d", (int)Oi_drive_y);
  printf(" crabX %d", (int)Oi_crab_x);
  printf(" crabY %d", (int)Oi_crab_y);
  printf("\n");
  */


  return;
}


void io_print_rc_inputs()
{

  /*
  printf(" Lpot %d", (int)GET_ANALOG_VALUE_SHIFT(Analog_in_left_pot));
  printf(" Rpot %d", (int)GET_ANALOG_VALUE_SHIFT(Analog_in_right_pot));
  printf("\n");
  */

  return;
}



