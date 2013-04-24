/*******************************************************************************
* FILE NAME: ws_io.c
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
#include "ws_includes.h"
#include "ws_io.h"
#include "user_routines.h"
#include "printf_lib.h"
#include "delays.h"       /*defined locally*/
#else
#include "RC_Simulator.h"
#include "ifi_aliases.h"
#include "ifi_default.h"
#include "ws_includes.h"
#include "ws_io.h"
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

#ifdef MOTOR_TEST
  if ((Oi_drive_y > (127 - SC_CALIB_STICK_DEADZONE)) &&
      (Oi_drive_y < (127 + SC_CALIB_STICK_DEADZONE)))
  {
    Oi_drive_y = 127;
  }

  /*
  Rc_analog_out_lf_wheel = Oi_drive_y;
  Rc_analog_out_rf_wheel = Oi_drive_y;
  Rc_analog_out_lb_wheel = Oi_drive_y;
  Rc_analog_out_rb_wheel = Oi_drive_y;
  Rc_analog_out_big_arm_shoulder = Oi_drive_y;
  Rc_analog_out_big_arm_elbow = Oi_drive_y;
  Rc_analog_out_small_arm_shoulder = Oi_drive_y;
  Rc_analog_out_winch = Oi_drive_y;

  printf(" driveY=%d\n", (int)Oi_drive_y);
  */

#else
  Rc_analog_out_lf_wheel = 127 + motor_vals->lf_wheel;
  Rc_analog_out_rf_wheel = 127 + motor_vals->rf_wheel;
  Rc_analog_out_lb_wheel = 127 + motor_vals->lb_wheel;
  Rc_analog_out_rb_wheel = 127 + motor_vals->rb_wheel;
  Rc_analog_out_big_arm_shoulder = 127 + motor_vals->big_arm_shoulder;
  Rc_analog_out_big_arm_elbow = 127 + motor_vals->big_arm_elbow;
  Rc_analog_out_small_arm_shoulder = 127 + motor_vals->small_arm_shoulder;
  Rc_analog_out_winch = 127 + motor_vals->winch;
  Rc_analog_out_arm_brake = motor_vals->arm_brake;
  Rc_analog_out_winch_brake = motor_vals->winch_brake;
#endif



  /*
  printf(" wheels: lf %d rf %d lb %d rb %d\n", (int)motor_vals->lf_wheel,
         (int)motor_vals->rf_wheel, (int)motor_vals->lb_wheel,
         (int)motor_vals->rb_wheel);
  */

  /*
  printf(" b_arm shoulder %3d elbow %3d  s_arm shoulder %3d\n",
         (int)motor_vals->big_arm_shoulder,
         (int)motor_vals->big_arm_elbow,
         (int)motor_vals->small_arm_shoulder);
  */

  /*
  printf(" winch %3d  brake %3d\n", (int)Rc_analog_out_winch,
         (int)Rc_analog_out_winch_brake);
  */

  relay7_fwd = 0;
  relay7_rev = 0;


#ifdef MOTOR_TEST

#else
  /* front suspension */
  if (motor_vals->suspension_front == SUSPENSION_DOWN)
  {
    Rc_relay_susp_front = 1;
  }
  else
  {
    Rc_relay_susp_front = 0;
  }

  /* back suspension */
  if (motor_vals->suspension_back == SUSPENSION_DOWN)
  {
    Rc_relay_susp_back = 1;
  }
  else
  {
    Rc_relay_susp_back = 0;
  }
/*
  printf(" susp f %d b %d\n", (int)Rc_relay_susp_front,
         (int)Rc_relay_susp_back);
         */

  /* pump */
  if (motor_vals->pump == PUMP_ON)
  {
    Rc_relay_pump_on = 1;
    Rc_relay_pump_not_used = 0;
  }
  else
  {
    Rc_relay_pump_on = 0;
    Rc_relay_pump_not_used = 0;
  }

  /* accumulator */
  if (motor_vals->accumulator == ACCUMULATOR_FWD)
  {
    Rc_relay_accumulator_fwd = 1;
    Rc_relay_accumulator_rev = 0;
  }
  else if (motor_vals->accumulator == ACCUMULATOR_REV)
  {
    Rc_relay_accumulator_fwd = 0;
    Rc_relay_accumulator_rev = 1;
  }
  else
  {
    Rc_relay_accumulator_fwd = 0;
    Rc_relay_accumulator_rev = 0;
  }

  /* small arm squeezer */
#if 1
  if (motor_vals->small_arm_squeeze == SQUEEZE_IN)
  {
    Rc_relay_s_arm_squeezer_out = 1;
  }
  else
  {
    Rc_relay_s_arm_squeezer_out = 0;
  }

  Rc_relay_s_arm_not_used = 1;
#else
  if (motor_vals->small_arm_squeeze == SQUEEZE_IN)
  {
    Rc_relay_s_arm_squeezer_in = 1;
    Rc_relay_s_arm_squeezer_out = 0;
  }
  else if (motor_vals->small_arm_squeeze == SQUEEZE_OUT)
  {
    Rc_relay_s_arm_squeezer_in = 0;
    Rc_relay_s_arm_squeezer_out = 1;
  }
  else
  {
    Rc_relay_s_arm_squeezer_in = 0;
    Rc_relay_s_arm_squeezer_out = 0;
  }
#endif

  /*
  printf("squeezer in %d out %d\n", (int)Rc_relay_s_arm_squeezer_in,
         (int)Rc_relay_s_arm_squeezer_out);
  printf("squeezer %d\n", (int)Rc_relay_s_arm_squeezer_out);
  */

  /* front bumper */
  if (motor_vals->front_bumper == BUMPER_IN)
  {
    Rc_relay_bumper_front = 0;
  }
  else
  {
    Rc_relay_bumper_front = 1;
  }

  /* back bumper */
  if (motor_vals->back_bumper == BUMPER_IN)
  {
    Rc_relay_bumper_back = 0;
  }
  else
  {
    Rc_relay_bumper_back = 1;
  }

  /*
  printf("bumper f %d b %d\n", (int)Rc_relay_bumper_front,
         (int)Rc_relay_bumper_back);
  */

#endif

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

#ifdef MOTOR_TEST
  if ((Oi_drive_y > (127 - SC_CALIB_STICK_DEADZONE)) &&
      (Oi_drive_y < (127 + SC_CALIB_STICK_DEADZONE)))
  {
    Oi_drive_y = 127;
  }

  /*
  Rc_analog_out_crab = Oi_drive_y;
  */

  printf(" driveY %d\n", (int)Oi_drive_y);

#else


  Rc_analog_out_crab = 127 + motor_vals->crab;


  /*
  printf("crab out %3d\n", (int)Rc_analog_out_crab);
  */


#endif

  /*
  printf(" crab speed: %d\n", (int)motor_vals->crab);
  */

  return;
}



/*******************************************************************************
* FUNCTION NAME: joystick_scaling
* PURPOSE:       Scale joystick so the x & y ranges are 0-254
* ARGUMENTS:     none
* RETURNS:       none
*******************************************************************************/
void joystick_scaling(UINT8 *joystick_val, UINT8 stick_min_val,
                      UINT8 stick_mid_val, UINT8 stick_max_val)
{
  int tmp_val;

  /*
  printf("In %3d", (int)*joystick_val);
  */

  if (*joystick_val < stick_mid_val)
  {
    tmp_val = ((((int)stick_mid_val - *joystick_val) * 127) /
               (stick_mid_val - stick_min_val));

    /* prevent underflow if tmp_val is > 127 */
    *joystick_val = 127 - MAX_RETURN(tmp_val, 127);
  }
  else if (*joystick_val > stick_mid_val)
  {
    tmp_val = (((*joystick_val - (int)stick_mid_val) * 127) /
               ((int)stick_max_val - stick_mid_val));

    /* prevent overflow if tmp_val is > 127 */
    *joystick_val = 127 + MAX_RETURN(tmp_val, 127);
  }

  /*
  printf("  Out %3d", (int)*joystick_val);
  printf("\n");
  */

  return;
}



void io_print_oi_inputs()
{
  /*
  printf(" Drive x %d y %d ", (int)Oi_drive_x, (int)Oi_drive_y);
  printf(" Crab x %d y %d ", (int)Oi_crab_x, (int)Oi_crab_y);
  printf("\n");
  */

  /*
  printf(" susp enable %d  crab manu %d", (int)Oi_sw_suspension_enable,
         (int)Oi_sw_crab_manual_mode);
  printf("\n");
  */
/*
  printf("Susp_front=%d, SuspBack=%d ",(int)Oi_sw_manu_susp_front,
                                         (int)Oi_sw_manu_susp_back);
 */
  /*
  printf("p1 x %3d y %3d wheel %3d aux %3d top %d trig %d aux1 %d aux2 %d\n",
         (int)p1_x, (int)p1_y, (int)p1_wheel, (int)p1_aux,
         (int)p1_sw_top, (int)p1_sw_trig, (int)p1_sw_aux1, (int)p1_sw_aux2);
  printf("p2 x %3d y %3d wheel %3d aux %3d top %d trig %d aux1 %d aux2 %d\n",
         (int)p2_x, (int)p2_y, (int)p2_wheel, (int)p2_aux,
         (int)p2_sw_top, (int)p2_sw_trig, (int)p2_sw_aux1, (int)p2_sw_aux2);
  printf("p3 x %3d y %3d wheel %3d aux %3d top %d trig %d aux1 %d aux2 %d\n",
         (int)p3_x, (int)p3_y, (int)p3_wheel, (int)p3_aux,
         (int)p3_sw_top, (int)p3_sw_trig, (int)p3_sw_aux1, (int)p3_sw_aux2);
  printf("p4 x %3d y %3d wheel %3d aux %3d top %d trig %d aux1 %d aux2 %d\n",
         (int)p4_x, (int)p4_y, (int)p4_wheel, (int)p4_aux,
         (int)p4_sw_top, (int)p4_sw_trig, (int)p4_sw_aux1, (int)p4_sw_aux2);
  */

  return;
}


void io_print_rc_inputs()
{

  /*
  printf(" IR F %d  R %d", (int)Dig_in_plat_fl_wheel_f,
         (int)Dig_in_plat_bl_wheel_f);
  printf(" Crab pot %d", (int)GET_ANALOG_VALUE_SHIFT(Analog_in_crab_pot));
  printf("\n");
  */

  /*
  printf(" Big arm shoulder %3d elbow %3d\n",
         (int)GET_ANALOG_VALUE_SHIFT(Analog_in_big_arm_shoulder_pot),
         (int)GET_ANALOG_VALUE_SHIFT(Analog_in_big_arm_elbow_pot));
  */

  /*printf("pressure %3d\n",
         (int)GET_ANALOG_VALUE_SHIFT(Analog_in_pressure_sensor));*/

  /*
  printf("field side %d\n", (int)Dig_in_field_side);
  */

  /*
  printf(" FL f%d b%d FR f%d b%d BL f%d b%d BR f%d b%d\n",
         (int)Dig_in_plat_fl_wheel_f,
         (int)Dig_in_plat_fl_wheel_b,
         (int)Dig_in_plat_fr_wheel_f,
         (int)Dig_in_plat_fr_wheel_b,
         (int)Dig_in_plat_bl_wheel_f,
         (int)Dig_in_plat_bl_wheel_b,
         (int)Dig_in_plat_br_wheel_f,
         (int)Dig_in_plat_br_wheel_b);
  */

  return;
}



