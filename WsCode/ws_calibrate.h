/*******************************************************************************
* FILE NAME: ws_calibrate.h
*
* DESCRIPTION:
*  This is the include file which corresponds to ws_calibrate.c
*
*  It contains some aliases and function prototypes used in those files.
*
* USAGE:
*  If you add your own routines to those files, this is a good place to add
*  your custom macros (aliases), type definitions, and function prototypes.
*******************************************************************************/

#ifndef __ws_calibrate_h_
#define __ws_calibrate_h_

/******************************* TYPEDEFS *************************************/
typedef enum
{
  DISPLAY_NONE,
  DISPLAY_DRIVE_X,
  DISPLAY_DRIVE_Y,
  DISPLAY_CRAB_X,
  DISPLAY_CRAB_Y,
  DISPLAY_BIG_ARM_SHOULDER,
  DISPLAY_BIG_ARM_ELBOW,
  DISPLAY_SMALL_ARM_SHOULDER,
  DISPLAY_CRAB_POT,
  DISPLAY_BIG_ARM_SHOULDER_POT,
  DISPLAY_BIG_ARM_ELBOW_POT,
  DISPLAY_SMALL_ARM_SHOULDER_POT
} DISPLAY_TYPE;

/******************************** MACROS **************************************/

/***************************** DEFINITIONS ************************************/
#define CALIBRATE_ZONE      50

#define CALIBRATE_KNOWN_BYTE_1  0xFA
#define CALIBRATE_KNOWN_BYTE_2  0xDE


#define CAL_MASK_CRAB_FWD       0x01
#define CAL_MASK_CRAB_LEFT      0x02
#define CAL_MASK_CRAB_RIGHT     0x04

#define CAL_MASK_B_ARM_SHOULDER_HI  0x01
#define CAL_MASK_B_ARM_SHOULDER_LO  0x02
#define CAL_MASK_B_ARM_ELBOW_HI     0x04
#define CAL_MASK_B_ARM_ELBOW_LO     0x08
#define CAL_MASK_S_ARM_SHOULDER_HI  0x10
#define CAL_MASK_S_ARM_SHOULDER_LO  0x20

#define DEFAULT_CRAB_FWD        127
#define DEFAULT_CRAB_LEFT       235
#define DEFAULT_CRAB_RIGHT      20

#define DEFAULT_BIG_ARM_SHOULDER_HI     200
#define DEFAULT_BIG_ARM_SHOULDER_LO     50
#define DEFAULT_BIG_ARM_ELBOW_HI        200
#define DEFAULT_BIG_ARM_ELBOW_LO        50
#define DEFAULT_SMALL_ARM_SHOULDER_HI   200
#define DEFAULT_SMALL_ARM_SHOULDER_LO   10


/****************************** STRUCTURES ************************************/

/************************* FUNCTION PROTOTYPES ********************************/
extern void calibrate_pots(void);
extern void retrieve_calibration(CalibrationValsType *);
extern void display_calibration(void);

#endif /* __ws_calibrate_h_ */

