/*******************************************************************************
* FILE NAME: autonomous.h
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

#ifndef __autonomous_h_
#define __autonomous_h_

/******************************* TYPEDEFS *************************************/
typedef enum
{
  AUTO_ON,
  AUTO_OFF
} AutoStateType;

/******************************** MACROS **************************************/

/***************************** DEFINITIONS ************************************/
#define INVALID_WAYPOINT_NUM 255

/****************************** STRUCTURES ************************************/
typedef struct position_
{
  UINT8 x;
  UINT8 y;
  UINT8 theta;
} PositionType;

typedef struct waypoint_common_
{
  UINT8 orientation; /* compass reading of the robot in brads, 0 if fwd */
  UINT8 speed;       /* speed to get to the point, 127 if full speed    */
  UINT8 range;       /* size of the waypoint, determines how close we need
                      * to be to the point before moving on to the next point */
} WaypointInfo;

typedef struct waypoint_pos_
{
  UINT8 x_pos;
  UINT8 y_pos;
}WaypointPos;

typedef struct waypoint_
{
  const WaypointPos*  p_waypoint_pos;
  const WaypointInfo* p_waypoint_info;
} Waypoints;

/************************* FUNCTION PROTOTYPES ********************************/

extern void auto_main(void);
extern UINT8 input_data(PositionType *);
extern void waypoint_init(void);
extern void autonomous_init(void);
extern UINT8 waypoint_manager(PositionType *, PositionType *);
extern void read_waypoint(void);
extern void target_angle_speed_controller(PositionType *, PositionType *,
                                          CrabTargetsType *);
extern void crab_position_controller(UINT8, PositionType *, CrabTargetsType *);

#endif /* __autonomous_h_ */

