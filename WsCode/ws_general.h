/*******************************************************************************
* FILE NAME: ws_general.h
*
* DESCRIPTION:
*  This is the include file which corresponds to ws_general.c
*
*  It contains some aliases and function prototypes used in those files.
*
* USAGE:
*  If you add your own routines to those files, this is a good place to add
*  your custom macros (aliases), type definitions, and function prototypes.
*******************************************************************************/

#ifndef __ws_general_h_
#define __ws_general_h_

/******************************* TYPEDEFS *************************************/

/*
 * UP - wheels are in robot, not extended down
 * DN - wheels are extended down
 */
typedef enum
{
  FRONT_UP_BACK_UP_FLOOR = 0,
  FRONT_UP_BACK_UP_PLATFORM,
  FRONT_DN_BACK_DN,
  FRONT_UP_BACK_DN,
  FRONT_DN_BACK_UP
} SuspensionStateType;

typedef enum
{
  FLOOR = 0,
  PLATFORM_1,
  PLATFORM_2
} LevelType;

typedef enum
{
  AUTO = 0,
  MANUAL
} AutoManuType;

typedef enum
{
  DISPLAY_DATA_PSI,
  DISPLAY_DATA_AUTO,
  DISPLAY_DATA_CALIBRATE
} DisplayDataType;

/******************************** MACROS **************************************/

/***************************** DEFINITIONS ************************************/
#define PUMP_HI_VAL   223     /* point where we turn pump off */
#define PUMP_LO_VAL   196     /* point where we turn pump on */

#define SEE_PLATFORM      1
#define NOT_SEE_PLATFORM  0

#define BIG_ARM_SHOULDER_STICK_DEADZONE_MOVE  30
#define BIG_ARM_SHOULDER_STICK_DEADZONE_BRAKE 20
#define BIG_ARM_ELBOW_STICK_DEADZONE      25
#define SMALL_ARM_SHOULDER_STICK_DEADZONE 10

#define BIG_ARM_SHOULDER_SCALE_NUMERATOR    1
#define BIG_ARM_SHOULDER_SCALE_DENOMINATOR  1
#define BIG_ARM_SHOULDER_SLOW_ZONE          20
#define BIG_ARM_SHOULDER_MIN_SPEED          20

#define WINCH_UP_HAT                 12
#define WINCH_DOWN_HAT              254
#define BIG_ARM_AUTO_MOVE_HAT        12
#define SMALL_ARM_AUTO_MOVE_HAT     254
#define PLATFORM_SEL_UP_HAT          71
#define PLATFORM_SEL_DOWN_HAT       193

#define WINCH_BRAKE_ENGAGE      127
#define WINCH_BRAKE_RELEASE       0
#define BIG_ARM_BRAKE_ENGAGE    255
#define BIG_ARM_BRAKE_RELEASE   127

#define NUM_PACKETS_TO_EXTEND_BUMPERS  20
#define NUM_PACKETS_TO_RAISE_ROBOT     40

/****************************** STRUCTURES ************************************/

/************************* FUNCTION PROTOTYPES ********************************/
extern void display_oi_data(UINT8, DisplayDataType);
extern int  test_pots(unsigned char);
extern void pump_control(MotorValsType *);
extern void big_arm_control(MotorValsType *, CalibrationValsType *);
extern void small_arm_control(MotorValsType *, CalibrationValsType *);
extern void winch_control(MotorValsType *);
extern void bumper_control(MotorValsType *);
extern void suspension_control(MotorValsType *);

#endif /* __ws_general_h_ */

