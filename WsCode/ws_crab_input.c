/*******************************************************************************
* FILE NAME: ws_crab_input.c
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

#include "ws_crab_input.h"
#include "ws_trig.h"


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
void  crab_manu_override(MotorValsType *motor_vals)
{
  INT8 manu_crab_speed;

  manu_crab_speed = Oi_crab_x - 127;

  /* limit speed in manual mode */
  MIN(manu_crab_speed, -(CRAB_MANU_SPEED_MAX));
  MAX(manu_crab_speed, CRAB_MANU_SPEED_MAX);

  /* move crab modules if button is pressed */
  if (Oi_sw_crab_manu_move == 1)
  {
    motor_vals->crab = manu_crab_speed;
  }
  else
  {
    motor_vals->crab = 0;
  }

  return;
}



/*******************************************************************************
* FUNCTION NAME: crab_stick_input
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
void crab_stick_input(CrabTargetsType *crab_targets)
{
  /* since the joystick only goes from 13-250 we should scale this to 0-255 */
  if (Oi_crab_x > 127)
  {
    /* make sure joystick doesnt go over max */
    MAX(Oi_crab_x, CRAB_STICK_MAX_VALUE);

    /* Scale joystick max to 255 */
    Oi_crab_x = (127 + (((int)Oi_crab_x - 127) * 127 / CRAB_STICK_TRAVEL_HI));
  }
  else
  {
    /* make sure joystick doesnt go under min */
    MIN(Oi_crab_x, CRAB_STICK_MIN_VALUE);

    /* scale joystick min to 0 */
    Oi_crab_x = (127 - ((127 - (int)Oi_crab_x) * 127 / CRAB_STICK_TRAVEL_LOW));
  }

  if ((Oi_crab_x > (127 - ONE_AXIS_DEADZONE)) &&
      (Oi_crab_x < (127 + ONE_AXIS_DEADZONE)))
  {
    /* keep wheel straight */
    crab_targets->crab_tar = 127;
  }
  else
  {
    if (Oi_crab_x > (127 + ONE_AXIS_DEADZONE))
    {
      /* turn wheels left */
      /* scale so we turn 180 degrees to 90 degrees */
      crab_targets->crab_tar = 127 + ((Oi_crab_x - 127) / 2);
    }
    else if (Oi_crab_x < (127 - ONE_AXIS_DEADZONE))
    {
      /* turn wheels right */
      /* scale so we turn 180 degrees to 90 degrees */
      crab_targets->crab_tar = 127 - ((127 - Oi_crab_x) / 2);
    }
  }

  return;
}


