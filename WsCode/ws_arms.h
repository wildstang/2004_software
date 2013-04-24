/*******************************************************************************
* FILE NAME: ws_arms.h
*
* DESCRIPTION:
*  This is the include file which corresponds to ws_arms.c
*
*  It contains some aliases and function prototypes used in those files.
*
* USAGE:
*  If you add your own routines to those files, this is a good place to add
*  your custom macros (aliases), type definitions, and function prototypes.
*******************************************************************************/

#ifndef __ws_arms_h_
#define __ws_arms_h_

/******************************* TYPEDEFS *************************************/

/******************************** MACROS **************************************/

/***************************** DEFINITIONS ************************************/
/* distances from small arm targets */
#if 0
#define SMALL_ARM_POSITION_NEAR_DOWN   3
#define SMALL_ARM_POSITION_CLOSE_DOWN  10
#define SMALL_ARM_POSITION_FAR_DOWN    30
#define SMALL_ARM_POSITION_NEAR_UP  3
#define SMALL_ARM_POSITION_CLOSE_UP 10
#define SMALL_ARM_POSITION_FAR_UP   30
#else
#define SMALL_ARM_POSITION_NEAR_DOWN   4
#define SMALL_ARM_POSITION_CLOSE_DOWN  25
#define SMALL_ARM_POSITION_FAR_DOWN    30
#define SMALL_ARM_POSITION_NEAR_UP  4
#define SMALL_ARM_POSITION_CLOSE_UP 20
#define SMALL_ARM_POSITION_FAR_UP   30
#endif

/* small arm speeds */
#if 0
#define SMALL_ARM_SLOW_DOWN            30
#define SMALL_ARM_FAST_DOWN            126  /* must be < 127 */
#define SMALL_ARM_SLOW_UP              50
#define SMALL_ARM_FAST_UP              126  /* must be < 127 */
#else
#define SMALL_ARM_SLOW_DOWN            20
#define SMALL_ARM_FAST_DOWN            126  /* must be < 127 */
#define SMALL_ARM_SLOW_UP              35
#define SMALL_ARM_FAST_UP              126  /* must be < 127 */
#endif

#define SMALL_ARM_UP_LIMIT         245   /* Highest pot value to allow */
#define SMALL_ARM_DOWN_LIMIT       10    /* Lowest pot value to allow */

#define BIG_ARM_HANG_TARGET 175
#define BIG_ARM_HANG_TGT_10 ((int)(BIG_ARM_HANG_TARGET-15)<<(int)2)
#define BIG_ARM_AUTO_MOVE_SPEED  50

#define SMALL_ARM_POPPER_POS    38  /* arms are 45 on proto */
#define SMALL_ARM_GRAB_BB_POS   67  /* arms are 72 on proto */
#define SMALL_ARM_UP_POS        121
#define SMALL_ARM_UP_BB_POS     140


/****************************** STRUCTURES ************************************/

/************************* FUNCTION PROTOTYPES ********************************/
INT16 get_big_arm_speed(void);
UINT8 big_arm_speed_feedback(INT8 desired_speed);
UINT8 arm_scaling(UINT8 arm_tar_brads, UINT8 down_pot_constraint,
                  UINT8 up_pot_constraint);

#endif /* __ws_crab_feedback_h_ */

