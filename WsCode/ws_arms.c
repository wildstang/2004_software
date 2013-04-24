/*******************************************************************************
* FILE NAME: ws_arms.c
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

#include "ws_arms.h"
#include "ws_rtc.h"
#include "ws_feedback.h"

#define BIG_ARM_TIME_DELTA_SHIFT  2  /* Num of bits to shift off the big arm
                                      * time delta per loop 
                                      */

#if 0
/*******************************************************************************
* FUNCTION NAME: big_arm_speed_feedback 
* PURPOSE:       Drive big arm based on speed desired and current speed
*                It is worth noting that the speed of the arm is not just the
*                speed sent to the speed controller.  The speed controller is
*                really a power controller, but the speed of the arm will
*                change based on gravity and how much weight it is moving.
* CALLED FROM:
* ARGUMENTS:     desired_speed - desired speed to run the big arm
* RETURNS:       PWM value to cause the desired speed
*******************************************************************************/
UINT8 big_arm_speed_feedback(INT8 desired_speed)
{
  INT16 big_arm_speed;
  INT16 diff_from_target;

  /* Get the current speed (signed) */
  big_arm_speed = get_big_arm_speed();

  /* desired speed and curr speed are signed so adding them together gives
   * us the speed we need to run at to get us going the correct speed.  It
   * will probably need to be scaled.
   */
  diff_from_target = desired_speed - big_arm_speed;
  MAX(diff_from_target, 127);  /* Ensure we don't roll the byte below */
  MIN(diff_from_target, -127); /* Ensure we don't roll the byte below */

  return((UINT8)diff_from_target + 127);
}

/*******************************************************************************
* FUNCTION NAME: get_big_arm_speed
* PURPOSE:       It is worth noting that the speed of the arm is not just the
*                speed sent to the speed controller.  The speed controller is
*                really a power controller, but the speed of the arm will
*                change based on gravity and how much weight it is moving.
*                Use the change in pot value over time to determine the speed.
* CALLED FROM:
* ARGUMENTS:
* RETURNS:       Current speed of the big arm (signed based on direction)
*                positive speed means the arm is moving UP?
*                negative speed means the arm is moving DOWN?
*
* NOTE: The first loop ever will return a bogus value since prev is initialied
*       to zero.
*******************************************************************************/
INT16 get_big_arm_speed(void)
{
   UINT16        time_delta;
   INT16         arm_pos_delta;
   INT16         arm_speed;
   UINT16        curr_arm_pos;
   static UINT16 s_prev_arm_pos=0;

   /* Get the current position of the big arm (10 bit resloution) */
   curr_arm_pos = Get_Analog_Value(Analog_in_big_arm_shoulder_pot);

   /* Get the time since last loop */
   time_delta = getRtcDelta(RTC_DELTA_BIG_ARM);

   /* Reduce the precision of time_delta so we can divide by a reasonably
    * sized time.
    */
   time_delta = time_delta >> BIG_ARM_TIME_DELTA_SHIFT;

   /* Make sure time_delta is never zero...div by 0 is bad! */
   if(time_delta == 0) 
   {
      time_delta = 1;
   }

   /* Determine how far the arm has moved since last loop (signed) */
   arm_pos_delta = curr_arm_pos - s_prev_arm_pos;

   /* Save off the curr position so we know it next loop */
   s_prev_arm_pos = curr_arm_pos;

   /* Speed is simply change in pos / time */
   return(arm_pos_delta/time_delta);
}

/*******************************************************************************
* FUNCTION NAME: small_arm_pos_feedback 
* PURPOSE:       Drive small arm to the specified position.
* CALLED FROM:
* ARGUMENTS:     desired_pos - desired position for the small arm
* RETURNS:       PWM value to cause the desired speed
*******************************************************************************/
UINT8 small_arm_speed_feedback(UINT8 desired_pos)
{

}

#endif

/*******************************************************************************
* FUNCTION NAME: arm_scaling
* PURPOSE:       Convert from binary degrees to pot values
*                0   brads indicates arms down
*                63  brads indicates arms parallel to the ground
*                127 brads indicates arms straight up
*
*                127
*                 |
*                 -- 63
*                 |
*                 0
*
*                 Note: We can move the arm past 127 and it is the callers
*                 responsibility to limit the pot travel points to ensure
*                 we don't break the arm.
*
* CALLED FROM:
* ARGUMENTS:
* RETURNS:       target (in pot values) of the arm 
* DOL:
*******************************************************************************/
UINT8 arm_scaling(UINT8 arm_tar_brads,
                  UINT8 down_pot_constraint,  /* Pot value at 0 brads */
                  UINT8 up_pot_constraint)    /* Pot value at 127 brads */
{
  UINT8 arm_tar_pot_vals;

  /* scale the 127 brads to the pot values */
  arm_tar_pot_vals = (((int)arm_tar_brads) *
                      ((int)up_pot_constraint - down_pot_constraint) /
                      127) + down_pot_constraint;

  return (arm_tar_pot_vals);
}


