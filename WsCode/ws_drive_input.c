/*******************************************************************************
* FILE NAME: ws_drive_input.c
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
#include "user_routines.h"
#include "printf_lib.h"
#include "delays.h"       /*defined locally*/
#include "ws_io.h"
#else
#include "RC_Simulator.h"
#include "ifi_aliases.h"
#include "ifi_default.h"
#include "stdio.h"
#include "ws_includes.h"
#include "ws_io.h"
#endif

#include "ws_drive_input.h"
#include "ws_trig.h"
#include "ws_cc.h"

/* For s_locked_in_theta */
#define INVALID_THETA 35  /* Define that signifies that Theta is not valid */

#define MIN_THETA_DIFF 20 /* Min distance needed for the robot to be able to
                           * correct its theta */

static UINT8 sg_locked_in_theta=INVALID_THETA;
static UINT8 sg_curr_theta;

/* For s_prev_steer_mode */
#define TANK_STEER_MODE     0x00
#define DEADZONE_STEER_MODE 0x01
#define CRAB_STEER_MODE     0x02

#define CRABING_DIST_FROM_TARGET  50  /* Dist from target passed to SetHeading
                                       * when we are crabing. Lower num causes
                                       * a quicker adjustment, higher causes slower.
                                       */
/*******************************************************************************
* FUNCTION NAME: drive_stick_input
* PURPOSE:       Process position of drive joystick
* CALLED FROM:   Process_Data_From_Master_uP()
* ARGUMENTS:     OIData - drive & crab sticks
*                motor_vals - drive wheels
* RETURNS:       none
* DOL:
*   Create a dead zone in the X & Y axis of the drive stick.
*   If the crab stick is in crab mode
*     Set all drive motors to drive_stick_y
*   Else
*     If top is pressed or crabbed right
*       adjust so right side of robot is now front of robot
*     Else if crabbed left
*       adjust so left side of robot is now front of robot
*     Endif
*   Endif
*   If turbo button is not pressed slow the drive motors by 50%
*******************************************************************************/
void  drive_stick_input(MotorValsType *motor_vals)
{
  static int oi_drive_y_prev = 127;
  static UINT8 s_prev_steer_mode=DEADZONE_STEER_MODE;

  unsigned char  right_drive_speed, left_drive_speed;
  INT16         tmp;
  CcReturnType   cc_ret_val;
     

  /* Read our current theta from the CC */
  cc_ret_val = cc_get_heading(&sg_curr_theta);

  if(cc_ret_val != CC_SUCCESS)
  {
     /* No CC available, set theta to the special INVALID_THETA val */
     sg_curr_theta = INVALID_THETA;
  }
  else if(sg_curr_theta == INVALID_THETA)
  {
     /* INVALID_THETA is special, if returned add one to it to keep it valid */
     sg_curr_theta = INVALID_THETA+1;
  }


  /* create deadzone in drive stick */
  if ((Oi_drive_y < (127 + DRIVE_DEADZONE)) &&
      (Oi_drive_y > (127 - DRIVE_DEADZONE)))
  {
    Oi_drive_y = 127;
  }

  if ((Oi_drive_x < (127 + DRIVE_DEADZONE)) &&
      (Oi_drive_x > (127 - DRIVE_DEADZONE)))
  {
    Oi_drive_x = 127;
  }

  /* Adjust Oi_drive_x/y to accelerate/decelerate */
  Oi_drive_y = drive_acceleration_adjust(Oi_drive_y, oi_drive_y_prev);

  /* if the crab stick is in the middle use tank style steering */
  if ((Oi_crab_x > (127 - CRAB_MODE_DEADZONE)) &&
      (Oi_crab_x < (127 + CRAB_MODE_DEADZONE)) &&
      (Oi_crab_y > (127 - CRAB_MODE_DEADZONE)))
  {
    /*---------------------------- tank steer mode-------------------- */
    right_drive_speed = Limit_Mix(2000 + Oi_drive_y + Oi_drive_x - 127);
    left_drive_speed = Limit_Mix(2000 + Oi_drive_y - Oi_drive_x + 127);

    motor_vals->rf_wheel = (int)right_drive_speed - 127;
    motor_vals->rb_wheel = (int)right_drive_speed - 127;
    motor_vals->lf_wheel = (int)left_drive_speed - 127;
    motor_vals->lb_wheel = (int)left_drive_speed - 127;

    s_prev_steer_mode=DEADZONE_STEER_MODE;
    sg_locked_in_theta = INVALID_THETA;
#if 0 
    if(Oi_crab_y < (127 + CRAB_MODE_DEADZONE))
    {
      /* Y is in the Deadzone...we are completely in the deadzone */
      s_prev_steer_mode=DEADZONE_STEER_MODE;
      s_locked_in_theta = INVALID_THETA;
    }
    else
    {
       /* We are driving tank style */
    if((Oi_drive_x == 127) && (s_prev_steer_mode != TANK_STEER_MODE))
    {
       /* Driver is driving straight and just entered TANK mode...
        * lock in theta to drive straight.
        */
        s_locked_in_theta = curr_theta;
    }
    else if(Oi_drive
    {
       /* Since we are tanking, there is no theta lock */
       s_locked_in_theta=INVALID_THETA;
      /* We are in Tank mode now */ 
      s_prev_steer_mode=TANK_STEER_MODE;
    }
#endif
    /*------------------------ END tank steer mode-------------------- */
  }
  else
  {
    /*------------------------------ crab steer mode ---------------- */
    if(s_prev_steer_mode != CRAB_STEER_MODE)
    {
       /* Since we just entered CRAB steering, lock in our current theta b/c
        * we don't want the robot getting skewed as we crab.
        */
       sg_locked_in_theta = sg_curr_theta;
       printf("CRAB-locking in %d ", (int)sg_curr_theta);
    }
    
    /* scaling for BUPOD wheel speed reduction will be done elsewhere */
    /* Set BUPOD speed and let SetHeading adjust them to keep the robot straight */
    motor_vals->lf_wheel = (int)Oi_drive_y - 127;
    motor_vals->rf_wheel = (int)Oi_drive_y - 127;
    motor_vals->lb_wheel = (int)Oi_drive_y - 127;
    motor_vals->rb_wheel = (int)Oi_drive_y - 127;

    s_prev_steer_mode=CRAB_STEER_MODE;  /* We are in Crab Mode */
    /*-------------------------- END crab steer mode ---------------- */
  }

  /* Anti-Turbo - if the turbo button is not pressed, slow the drive motors
     down by 50% */
  if (Oi_sw_turbo == 0)
  {
    tmp = ((int)motor_vals->lf_wheel *
           DRIVE_SCALE_NUMERATOR / DRIVE_SCALE_DENOMINATOR);
    motor_vals->lf_wheel = (UINT8)MIN_RETURN(tmp, -127);
    motor_vals->lf_wheel = (UINT8)MAX_RETURN(tmp, 127);

    tmp = ((int)motor_vals->rf_wheel *
           DRIVE_SCALE_NUMERATOR / DRIVE_SCALE_DENOMINATOR);
    motor_vals->rf_wheel = (UINT8)MIN_RETURN(tmp, -127);
    motor_vals->rf_wheel = (UINT8)MAX_RETURN(tmp, 127);

    tmp = ((int)motor_vals->lb_wheel *
           DRIVE_SCALE_NUMERATOR / DRIVE_SCALE_DENOMINATOR);
    motor_vals->lb_wheel = (UINT8)MIN_RETURN(tmp, -127);
    motor_vals->lb_wheel = (UINT8)MAX_RETURN(tmp, 127);

    tmp = ((int)motor_vals->rb_wheel *
           DRIVE_SCALE_NUMERATOR / DRIVE_SCALE_DENOMINATOR);
    motor_vals->rb_wheel = (UINT8)MIN_RETURN(tmp, -127);
    motor_vals->rb_wheel = (UINT8)MAX_RETURN(tmp, 127);
  }

  /* Update prev value so we can use it next loop */
  oi_drive_y_prev = Oi_drive_y;

  return;
}


/*******************************************************************************
* FUNCTION NAME: drive_acceleration_adjust
* PURPOSE:       Adjust 
* CALLED FROM:   drive_stick_input()
* ARGUMENTS:
*
* RETURNS:       none
* DOL:
******************************************************************************/
int  drive_acceleration_adjust(int desired_wheel_speed, int curr_wheel_speed)
{
  int new_wheel_speed;

  if (curr_wheel_speed >= (desired_wheel_speed -  DRIVE_ACCEL_RATE) &&
      curr_wheel_speed <= (desired_wheel_speed + DRIVE_ACCEL_RATE))
  {
  /* if current speed is close to target speed set it exactly to target speed */
    new_wheel_speed = desired_wheel_speed;
  }
  else if (curr_wheel_speed < desired_wheel_speed)
  {
    /* make sure the current speed doesn't wrap around */
    MAX(curr_wheel_speed, (254 - DRIVE_ACCEL_RATE));
    /* add to the current speed */
    new_wheel_speed = (curr_wheel_speed + DRIVE_ACCEL_RATE);
  }
  else
  {
    /* subtract from the current speed */
    MIN(curr_wheel_speed, DRIVE_ACCEL_RATE);
    new_wheel_speed = (curr_wheel_speed - DRIVE_ACCEL_RATE);
  }

  return new_wheel_speed;
}

/*******************************************************************************
* FUNCTION NAME: scale_bupod_speed
* PURPOSE:       Decrease the speed of stationary bupods while crabbing
* CALLED FROM:   
* ARGUMENTS:     motor_vals - speed of motors
*                calibration_vals - calibration of crab pot
* RETURNS:       none
*******************************************************************************/
void scale_bupod_speed(MotorValsType *motor_vals,
                       CalibrationValsType *calibration_vals)
{
  UINT8 crab_angle;

  /* get crab angle in pot ticks */
  crab_angle = GET_ANALOG_VALUE_SHIFT(Analog_in_crab_pot);

  /* crab_angle is now in pot ticks */
  /* scale to brads */
  crab_angle = pots_to_brads(crab_angle, calibration_vals->crab_fwd, 
                             calibration_vals->crab_180_left, 
                             calibration_vals->crab_180_right);
  /* crab_angle is now in brads */

  /* converted from 127 meaning straight ahead and 0 full to the right and 255
     full to the left, to 0 meaning straight ahead and 127 being full left and
     full right */
  if (crab_angle <= 127)
  {
    crab_angle = 127 - crab_angle;
  }
  else
  {
    crab_angle = crab_angle - 127;
  }

  if (Oi_sw_crab_manu_feed_sel == OI_CRAB_FEEDBACK_SELECTOR)
  {
    int lb_signed_speed;
    int rb_signed_speed;

    /* only scale bupods in crab feedback mode */
    /* Scale by cos of crab angle (which is multiplied by 256)
       divide by 256 to get true cos value*/
    
    /* Convert to int */ 
    lb_signed_speed = (int)motor_vals->lf_wheel;
    rb_signed_speed = (int)motor_vals->rf_wheel;

    /* Now scale the speed of the BUPODs to be the Y component */
    lb_signed_speed = (int)(lb_signed_speed * cos(crab_angle)) / 256;
    rb_signed_speed = (int)(rb_signed_speed * cos(crab_angle)) / 256;

    /* Non-invalid theta means we need to try to track to our locked in value */
    /* Assuming the switch to enable it is flipped */
    if((sg_locked_in_theta != INVALID_THETA) && (Oi_sw_drive_straight == 0) &&
       (lb_signed_speed != 0) && (rb_signed_speed != 0))
    {
       INT8   signed_theta_diff;

       /* Signed math ensures this give the corret dist to target */
       signed_theta_diff = sg_locked_in_theta - sg_curr_theta; 

       /* Set the Min diff so we compensate enough to correct always */
       if((signed_theta_diff<0) && (signed_theta_diff > -(MIN_THETA_DIFF)))
       {
          signed_theta_diff = -(MIN_THETA_DIFF);
       }
       if((signed_theta_diff>0) && (signed_theta_diff < MIN_THETA_DIFF))
       {
          signed_theta_diff = MIN_THETA_DIFF;
       }

       /* Scale up the diff to correct faster */
       signed_theta_diff = signed_theta_diff * 3;

       printf("Org-LB=%d RB=%d ",(int)motor_vals->lb_wheel, (int)motor_vals->rb_wheel);
       printf("SignLB=%d RB=%d ",(int)lb_signed_speed,(int)rb_signed_speed);
       /* We have a locked in heading, adjust motor vals to achieve our desired heading */
       printf("CRAB-tracking to %d (at %d) diff=%d ", (int)sg_locked_in_theta, (int)sg_curr_theta,
              (int)signed_theta_diff);

       lb_signed_speed = lb_signed_speed - signed_theta_diff;
       rb_signed_speed = rb_signed_speed + signed_theta_diff;
       
       printf("New-LB=%d RB=%d\n",(int)lb_signed_speed+127, (int)rb_signed_speed+127);
    
    }

    /* Convert back to PWM values, cap at the 127s to prevent wrap around */
    MAX(lb_signed_speed,127);
    MAX(rb_signed_speed,127);
    MIN(lb_signed_speed,-127);
    MIN(rb_signed_speed,-127);
    motor_vals->lb_wheel = lb_signed_speed;
    motor_vals->rb_wheel = rb_signed_speed;

  }

#if 0
  {
    /* this code has not been changed to use negative motor speeds */
    UINT8 tmp_speed;

    tmp_speed = ((int)motor_vals->lf_wheel - 127) / 4;

    printf(" crab angle %d", (int)crab_angle);

    if ((crab_angle >= 61) && (crab_angle < 67))
    {
      printf(" crabbed 90 tmp_speed %d", (int)tmp_speed);

      if (GET_ANALOG_VALUE_SHIFT(Analog_in_crab_pot) < 127)
      {
        printf(" right");
        motor_vals->rb_wheel += tmp_speed;
        motor_vals->lb_wheel -= tmp_speed;
      }
      else
      {
        printf(" left");
        motor_vals->rb_wheel -= tmp_speed;
        motor_vals->lb_wheel += tmp_speed;
      }

      printf(" r %d l %d", (int)motor_vals->rb_wheel,
             (int)motor_vals->lb_wheel);
    }

    printf("\n");
  }
#endif


  return;
}



/*******************************************************************************
* FUNCTION NAME: pots_to_brads
* PURPOSE:       convert from pot ticks to brads
* CALLED FROM:   
* ARGUMENTS:     crab_angle_pots - crab angle in pot ticks
*                middle_value - pot value when crabs are straight ahead
*                max_constraint - pot value when crabs are 180 deg left
*                min_constraint - pot value when crabs are 180 deg right
* RETURNS:       crab angle in brads
*******************************************************************************/
UINT8 pots_to_brads(UINT8 crab_angle_pots, UINT8 middle_value,
                    UINT8 max_constraint, UINT8 min_constraint)
{
  UINT8 crab_angle_brads;

  /* Cap the pot values at their max and min so the equation below will work
     correctly */
  MIN(crab_angle_pots, min_constraint);
  MAX(crab_angle_pots, max_constraint);

  /* scale so ticks go to full 0-255 binary degree range */
  if (crab_angle_pots >= middle_value)
  {
    /* Brads = Brads + ((Pot Ticks - Pot Ticks) * Brads /
                        (Pot Ticks - Pot Ticks)) */
    crab_angle_brads = 127 + ((crab_angle_pots - middle_value) * (int)64 /
                              (max_constraint - middle_value));
  }
  else
  {
    /* Brads = Brads - ((Pot Ticks - Pot Ticks) * Brads /
                        (Pot Ticks - Pot Ticks)) */
    crab_angle_brads = 127 - ((middle_value - crab_angle_pots) * (int)64 /
                              (middle_value - min_constraint));
  }

  return (crab_angle_brads);
}


