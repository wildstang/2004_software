/*******************************************************************************
* FILE NAME: ws_code.h
*
* DESCRIPTION:
*  This is the include file which corresponds to ws_code.c
*
*  It contains some aliases and function prototypes used in those files.
*
* USAGE:
*  If you add your own routines to those files, this is a good place to add
*  your custom macros (aliases), type definitions, and function prototypes.
*******************************************************************************/

#ifndef __ws_code_h_
#define __ws_code_h_

/******************************* TYPEDEFS *************************************/
#ifdef CYGWIN_IFI_SIMULATOR
/* Simulator defines */
typedef unsigned char   INT8;
typedef unsigned char   UINT8;
typedef signed char     SINT8;
typedef unsigned short  UINT16;
typedef signed short    INT16;
typedef unsigned int    UINT32;
typedef signed int      SINT32;
#else
/* Target defines */
typedef unsigned char UINT8;
typedef char          INT8;
typedef unsigned int  UINT16;
typedef int           INT16;
#endif

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

/******************************** MACROS **************************************/

#define MAX(variable, max_val)  if (variable > (max_val)) variable = (max_val);
#define MIN(variable, min_val)  if (variable < (min_val)) variable = (min_val);

#define MAX_RETURN(value, max_val) ((value) > (max_val) ? (max_val) : (value))
#define MIN_RETURN(value, min_val) ((value) < (min_val) ? (min_val) : (value))

#define GET_ANALOG_VALUE_SHIFT(a) (Get_Analog_Value(a) >> 2)

/***************************** DEFINITIONS ************************************/

#define TRUE  1
#define FALSE 0

#define SUCCESS 1
#define FAIL    0


/*** crab feedback loop ***/
/* distances from crab targets */
#define CRAB_POSITION_NEAR_LEFT   3
#define CRAB_POSITION_CLOSE_LEFT  25
#define CRAB_POSITION_FAR_LEFT    50
#define CRAB_POSITION_NEAR_RIGHT  2
#define CRAB_POSITION_CLOSE_RIGHT 15
#define CRAB_POSITION_FAR_RIGHT   40

/* crab speeds */
#define CRAB_SLOW_LEFT            30
#define CRAB_MED_LEFT             70
#define CRAB_FAST_LEFT            126  /* must be < 127 */
#define CRAB_SLOW_RIGHT           35
#define CRAB_MED_RIGHT            85
#define CRAB_FAST_RIGHT           126  /* must be < 127 */

#define CRAB_MANU_SPEED_MAX       50



#define POT_CRAB_RIGHT_LIMIT      5
#define POT_CRAB_LEFT_LIMIT       250


/*** Scaling constants ***/
#define DRIVE_SCALE_50            50 /* turbo drive speed reduction (percent) */
#define DRIVE_CON_50              64 /* 128 * (1 - reduction percent) */

/*** Joystick constants ***/
#define DRIVE_DEADZONE            5
#define ONE_AXIS_DEADZONE         10
#define TWO_AXIS_DEADZONE         50
#define CRAB_MODE_DEADZONE        20

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
typedef struct motor_vals_
{
  unsigned char lf_wheel;
  unsigned char rf_wheel;
  unsigned char lb_wheel;
  unsigned char rb_wheel;
  unsigned char left_crab;
  unsigned char right_crab;
  RelayValsType pump;
  RelayValsType foot;
} MotorValsType;

typedef struct crab_targets_
{
  unsigned char left_crab_tar;
  unsigned char right_crab_tar;
} CrabTargetsType;

/************************* FUNCTION PROTOTYPES ********************************/
extern int  test_pots(unsigned char, unsigned char);
extern void pump_control(MotorValsType *);
extern void foot_control(MotorValsType *);
extern void drive_stick_input(MotorValsType *);
extern void crab_manu_override(MotorValsType *);
extern void crab_stick_input(CrabTargetsType *);
extern void one_axis_crab_steering(CrabTargetsType *);
extern void two_axis_crab_steering(CrabTargetsType *,
                                   unsigned char *);
extern void crab_system(CrabTargetsType *, MotorValsType *);
extern unsigned char crab_scaling(unsigned char, unsigned char, unsigned char);
extern unsigned char crab_feedback_left(unsigned char, unsigned char);
extern unsigned char crab_feedback_right(unsigned char, unsigned char);
extern unsigned char arctan(unsigned char, unsigned char);


#endif /* __ws_code_h_ */

