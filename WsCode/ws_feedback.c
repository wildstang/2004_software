/*******************************************************************************
* FILE NAME: ws_feedback.c
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

#include "ws_feedback.h"


/*******************************************************************************
* FUNCTION NAME: general_pos_feedback
* PURPOSE:       Drive appendage to the specified position
* CALLED FROM:
* ARGUMENTS:     desired_pos - desired position of appendage
*                curr_pos - current position of appendage
* RETURNS:       PWM value to cause the desired position
*******************************************************************************/
INT8 general_pos_feedback(UINT8 curr_pos, UINT8 desired_pos,
                          static const rom FeedbackConstsType *consts)
{
  INT8  motor_speed;
  UINT8 target_diff;

  if (desired_pos > curr_pos)
  {
    /* The pot values need to increase */
    target_diff = desired_pos - curr_pos;

    if (target_diff < consts->pos_near_inc)
    {
      /* We are close enough, don't move any more */
      motor_speed = 0;
    }
    else if (target_diff < consts->pos_close_inc)
    {
      /* We are close, move at a fixed speed */
      motor_speed = consts->speed_slow_inc;
    }
    else if (target_diff < consts->pos_far_inc)
    {
      /* We are in the mid range, scale based on how far we are */
      motor_speed = (consts->speed_slow_inc +
                     (int)(consts->speed_fast_inc - consts->speed_slow_inc) *
                     (int)(target_diff - consts->pos_close_inc) /
                     (int)(consts->pos_far_inc - consts->pos_close_inc));
    }
    else
    {
      /* We are far away, move at full speed */
      motor_speed = consts->speed_fast_inc;
    }
  }
  else if (curr_pos > desired_pos)
  {
    /* The pot values need to decrease */
    target_diff = curr_pos - desired_pos;

    if (target_diff < consts->pos_near_dec)
    {
      /* We are close enough, don't move any more */
      motor_speed = 0;
    }
    else if (target_diff < consts->pos_close_dec)
    {
      /* We are close, move at a fixed speed */
      motor_speed = 0 - consts->speed_slow_dec;
    }
    else if (target_diff < consts->pos_far_dec)
    {
      /* We are in the mid range, scale based on how far we are */
      motor_speed = 0 - (consts->speed_slow_dec +
               (int)(consts->speed_fast_dec - consts->speed_slow_dec) *
               (int)(target_diff - consts->pos_close_dec) /
               (int)(consts->pos_far_dec - consts->pos_close_dec));
    }
    else
    {
      /* We are far away, move at full speed */
      motor_speed = 0 - consts->speed_fast_dec;
    }
  }
  else
  {
    motor_speed = 0;
  }

  return (motor_speed);
}

