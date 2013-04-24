/*******************************************************************************
* FILE NAME: ws_crab_input.h
*
* DESCRIPTION:
*  This is the include file which corresponds to ws_crab_input.c
*
*  It contains some aliases and function prototypes used in those files.
*
* USAGE:
*  If you add your own routines to those files, this is a good place to add
*  your custom macros (aliases), type definitions, and function prototypes.
*******************************************************************************/

#ifndef __ws_crab_input_h_
#define __ws_crab_input_h_

/******************************* TYPEDEFS *************************************/

/******************************** MACROS **************************************/

/***************************** DEFINITIONS ************************************/
#define CRAB_MANU_SPEED_MAX       50

#define ONE_AXIS_DEADZONE         10
#define TWO_AXIS_DEADZONE         50

#define STICK_LEFT_SIDE           191 /* the value in brads when the joystick
                                         is on the left side */
#define STICK_RIGHT_SIDE          64  /* the value in brads when the joystick
                                         is on the right side */

#define ONE_TWO_AXIS_CRAB_DIVIDER 110

#define TWO_AXIS_NOT_CENTER       0
#define TWO_AXIS_CENTER           1
#define TWO_AXIS_RIGHT            0
#define TWO_AXIS_LEFT             1


/****************************** STRUCTURES ************************************/

/************************* FUNCTION PROTOTYPES ********************************/
extern void crab_manu_override(MotorValsType *);
extern void crab_stick_input(CrabTargetsType *);

#endif /* __ws_crab_input_h_ */

