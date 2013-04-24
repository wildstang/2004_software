/*******************************************************************************
* FILE NAME: ws_code.c
*
* DESCRIPTION:
*
* USAGE:
*
*******************************************************************************/

#include "ifi_aliases.h"
#include "ifi_default.h"
#include "ifi_utilities.h"
#include "serial_utils.h"
#include "user_routines.h"
#include "printf_lib.h"
#include "delays.h"       /*defined locally*/
#include "oi_data.h" /* For OI Data from slave RC */
#include "ws_code.h"


/*******************************************************************************
* FUNCTION NAME: test_pots
* PURPOSE:       Range-checks the pots for illegal values signifying a failure
* CALLED FROM:   Process_Data_From_Master_uP()
* ARGUMENTS:     right_pot - value of right pot
*                left_pot  - value of left pot
* RETURNS:       TRUE if pots are good, FALSE if pots are bad
*******************************************************************************/
int test_pots(unsigned char right_pot, unsigned char left_pot)
{
  if ((right_pot == 0) || (left_pot == 0) ||
      (right_pot >= 254) || (left_pot >= 254))
  {
    return (FALSE);
  }

  return (TRUE);
}



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
void  drive_stick_input(oi_data *OIData, MotorValsType *motor_vals)
{
  unsigned char right_drive_speed, left_drive_speed;
  UINT16 tmp;

  /* create deadzone in drive stick */
  if ((OIData->p4_aux < (127 + DRIVE_DEADZONE)) &&
      (OIData->p4_aux > (127 - DRIVE_DEADZONE)))
  {
    OIData->p4_aux = 127;
  }

  if ((OIData->p4_wheel < (127 + DRIVE_DEADZONE)) &&
      (OIData->p4_wheel > (127 - DRIVE_DEADZONE)))
  {
    OIData->p4_wheel = 127;
  }


  /* if the crab stick is in the middle, or if it is near 90 degrees then use
     tank style steering */
  if (((OIData->p4_x > (127 - CRAB_MODE_DEADZONE)) &&
       (OIData->p4_x < (127 + CRAB_MODE_DEADZONE)) &&
       (OIData->p4_y > (127 - CRAB_MODE_DEADZONE)) &&
       (OIData->p4_y < (127 + CRAB_MODE_DEADZONE))) ||
      ((OIData->p4_x > (JOYSTICK_MAX_VALUE - CRAB_MODE_DEADZONE)) &&
       (OIData->p4_y > (127 - CRAB_MODE_DEADZONE))) ||
      ((OIData->p4_x < (JOYSTICK_MIN_VALUE + CRAB_MODE_DEADZONE)) &&
       (OIData->p4_y > (127 - CRAB_MODE_DEADZONE))))
  {
    /* tank steer mode */
    right_drive_speed = Limit_Mix(2000 + OIData->p4_aux +
                                  OIData->p4_wheel - 127);
    left_drive_speed = Limit_Mix(2000 + OIData->p4_aux -
                                 OIData->p4_wheel + 127);

    if ((OIData->swB.p2_p4_sw.p34_top == 1) ||
        (OIData->p4_x < (JOYSTICK_MIN_VALUE + CRAB_MODE_DEADZONE)))
    {
      /* if the top button is pressed or we are crabbed right then adjust the
         target by 90 degrees so the front of the robot is changed to the right
         side */
      /* since we are 90 degrees adjusted, our right side is the front and left
         side is the back */
      motor_vals->lf_wheel = left_drive_speed;
      motor_vals->rf_wheel = left_drive_speed;
      motor_vals->lb_wheel = right_drive_speed;
      motor_vals->rb_wheel = right_drive_speed;

      /* if the front skid pad is engaged, we need to disengage it and engage
         the 90 degree skid pad */
    }
    else if (OIData->p4_x < (JOYSTICK_MAX_VALUE - CRAB_MODE_DEADZONE))
    {
      /* if we are crabbed left then adjust the target by 90 degrees so the
         front of the robot is changed to the left side */
      motor_vals->lf_wheel = right_drive_speed;
      motor_vals->rf_wheel = right_drive_speed;
      motor_vals->lb_wheel = left_drive_speed;
      motor_vals->rb_wheel = left_drive_speed;

      /* if the front skidpad is engaged, we need to disengage it and engage
         the 90 degree skid pad */
    }
    else
    {
      motor_vals->rf_wheel = right_drive_speed;
      motor_vals->rb_wheel = right_drive_speed;
      motor_vals->lf_wheel = left_drive_speed;
      motor_vals->lb_wheel = left_drive_speed;
    }
  }
  else
  {
    /* crab steer mode */
    motor_vals->lf_wheel = OIData->p4_aux;
    motor_vals->rf_wheel = OIData->p4_aux;
    motor_vals->lb_wheel = OIData->p4_aux;
    motor_vals->rb_wheel = OIData->p4_aux;
  }

  /* Anti-Turbo - if the turbo button is not pressed, slow the drive motors
     down by 50% */
  if (OIData->swB.p2_p4_sw.p34_aux2 == 0)
  {
    tmp = ((int)motor_vals->lf_wheel * DRIVE_SCALE_50 / 100) + DRIVE_CON_50;
    motor_vals->lf_wheel = (UINT8)MAX_RETURN(tmp, 254);

    tmp = ((int)motor_vals->rf_wheel * DRIVE_SCALE_50 / 100) + DRIVE_CON_50;
    motor_vals->rf_wheel = (UINT8)MAX_RETURN(tmp, 254);

    tmp = ((int)motor_vals->lb_wheel * DRIVE_SCALE_50 / 100) + DRIVE_CON_50;
    motor_vals->lb_wheel = (UINT8)MAX_RETURN(tmp, 254);

    tmp = ((int)motor_vals->rb_wheel * DRIVE_SCALE_50 / 100) + DRIVE_CON_50;
    motor_vals->rb_wheel = (UINT8)MAX_RETURN(tmp, 254);
  }

  return;
}



/*******************************************************************************
* FUNCTION NAME: crab_manu_override
* PURPOSE:       Move the crab modules manually without feedback
* CALLED FROM:   Process_Data_From_Master_uP()
* ARGUMENTS:     OIData - crab stick x, top & trigger or crab stick
*                motor_vals - crab motors
* RETURNS:       none
* DOL:
*   If the top button is pressed, move the right crab modules
*   If the trigger button is pressed, move the left crab modules
*******************************************************************************/
void  crab_manu_override(oi_data *OIData, MotorValsType *motor_vals)
{
  unsigned char manu_crab_speed;

  manu_crab_speed = OIData->p4_x;

  /* limit speed in manual mode */
  MIN(manu_crab_speed, (127 - CRAB_MANU_SPEED_MAX));
  MAX(manu_crab_speed, (127 + CRAB_MANU_SPEED_MAX));

  /* move right crab modules if top is pressed */
  if (OIData->swB.p2_p4_sw.p34_top == 1)
  {
    motor_vals->right_crab = manu_crab_speed;
  }
  else
  {
    motor_vals->right_crab = 127;
  }

  /* move left crab modules if trigger is pressed */
  if (OIData->swB.p2_p4_sw.p34_trig == 1)
  {
    motor_vals->left_crab = manu_crab_speed;
  }
  else
  {
    motor_vals->left_crab = 127;
  }

  return;
}



/*******************************************************************************
* FUNCTION NAME: crab_stick_input
* PURPOSE:       Convert the crab stick position to crab module targets
* CALLED FROM:   Process_Data_From_Master_uP()
* ARGUMENTS:     OIData - crab stick x & y
*                crab_targets - right & left crab targets
* RETURNS:       none
* DOL:
*   If the crab stick is above the 1/2 axis divider
*     call one_axis_crab_steering()
*   Else
*     call two_axis_crab_steering()
*   Endif
*******************************************************************************/
void  crab_stick_input(oi_data *OIData, CrabTargetsType *crab_targets)
{
  static unsigned char prev_loop_crab_pos = 0;

  /* determine if the crab stick is in 1 or 2 axis mode */
  if (OIData->p4_y >= ONE_TWO_AXIS_CRAB_DIVIDER)
  {
    /* printf("one axis  "); */
    one_axis_crab_steering(OIData, crab_targets);
    prev_loop_crab_pos = TWO_AXIS_CENTER;
  }
  else
  {
    /* printf("two axis  "); */
    two_axis_crab_steering(OIData, crab_targets, &prev_loop_crab_pos);
  }

  /* pigeon-toe the crabs if top is pressed */
  if (OIData->swB.p2_p4_sw.p34_top == 1)
  {
    crab_targets->right_crab_tar -= 12;
    crab_targets->left_crab_tar += 12;
  }

  return;
}




/*******************************************************************************
* FUNCTION NAME: one_axis_crab_steering
* PURPOSE:       Convert the one axis crab stick position to crab module targets
* CALLED FROM:   crab_stick_input()
* ARGUMENTS:     OIData - crab stick x
*                crab_targets - right & left crab targets
* RETURNS:       none
* DOL:
*   Scale the joystick so the range is 0-255
*   If the crab stick in in the deadzone
*     set the target to 127
*   Else if the stick is to the left
*     set target to the left
*   Else stick is to the right
*     set target to the right
*******************************************************************************/
void one_axis_crab_steering(oi_data *OIData, CrabTargetsType *crab_targets)
{
  /* since the joystick only goes from 13-250 we should scale this to 0-255 */
  if (OIData->p4_x > 127)
  {
    /* make sure joystick doesnt go over max */
    MAX(OIData->p4_x, JOYSTICK_MAX_VALUE);

    /* Scale joystick max to 255 */
    OIData->p4_x = (127 + (((int)OIData->p4_x - 127) * 127 /
                           JOYSTICK_TRAVEL_HI));
  }
  else
  {
    /* make sure joystick doesnt go under min */
    MIN(OIData->p4_x, JOYSTICK_MIN_VALUE);

    /* scale joystick min to 0 */
    OIData->p4_x = (127 - ((127 - (int)OIData->p4_x) * 127 /
                           JOYSTICK_TRAVEL_LOW));
  }

  if ((OIData->p4_x > (127 - ONE_AXIS_DEADZONE)) &&
      (OIData->p4_x < (127 + ONE_AXIS_DEADZONE)))
  {
    /* keep wheel straight */
    crab_targets->right_crab_tar = 127;
    crab_targets->left_crab_tar = 127;
  }
  else
  {
    if (OIData->p4_x > (127 + ONE_AXIS_DEADZONE))
    {
      /* turn wheels left */
      /* scale so we turn 180 degrees to 90 degrees */
      crab_targets->right_crab_tar = 127 + ((OIData->p4_x - 127) / 2);
      crab_targets->left_crab_tar = crab_targets->right_crab_tar;
    }
    else if (OIData->p4_x < (127 - ONE_AXIS_DEADZONE))
    {
      /* turn wheels right */
      /* scale so we turn 180 degrees to 90 degrees */
      crab_targets->right_crab_tar = 127 - ((127 - OIData->p4_x) / 2);
      crab_targets->left_crab_tar = crab_targets->right_crab_tar;
    }
  }

  return;
}



/*******************************************************************************
* FUNCTION NAME: two_axis_crab_steering
* PURPOSE:       Convert the two axis crab stick position to crab module targets
* CALLED FROM:   crab_stick_input()
* ARGUMENTS:     OIData - crab stick x & y
*                crab_targets - right & left crab targets
* RETURNS:       none
* DOL:
*******************************************************************************/
void two_axis_crab_steering(oi_data *OIData, CrabTargetsType *crab_targets,
                            unsigned char *prev_loop_crab_pos)
{
  UINT16 tmp;

  static unsigned char prev_loop_dir = 0;

  if ((OIData->p4_x > 127 - TWO_AXIS_DEADZONE) &&
      (OIData->p4_x < 127 + TWO_AXIS_DEADZONE) &&
      (OIData->p4_y > 127 - TWO_AXIS_DEADZONE) &&
      (OIData->p4_y < 127 + TWO_AXIS_DEADZONE))
  {
    crab_targets->left_crab_tar = 127;
  }
  else
  {
    /* Must figure out which quadrant the stick is in and take the difference
       between stick position and center of stick to get Opposite & Adjacent.
       Get angle from ArcTan subroutine. Convert from degrees to binary
       degrees. Offset result to put angle in correct quadrant */
    if (OIData->p4_x < 127)
    {
      if (OIData->p4_y > 127)
      {
        /* Quadrant 1 */
        crab_targets->left_crab_tar = 63 + arctan(OIData->p4_y - 127,
                                                  127 - OIData->p4_x);
      }
      else
      {
        /* Quadrant 4 */
        crab_targets->left_crab_tar = arctan(127 - OIData->p4_x,
                                             127 - OIData->p4_y);
      }
    }
    else
    {
      if (OIData->p4_y > 127)
      {
        /* Quadrant 2 */
        crab_targets->left_crab_tar = 127 + arctan(OIData->p4_x - 127,
                                                   OIData->p4_y - 127);
      }
      else
      {
        /* Quadrant 3 */
        tmp = 191 + arctan(127 - OIData->p4_y, OIData->p4_x - 127);
        crab_targets->left_crab_tar = (UINT8)MAX_RETURN(tmp, 255);
      }
    }
  }

  /* We need to now handle the case where the driver moves the joystick from
     lower left to lower right. We don't want the crabs to wip around in this
     case. So, the only way to go from the lower left to the lower right (or
     vise versa) is to go through the center (191-64) first. */
  /* If previously we were on the left side of the stick but the new value is
     to the right then we want to set the target to be the largest value on the
     left. */
  if ((*prev_loop_crab_pos == TWO_AXIS_NOT_CENTER) &&
      (prev_loop_dir == TWO_AXIS_LEFT) &&
      (crab_targets->left_crab_tar < STICK_RIGHT_SIDE))
  {
    crab_targets->left_crab_tar = 254;
  }
  else
  {
    /* Now make sure the same problem is happening on the right side */
    if ((*prev_loop_crab_pos == TWO_AXIS_NOT_CENTER) &&
        (prev_loop_dir == TWO_AXIS_RIGHT) &&
        (crab_targets->left_crab_tar < STICK_LEFT_SIDE))
    {
      crab_targets->left_crab_tar = 0;
    }
  }

  /* since we only set the left target in the above cases, we must now give the
     right target the same value */
  crab_targets->right_crab_tar = crab_targets->left_crab_tar;

  /* This stores the joysticks previous loop value */
  if (crab_targets->left_crab_tar > STICK_LEFT_SIDE)
  {
    *prev_loop_crab_pos = TWO_AXIS_LEFT;
    prev_loop_dir = TWO_AXIS_NOT_CENTER;
  }
  else
  {
    if (crab_targets->left_crab_tar < STICK_RIGHT_SIDE)
    {
      *prev_loop_crab_pos = TWO_AXIS_RIGHT;
      prev_loop_dir = TWO_AXIS_NOT_CENTER;
    }
    else
    {
      *prev_loop_crab_pos = TWO_AXIS_CENTER;
    }
  }

  return;
}



/*******************************************************************************
* FUNCTION NAME: crab_system
* PURPOSE:       Move crab modules based on crab targets
* CALLED FROM:   Process_Data_From_Local_IO()
* ARGUMENTS:     crab_targets - right & left crab targets
*                motor_vals - speed of crab motors
* RETURNS:       none
* DOL:
*   Scale target from binary degrees to pot values
*   Range check target values to make sure they're within safe range of pot
*   Feedback loop on left crab
*   Feedback loop on right crab
*******************************************************************************/
void crab_system(CrabTargetsType *crab_targets, MotorValsType *motor_vals)
{
  /* scale from binary degrees to pot ticks */
  /* at this point the left & right targets are in brads */

  printf("crabTar BRAD R %d L %d\n", (int)crab_targets->left_crab_tar,
         (int)crab_targets->right_crab_tar);

  crab_targets->left_crab_tar = crab_scaling(crab_targets->left_crab_tar,
                                             POT_LCRAB_LEFT_180,
                                             POT_LCRAB_RIGHT_180);
  crab_targets->right_crab_tar = crab_scaling(crab_targets->right_crab_tar,
                                              POT_RCRAB_LEFT_180,
                                              POT_RCRAB_RIGHT_180);
  /* at this point the left & right targets are in pot ticks */

  printf("crabTar POT  R %d L %d\n", (int)crab_targets->left_crab_tar,
         (int)crab_targets->right_crab_tar);

  /* make sure pot target is not outside of safe range of pot */
  MAX(crab_targets->left_crab_tar, POT_CRAB_LEFT_LIMIT);
  MIN(crab_targets->left_crab_tar, POT_CRAB_RIGHT_LIMIT);
  MAX(crab_targets->right_crab_tar, POT_CRAB_LEFT_LIMIT);
  MIN(crab_targets->right_crab_tar, POT_CRAB_RIGHT_LIMIT);

  /* call feedback loops for right & left crabs */
  motor_vals->left_crab =
    crab_feedback_left(Get_Analog_Value(ANALOG_IN_LEFT_POT),
                       crab_targets->left_crab_tar);
  motor_vals->right_crab =
    crab_feedback_right(Get_Analog_Value(ANALOG_IN_RIGHT_POT),
                        crab_targets->right_crab_tar);

  return;
}



/*******************************************************************************
* FUNCTION NAME: crab_scaling
* PURPOSE:       Convert from binary degrees to pot values
* CALLED FROM:   crab_system()
* ARGUMENTS:     
* RETURNS:       target (in pot values) of crab module
* DOL:
*******************************************************************************/
unsigned char crab_scaling(unsigned char crab_tar_brads,
                           unsigned char left_constraint,
                           unsigned char right_constraint)
{
  unsigned char crab_tar_pot_vals;

  /* scale so full 0-255 binary degree range maps to pot range */
  if (crab_tar_brads >= 127)
  {
    crab_tar_pot_vals = 127 + (((int)crab_tar_brads - 127) *
                               ((int)left_constraint - 127) / 128);
  }
  else
  {
    crab_tar_pot_vals = 127 - ((127 - (int)crab_tar_brads) *
                               (127 - (int)right_constraint) / 128);
  }

  return (crab_tar_pot_vals);
}


/*******************************************************************************
* FUNCTION NAME: crab_feedback_left
* PURPOSE:       feedback loop for left crab modules
* CALLED FROM:   crab_system()
* ARGUMENTS:     lcrab_pos - position of left crab modules
*                lcrab_tgt - target of left crab modules
* RETURNS:       speed of left crab motor
* DOL:
*******************************************************************************/
unsigned char crab_feedback_left(unsigned char lcrab_pos,
                                 unsigned char lcrab_tgt)
{
  unsigned char lcrab_speed;
  unsigned char target_diff;

  if (lcrab_tgt > lcrab_pos)
  {
    target_diff = lcrab_tgt - lcrab_pos;

    if (target_diff < CRAB_POSITION_NEAR_LEFT)
    {
      lcrab_speed = 127;
    }
    else if (target_diff < CRAB_POSITION_CLOSE_LEFT)
    {
      lcrab_speed = 127 + CRAB_SLOW_LEFT;
    }
    else if (target_diff < CRAB_POSITION_FAR_LEFT)
    {
      lcrab_speed = 127 + CRAB_MED_LEFT;
    }
    else
    {
      lcrab_speed = 127 + CRAB_FAST_LEFT;
    }
  }
  else if (lcrab_pos > lcrab_tgt)
  {
    target_diff = lcrab_pos - lcrab_tgt;

    if (target_diff < CRAB_POSITION_NEAR_LEFT)
    {
      lcrab_speed = 127;
    }
    else if (target_diff < CRAB_POSITION_CLOSE_LEFT)
    {
      lcrab_speed = 127 - CRAB_SLOW_LEFT;
    }
    else if (target_diff < CRAB_POSITION_FAR_LEFT)
    {
      lcrab_speed = 127 - CRAB_MED_LEFT;
    }
    else
    {
      lcrab_speed = 127 - CRAB_FAST_LEFT;
    }
  }
  else
  {
    lcrab_speed = 127;
  }

  return (lcrab_speed);
}



/*******************************************************************************
* FUNCTION NAME: crab_feedback_right
* PURPOSE:       feedback loop for right crab modules
* CALLED FROM:   crab_system()
* ARGUMENTS:     rcrab_pos - position of right crab modules
*                rcrab_tgt - target of right crab modules
* RETURNS:       speed of right crab motor
* DOL:
*******************************************************************************/
unsigned char crab_feedback_right(unsigned char rcrab_pos,
                                  unsigned char rcrab_tgt)
{
  unsigned char rcrab_speed;
  unsigned char target_diff;

  if (rcrab_tgt > rcrab_pos)
  {
    target_diff = rcrab_tgt - rcrab_pos;

    if (target_diff < CRAB_POSITION_NEAR_RIGHT)
    {
      rcrab_speed = 127;
    }
    else if (target_diff < CRAB_POSITION_CLOSE_RIGHT)
    {
      rcrab_speed = 127 + CRAB_SLOW_RIGHT;
    }
    else if (target_diff < CRAB_POSITION_FAR_RIGHT)
    {
      rcrab_speed = 127 + CRAB_MED_RIGHT;
    }
    else
    {
      rcrab_speed = 127 + CRAB_FAST_RIGHT;
    }
  }
  else if (rcrab_pos > rcrab_tgt)
  {
    target_diff = rcrab_pos - rcrab_tgt;

    if (target_diff < CRAB_POSITION_NEAR_RIGHT)
    {
      rcrab_speed = 127;
    }
    else if (target_diff < CRAB_POSITION_CLOSE_RIGHT)
    {
      rcrab_speed = 127 - CRAB_SLOW_RIGHT;
    }
    else if (target_diff < CRAB_POSITION_FAR_RIGHT)
    {
      rcrab_speed = 127 - CRAB_MED_RIGHT;
    }
    else
    {
      rcrab_speed = 127 - CRAB_FAST_RIGHT;
    }
  }
  else
  {
    rcrab_speed = 127;
  }

  return (rcrab_speed);
}



/*******************************************************************************
* FUNCTION NAME: arctan
* PURPOSE:       return arctan of opposite / adjecent pair
* CALLED FROM:   two_axis_crab_steering()
* ARGUMENTS:     opposite - opposite side of triangle
*                adjacent - adjacent side of triangle
* RETURNS:       arctan of opposite / adjacent pair
* DOL:
*******************************************************************************/
unsigned char arctan(unsigned char opposite, unsigned char adjacent)
{
  static unsigned char arctan_lookup[65] = { 0, 1, 1, 2, 3, 3, 4, 4, 5, 6,
                                             6, 7, 8, 8, 9, 9,10,11,11,12,
                                            12,13,13,14,15,15,16,16,17,17,
                                            18,18,19,19,20,20,21,21,22,22,
                                            23,23,24,24,25,25,25,26,26,27,
                                            27,27,28,28,29,29,29,30,30,30,
                                            31,31,31,32,32};
  unsigned char arctan_val;
  unsigned char tmp;
  unsigned char swap_angle_flag;

  if ((opposite == 0) && (adjacent == 0))
  {
    /* passed in opposite & adjacent are both zero...should not happen but keep
       from a divide by zero and just return angle of zero */
    arctan_val = 0;
  }
  else
  {
    /* We always want our opposite to be smaller than our adjacent so we only
       have to deal with 45 degrees or smaller.  If the passed in opposite is
       larger, we simply reverse them and calculate the other angle prior to
       output. */
    if (opposite < adjacent)
    {
      /* passed in opposite is smaller than adjacent...perfect! */
      swap_angle_flag = FALSE;
    }
    else
    {
      /* passed in opposite is larger than adjacent...swap it and set the swap
         angle flag so we will calculate the exact angle prior to output */
      tmp = opposite;
      opposite = adjacent;
      adjacent = tmp;
      swap_angle_flag = TRUE;
    }

    /* this is a TAN lookup table that will convert (opposite/adjacent)*64 into
       an angle from 0-32 brads (45 degrees).  Since the angle must be less
       than 45 degrees, the opposite must be less than the adjacent side.
       NOTE: (a << 6) is the same as (a * 64), but the shifting is much faster
       in hardware */
    arctan_val = arctan_lookup[(opposite << 6) / adjacent];

    /* if we swapped the opposite & adjacent then we have to convert to the
       other angle in the triange.  This is simple, just subtract 90 degrees
       (60 brads). */
    if (swap_angle_flag == TRUE)
    {
      /* swap flag was set so calculate the other angle in the triangle */
      arctan_val = 64 - arctan_val;
    }
  }

  return (arctan_val);
}


