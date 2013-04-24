/*******************************************************************************
* FILE NAME: ws_includes.h
*
* DESCRIPTION:
*  It contains some aliases and function prototypes used in those files.
*
* USAGE:
*  If you add your own routines to those files, this is a good place to add
*  your custom macros (aliases), type definitions, and function prototypes.
*******************************************************************************/

#ifndef __ws_includes_h_
#define __ws_includes_h_

/******************************* TYPEDEFS *************************************/

typedef enum
{
  POTS_GOOD,
  POTS_BAD
} PotStateType;

typedef enum
{
  RELAY_OFF = 0,
  RELAY_FWD,
  RELAY_REV
} RelayValsType;

typedef enum
{
  PUMP_ON = 0,
  PUMP_OFF
} PumpRunType;

typedef enum
{
  SUSPENSION_UP = 0,
  SUSPENSION_DOWN
} SuspensionMoveType;

typedef enum
{
  BUMPER_IN = 0,
  BUMPER_OUT
} BumperMoveType;

typedef enum
{
  ACCUMULATOR_STOP = 0,
  ACCUMULATOR_FWD,
  ACCUMULATOR_REV
} AccumulatorMoveType;

typedef enum
{
  SQUEEZE_IN = 0,
  SQUEEZE_OUT
} ArmSqueezeType;

/******************************** MACROS **************************************/

#define MAX(variable, max_val)  if (variable > (max_val)) variable = (max_val)
#define MIN(variable, min_val)  if (variable < (min_val)) variable = (min_val)

#define MAX_RETURN(value, max_val) ((value) > (max_val) ? (max_val) : (value))
#define MIN_RETURN(value, min_val) ((value) < (min_val) ? (min_val) : (value))

#define GET_ANALOG_VALUE_SHIFT(a) (Get_Analog_Value(a) >> 2)

#define HAT_RANGE_CHECK(hat, value) \
   ((((hat) >= ((int)(value) - HAT_RANGE)) && \
     ((hat) <= ((int)(value) + HAT_RANGE))) ? \
    TRUE : FALSE)

/***************************** DEFINITIONS ************************************/

#define TRUE  1
#define FALSE 0

#define SUCCESS 1
#define FAIL    0

#define ROBOT_ENABLED  0
#define ROBOT_DISABLED 1

#define NUM_PACKETS_PER_SEC 40

/*** Joystick constants ***/
#define CRAB_MODE_DEADZONE        30

#define HAT_RANGE   10


/****************************** STRUCTURES ************************************/
typedef struct motor_vals_
{
  INT8                lf_wheel;
  INT8                rf_wheel;
  INT8                lb_wheel;
  INT8                rb_wheel;
  INT8                crab;
  INT8                big_arm_shoulder;
  INT8                big_arm_elbow;
  INT8                small_arm_shoulder;
  INT8                winch;
  UINT8               arm_brake;
  UINT8               winch_brake;
  PumpRunType         pump;
  SuspensionMoveType  suspension_front;
  SuspensionMoveType  suspension_back;
  AccumulatorMoveType accumulator;
  ArmSqueezeType      small_arm_squeeze;
  BumperMoveType      front_bumper;
  BumperMoveType      back_bumper;
} MotorValsType;

typedef struct crab_targets_
{
  unsigned char crab_tar;
} CrabTargetsType;

typedef struct calibration_vals_
{
  UINT8 crab_fwd;
  UINT8 crab_180_left;
  UINT8 crab_180_right;
  UINT8 big_arm_shoulder_hi;
  UINT8 big_arm_shoulder_lo;
  UINT8 big_arm_elbow_hi;
  UINT8 big_arm_elbow_lo;
  UINT8 small_arm_shoulder_hi;
  UINT8 small_arm_shoulder_lo;
} CalibrationValsType;

/************************* FUNCTION PROTOTYPES ********************************/

#endif /* __ws_includes_h_ */

