/*******************************************************************************
* FILE NAME: ws_autonomous.c
*
* DESCRIPTION:
*
* USAGE:
*
*******************************************************************************/

#include "ifi_aliases.h"
#include "ifi_default.h"
#include "ifi_utilities.h"
#include "ws_includes.h"
#include "user_routines.h"
#include "printf_lib.h"
#include "delays.h"       /*defined locally*/
#include "ws_io.h"
#include "ws_serial2.h"
#include "ws_sync_serial.h"

#include "ws_general.h"
#include "ws_trig.h"
#include "ws_autonomous.h"
#include "ws_crab_feedback.h"
#include "ws_arms.h"
#include "ws_feedback.h"
#include "ws_cc.h"
#include "ws_waypoints_34.h"



/*#define RAISE_BOT_AT_START*/
#define BUMPERS_AT_START



/********************* PROGRAM 3 ***************************************/

/* "The Slammer" */
/* Grab goal and push it under opponent's ball drop */

WaypointPos g_waypoints_prog3_right[]={
 /*{ x,y  }*/
#ifdef RAISE_BOT_AT_START
   {  1,  1},  /* For timepoints, this is the L,R motor vals */
#endif
#ifdef BUMPERS_AT_START
   {((UINT16)STARTING_POS_RIGHT_X + 200), ((UINT16)STARTING_POS_RIGHT_Y + 3)},
#endif
   {((UINT16)STARTING_POS_RIGHT_X + 200), ((UINT16)STARTING_POS_RIGHT_Y + 3)},
   {((UINT16)STARTING_POS_RIGHT_X + 260), ((UINT16)STARTING_POS_RIGHT_Y + 3)},
   {((UINT16)STARTING_POS_RIGHT_X + 400), ((UINT16)STARTING_POS_RIGHT_Y)},
   {((UINT16)STARTING_POS_RIGHT_X + 400), ((UINT16)STARTING_POS_RIGHT_Y)},
   {((UINT16)STARTING_POS_RIGHT_X + 530), ((UINT16)STARTING_POS_RIGHT_Y + 55)},
   {  0,  0}}; /* speed can never equal 0 because that means end of prog */

WaypointPos g_waypoints_prog3_left[]={
 /*{ x,y  }*/
#ifdef RAISE_BOT_AT_START
   {  1,  1},  /* For timepoints, this is the L,R motor vals */
#endif
#ifdef BUMPERS_AT_START
   {((UINT16)STARTING_POS_LEFT_X + 200), ((UINT16)STARTING_POS_LEFT_Y - 3)},
#endif
   {((UINT16)STARTING_POS_LEFT_X + 200), ((UINT16)STARTING_POS_LEFT_Y - 3)},
   {((UINT16)STARTING_POS_LEFT_X + 260), ((UINT16)STARTING_POS_LEFT_Y - 3)},
   {((UINT16)STARTING_POS_LEFT_X + 400), ((UINT16)STARTING_POS_LEFT_Y)},
   {((UINT16)STARTING_POS_LEFT_X + 400), ((UINT16)STARTING_POS_LEFT_Y)},
   {((UINT16)STARTING_POS_LEFT_X + 530), ((UINT16)STARTING_POS_LEFT_Y - 55)},
   {  0,  0}};

WaypointInfo g_waypoints_prog3_info[]={
 /*{orientation, speed, range, options, small arm pos, max time }*/
#ifdef RAISE_BOT_AT_START
   /* Speed=num of pkts */
   {  0, WP_TIME_SPEED_1SEC,  0, WP_OPTION_TIMEPOINT | WP_OPTION_SUSP_DOWN, 0, 6},
#endif
#ifdef BUMPERS_AT_START
   {  0,127,150, WP_OPTION_CRAB | WP_OPTION_BUMPER_OUT | WP_OPTION_FULL_BUPOD, SMALL_ARM_UP_POS, 6},
#endif
   {  0,127,  4, WP_OPTION_CRAB, SMALL_ARM_UP_POS, 6},
   {  0, 40,  4, WP_OPTION_CRAB, SMALL_ARM_UP_POS, 6},
   {  0,127, 80, WP_OPTION_TANK | WP_ARM_OPTION_SQUEEZE_OUT, SMALL_ARM_GRAB_BB_POS, 6},
   {  0,127, 50, WP_OPTION_TANK, SMALL_ARM_GRAB_BB_POS, 6},
   {  0,127, 20, WP_OPTION_TANK | WP_OPTION_HARD_TURN,SMALL_ARM_UP_BB_POS, 6},
   {  0,  0,  0, WP_OPTION_CRAB, 0, 0}};

/********************* PROGRAM 4 ***************************************/

/* "The Stuffer" */
/* Grab goal and stuff it into opponent's ball chute */

WaypointPos g_waypoints_prog4_right[]={
 /*{ x,y  }*/
#ifdef RAISE_BOT_AT_START
   {  1,  1},  /* For timepoints, this is the L,R motor vals */
#endif
#ifdef BUMPERS_AT_START
   {((UINT16)STARTING_POS_RIGHT_X + 200), ((UINT16)STARTING_POS_RIGHT_Y + 3)},
#endif
   {((UINT16)STARTING_POS_RIGHT_X + 200), ((UINT16)STARTING_POS_RIGHT_Y + 3)},
   {((UINT16)STARTING_POS_RIGHT_X + 260), ((UINT16)STARTING_POS_RIGHT_Y + 3)},
   {((UINT16)STARTING_POS_RIGHT_X + 530), ((UINT16)STARTING_POS_RIGHT_Y - 35)},
   {((UINT16)STARTING_POS_RIGHT_X + 530), ((UINT16)STARTING_POS_RIGHT_Y - 35)},
   {  0,  0}}; /* speed can never equal 0 because that means end of prog */

WaypointPos g_waypoints_prog4_left[]={
 /*{ x,y  }*/
#ifdef RAISE_BOT_AT_START
   {  1,  1},  /* For timepoints, this is the L,R motor vals */
#endif
#ifdef BUMPERS_AT_START
   {((UINT16)STARTING_POS_LEFT_X + 200), ((UINT16)STARTING_POS_LEFT_Y - 3)},
#endif
   {((UINT16)STARTING_POS_LEFT_X + 200), ((UINT16)STARTING_POS_LEFT_Y - 3)},
   {((UINT16)STARTING_POS_LEFT_X + 260), ((UINT16)STARTING_POS_LEFT_Y - 3)},
   {((UINT16)STARTING_POS_LEFT_X + 530), ((UINT16)STARTING_POS_LEFT_Y + 35)},
   {((UINT16)STARTING_POS_LEFT_X + 530), ((UINT16)STARTING_POS_LEFT_Y + 35)},
   {  0,  0}};

WaypointInfo g_waypoints_prog4_info[]={
 /*{orientation, speed, range, options, small arm pos, max time }*/
#ifdef RAISE_BOT_AT_START
   /* Speed=num of pkts */
   {  0, WP_TIME_SPEED_1SEC,  0, WP_OPTION_TIMEPOINT | WP_OPTION_SUSP_DOWN, 0, 6},
#endif
#ifdef BUMPERS_AT_START
   {  0,127,150, WP_OPTION_CRAB | WP_OPTION_BUMPER_OUT | WP_OPTION_FULL_BUPOD, SMALL_ARM_UP_POS, 6},
#endif
   {  0,127,  4, WP_OPTION_CRAB, SMALL_ARM_UP_POS, 6},
   {  0, 40,  4, WP_OPTION_CRAB, SMALL_ARM_UP_POS, 6},
   {  0,127,210, WP_OPTION_TANK | WP_ARM_OPTION_SQUEEZE_OUT | WP_OPTION_HARD_TURN, SMALL_ARM_GRAB_BB_POS, 6},
   {  0,127, 20, WP_OPTION_TANK | WP_OPTION_HARD_TURN, SMALL_ARM_GRAB_BB_POS, 6},
   {  0,  0,  0, WP_OPTION_CRAB, 0, 0}};

