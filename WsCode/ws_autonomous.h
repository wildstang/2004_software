/*******************************************************************************
* FILE NAME: ws_autonomous.h
*
* DESCRIPTION:
*  This is the include file which corresponds to autonomous.c
*
*  It contains some aliases and function prototypes used in those files.
*
* USAGE:
*  If you add your own routines to those files, this is a good place to add
*  your custom macros (aliases), type definitions, and function prototypes.
*******************************************************************************/

#ifndef __ws_autonomous_h_
#define __ws_autonomous_h_

/******************************* TYPEDEFS *************************************/
typedef enum
{
  AUTO_OFF = 0,
  AUTO_ON
} AutoStateType;

/******************************** MACROS **************************************/

/***************************** DEFINITIONS ************************************/
#define ROBOT_WIDTH           24
#define BUMPER_LENGTH         0

#define STARTING_POS_RIGHT_X  (200-BUMPER_LENGTH)
#define STARTING_POS_RIGHT_Y  200
#define STARTING_POS_RIGHT_T  0
#define STARTING_POS_LEFT_X   (200-BUMPER_LENGTH)
#define STARTING_POS_LEFT_Y   200
#define STARTING_POS_LEFT_T   0

#define INVALID_WAYPOINT_NUM 255

/*#define WP_OPTION_DRIVE_MASK   0x03*/
#define WP_OPTION_CRAB            0x01
#define WP_OPTION_TANK            0x02
#define WP_OPTION_TIMEPOINT       0x04 /* This point is time based */
#define WP_ARM_OPTION_SQUEEZE_OUT 0x08 /* small arm squeezer out */
#define WP_OPTION_SUSP_DOWN       0x10 /* lower suspensions */
#define WP_OPTION_HARD_TURN       0x20 /* make harder tank turn */
#define WP_OPTION_BUMPER_OUT      0x40 /* extend front bumper */
#define WP_OPTION_FULL_BUPOD      0x80 /* run BUPODS at full speed */

#define WP_TIME_SPEED_1SEC   4  /* At 26ms/packet...40 packets = 1 sec */

/* autonomous program selector knob */
#define WAYPOINT_OI_SEL0     4
#define WAYPOINT_OI_SEL1    25
#define WAYPOINT_OI_SEL2    47
#define WAYPOINT_OI_SEL3    71
#define WAYPOINT_OI_SEL4    93
#define WAYPOINT_OI_SEL5   116
#define WAYPOINT_OI_SEL6   139
#define WAYPOINT_OI_SEL7   161
#define WAYPOINT_OI_SEL8   184
#define WAYPOINT_OI_SEL9   206
#define WAYPOINT_OI_SEL10  230
#define WAYPOINT_OI_DIFF     4

#define LEFT_SIDE     0
#define RIGHT_SIDE    1

/****************************** STRUCTURES ************************************/
typedef struct position_
{
  UINT16 x;
  UINT16 y;
  UINT8  theta;
} PositionType;

typedef struct waypoint_data_
{
  INT8  speed;
  UINT8 option_flags;
  UINT8 s_arm_pos;
} WaypointDataType;

typedef struct waypoint_common_
{
  UINT8 orientation; /* compass reading of the robot in brads, 0 if fwd */
  INT8  speed;       /* speed to get to the point, 127 if full speed    */
  UINT8 range;       /* size of the waypoint, determines how close we need
                      * to be to the point before moving on to the next point */
  UINT8 option_flags; /* options for each waypoint */
  UINT8 s_arm_pos;   /* position of small arm in brads:
                      * 0 = striaght down
                      * 127 = straight up */
  UINT8 max_time;    /* max time allocated for each waypoint */
} WaypointInfo;

typedef struct waypoint_pos_
{
  UINT16 x_pos;
  UINT16 y_pos;
}WaypointPos;

typedef struct waypoint_
{
  WaypointPos*  p_waypoint_pos;
  WaypointInfo* p_waypoint_info;
} Waypoints;

/************************* FUNCTION PROTOTYPES ********************************/

extern void auto_chooser(void);
extern void auto_main(CalibrationValsType *);
extern UINT8 input_data(PositionType *);
extern void waypoint_init(void);
extern void autonomous_init(void);
extern UINT8 waypoint_manager(PositionType *, PositionType *,
                              WaypointDataType *);
extern void read_waypoint(void);
extern void target_angle_speed_controller(PositionType *, PositionType *,
                                          CrabTargetsType *, MotorValsType *,
                                          WaypointDataType *,
                                          CalibrationValsType *);
extern void crab_position_controller(UINT8, PositionType *, CrabTargetsType *);
extern void SetHeading(INT8, INT8, MotorValsType *, UINT8);
extern void display_auto_data(void);

#endif /* __ws_autonomous_h_ */

