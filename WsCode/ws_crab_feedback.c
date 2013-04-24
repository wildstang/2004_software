/*******************************************************************************
* FILE NAME: ws_crab_feedback.c
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

#include "ws_crab_feedback.h"
#include "ws_feedback.h"

/*** crab feedback loop ***/
/* distances from crab targets */
#if 0 /* 2004 Old Proto with diff pot gearing */
#define CRAB_POSITION_NEAR_LEFT   3
#define CRAB_POSITION_CLOSE_LEFT  10
#define CRAB_POSITION_FAR_LEFT    30
#define CRAB_POSITION_NEAR_RIGHT  3
#define CRAB_POSITION_CLOSE_RIGHT 10
#define CRAB_POSITION_FAR_RIGHT   30
#else
#define CRAB_POSITION_NEAR_LEFT   3
#define CRAB_POSITION_CLOSE_LEFT  7 
#define CRAB_POSITION_FAR_LEFT    20
#define CRAB_POSITION_NEAR_RIGHT  3
#define CRAB_POSITION_CLOSE_RIGHT 7
#define CRAB_POSITION_FAR_RIGHT   20
#endif

/* crab speeds */
#define CRAB_SLOW_LEFT            35
#define CRAB_FAST_LEFT            126  /* must be < 127 */
#define CRAB_SLOW_RIGHT           35
#define CRAB_FAST_RIGHT           126  /* must be < 127 */


#define CRAB_LIMIT_BRAD_RIGHT     63
#define CRAB_LIMIT_BRAD_LEFT      191
#define CRAB_TRAVEL_BRAD_RIGHT    (127 - CRAB_LIMIT_BRAD_RIGHT)
#define CRAB_TRAVEL_BRAD_LEFT     (CRAB_LIMIT_BRAD_LEFT - 127)

#define POT_CRAB_RIGHT_LIMIT      10
#define POT_CRAB_LEFT_LIMIT       245

static const rom FeedbackConstsType g_big_arm_feedback_constants={
  CRAB_POSITION_NEAR_RIGHT,
  CRAB_POSITION_CLOSE_RIGHT,
  CRAB_POSITION_FAR_RIGHT,
  CRAB_POSITION_NEAR_LEFT,
  CRAB_POSITION_CLOSE_LEFT,
  CRAB_POSITION_FAR_LEFT,
  CRAB_SLOW_RIGHT,
  CRAB_FAST_RIGHT,
  CRAB_SLOW_LEFT,
  CRAB_FAST_LEFT};

/*******************************************************************************
* FUNCTION NAME: crab_system
* PURPOSE:       Move crab modules based on crab targets
* CALLED FROM:   Process_Data_From_Local_IO()
* ARGUMENTS:     crab_targets - right & left crab targets
*                motor_vals - speed of crab motors
* RETURNS:       none
*******************************************************************************/
void crab_system(CrabTargetsType *crab_targets, MotorValsType *motor_vals,
                 CalibrationValsType *calibration_vals)
{
  UINT8 crab_tar_pot_ticks;

  /* put a limit on how far the crab can turn */
  MAX(crab_targets->crab_tar, CRAB_LIMIT_BRAD_LEFT);
  MIN(crab_targets->crab_tar, CRAB_LIMIT_BRAD_RIGHT);

  /* scale from binary degrees to pot ticks */
  /* at this point the crab target is in brads */

  /*
  printf(" crabTar BRAD %d", (int)crab_targets->crab_tar);
  printf("\n");
  */

  crab_tar_pot_ticks = crab_scaling(crab_targets->crab_tar,
                                    calibration_vals->crab_fwd,
                                    calibration_vals->crab_180_left,
                                    calibration_vals->crab_180_right);
  /* at this point the crab target is in pot ticks */

  /*
  printf(" crabTar POT %d", (int)crab_tar_pot_ticks);*/
  /*printf(" TAR_POT=%d CURR_POT=%d(%d) ", (int)crab_tar_pot_ticks), 
         (int)GET_ANALOG_VALUE_SHIFT(Analog_in_crab_pot),(int)Get_Analog_Value(Analog_in_crab_pot);*/
  /*printf("\n");
  */

  /* make sure pot target is not outside of safe range of pot */
  MAX(crab_tar_pot_ticks, POT_CRAB_LEFT_LIMIT);
  MIN(crab_tar_pot_ticks, POT_CRAB_RIGHT_LIMIT);

  /* call feedback loop for crab */
  motor_vals->crab =
    general_pos_feedback(GET_ANALOG_VALUE_SHIFT(Analog_in_crab_pot),
                         crab_tar_pot_ticks, &g_big_arm_feedback_constants);

  /*
  printf(" crabSpeed %d", (int)motor_vals->crab);
  printf("\n");
  */

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
UINT8 crab_scaling(UINT8 crab_tar_brads, UINT8 middle_value,
                   UINT8 left_constraint, UINT8 right_constraint)
{
  UINT8 crab_tar_pot_vals;

  /* scale so full 0-255 binary degree range maps to pot range */
  if (crab_tar_brads >= 127)
  {
    crab_tar_pot_vals = middle_value +
                        (((int)crab_tar_brads - 127) *
                         ((int)left_constraint - middle_value) /
                         CRAB_TRAVEL_BRAD_LEFT);
  }
  else
  {
    crab_tar_pot_vals = middle_value -
                        ((127 - (int)crab_tar_brads) *
                         (middle_value - (int)right_constraint) /
                         CRAB_TRAVEL_BRAD_RIGHT);
  }

  return (crab_tar_pot_vals);
}


/*******************************************************************************
* FUNCTION NAME: crab_feedback
* PURPOSE:       feedback loop for crab module
* CALLED FROM:   crab_system()
* ARGUMENTS:     crab_pos - position of crab module
*                crab_tgt - target of crab module
* RETURNS:       speed of crab motor
* DOL:
*******************************************************************************/
#if 0
UINT8 crab_feedback(UINT8 crab_pos, UINT8 crab_tgt)
{
  UINT8 crab_speed;
  UINT8 target_diff;

  if (crab_tgt > crab_pos)
  {
    target_diff = crab_tgt - crab_pos;

    if (target_diff < CRAB_POSITION_NEAR_LEFT)
    {
      crab_speed = 127;
    }
    else if (target_diff < CRAB_POSITION_CLOSE_LEFT)
    {
      crab_speed = 127 + CRAB_SLOW_LEFT;
    }
    else if (target_diff < CRAB_POSITION_FAR_LEFT)
    {
      crab_speed = 127 + (CRAB_SLOW_LEFT +
                    (int)(CRAB_FAST_LEFT - CRAB_SLOW_LEFT) *
                    (int)(target_diff - CRAB_POSITION_CLOSE_LEFT) /
                    (int)(CRAB_POSITION_FAR_LEFT - CRAB_POSITION_CLOSE_LEFT));
    }
    else
    {
      crab_speed = 127 + CRAB_FAST_LEFT;
    }
  }
  else if (crab_pos > crab_tgt)
  {
    target_diff = crab_pos - crab_tgt;

    if (target_diff < CRAB_POSITION_NEAR_LEFT)
    {
      crab_speed = 127;
    }
    else if (target_diff < CRAB_POSITION_CLOSE_LEFT)
    {
      crab_speed = 127 - CRAB_SLOW_LEFT;
    }
    else if (target_diff < CRAB_POSITION_FAR_LEFT)
    {
      crab_speed = 127 - (CRAB_SLOW_LEFT +
                    (int)(CRAB_FAST_LEFT - CRAB_SLOW_LEFT) *
                    (int)(target_diff - CRAB_POSITION_CLOSE_LEFT) /
                    (int)(CRAB_POSITION_FAR_LEFT - CRAB_POSITION_CLOSE_LEFT));
    }
    else
    {
      crab_speed = 127 - CRAB_FAST_LEFT;
    }
  }
  else
  {
    crab_speed = 127;
  }

  return (crab_speed);
}
#endif



