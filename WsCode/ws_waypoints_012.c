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
#include "ws_waypoints_012.h"



/*#define RAISE_BOT_AT_START*/
#define BUMPERS_AT_START



/********************* PROGRAM 0 ***************************************/

/* "The Sleeper" */
/***** Do nothing *****/

WaypointPos g_waypoints_prog0_right[]={
 /*{ x,y  }*/
   {  0,  0}};

WaypointPos g_waypoints_prog0_left[]={
 /*{ x,y  }*/
   {  0,  0}};

WaypointInfo g_waypoints_prog0_info[]={
 /*{orientation, speed, range, options, small arm pos, max time }*/
   {  0,  0,  0, WP_OPTION_CRAB, 0, 0}};



/********************* PROGRAM 1 ***************************************/

/* "The Popper" */
/****** Drive straight out and turn to hit 10 pt ball and back off *****/

WaypointPos g_waypoints_prog1_right[]={
 /*{ x,y  }*/
#if 0
   /* drive straight out and a little to the right */
   {((UINT16)STARTING_POS_RIGHT_X + 225), ((UINT16)STARTING_POS_RIGHT_Y - 20)},
   /* Turn and hit the 10pt ball */
   {((UINT16)STARTING_POS_RIGHT_X + 254), ((UINT16)STARTING_POS_RIGHT_Y - 60)},
   {((UINT16)STARTING_POS_RIGHT_X + 254), ((UINT16)STARTING_POS_RIGHT_Y - 40)},
#endif
#if 0
   /* drive straight out and a little to the right, block opp 10pt */
   {((UINT16)STARTING_POS_RIGHT_X + 225), ((UINT16)STARTING_POS_RIGHT_Y - 20)},
   /* Turn and block the opponents 10pt ball */
   {((UINT16)STARTING_POS_RIGHT_X + 254), ((UINT16)STARTING_POS_RIGHT_Y - 60)},
   /*
   {((UINT16)STARTING_POS_RIGHT_X + 254), ((UINT16)STARTING_POS_RIGHT_Y - 40)},
   */
#endif
#if 1
   {((UINT16)STARTING_POS_RIGHT_X + 240), ((UINT16)STARTING_POS_RIGHT_Y - 51)},
   {((UINT16)STARTING_POS_RIGHT_X + 240), ((UINT16)STARTING_POS_RIGHT_Y - 51)},
   {((UINT16)STARTING_POS_RIGHT_X + 223), ((UINT16)STARTING_POS_RIGHT_Y - 13)},
#endif
   {  0,  0}};

WaypointPos g_waypoints_prog1_left[]={
 /*{ x,y  }*/
   /* Drive straight out and a little to the left */
#if 0
   {((UINT16)STARTING_POS_LEFT_X + 225), ((UINT16)STARTING_POS_LEFT_Y + 20)},
   /* Turn and hit the 10pt ball */
   {((UINT16)STARTING_POS_LEFT_X + 254), ((UINT16)STARTING_POS_LEFT_Y + 60)},
   {((UINT16)STARTING_POS_LEFT_X + 254), ((UINT16)STARTING_POS_LEFT_Y + 40)},
#endif
#if 0
   /* drive straight out and a little to the right, block opp 10pt */
   {((UINT16)STARTING_POS_RIGHT_X + 225), ((UINT16)STARTING_POS_RIGHT_Y + 20)},
   /* Turn and block the opponents 10pt ball */
   {((UINT16)STARTING_POS_RIGHT_X + 254), ((UINT16)STARTING_POS_RIGHT_Y + 60)},
   /*
   {((UINT16)STARTING_POS_RIGHT_X + 254), ((UINT16)STARTING_POS_RIGHT_Y + 40)},
   */
#endif
#if 1
   {((UINT16)STARTING_POS_LEFT_X + 240), ((UINT16)STARTING_POS_LEFT_Y + 51)},
   {((UINT16)STARTING_POS_LEFT_X + 240), ((UINT16)STARTING_POS_LEFT_Y + 51)},
   {((UINT16)STARTING_POS_LEFT_X + 223), ((UINT16)STARTING_POS_LEFT_Y + 13)},
#endif
   {  0,  0}};

WaypointInfo g_waypoints_prog1_info[]={
 /*{orientation, speed, range, options, small arm pos, max time }*/
#if 0
   {  0,127,  4, WP_OPTION_CRAB, 0, 6},
   {  0,127, 12, WP_OPTION_TANK, 0, 2}, /* small arm was 27 */
   /*
   {  0,-127, 5, WP_OPTION_TANK | WP_OPTION_HARD_TURN, 0, 2},
   */
#endif
#if 1
   {  0,127,100, WP_OPTION_CRAB | WP_OPTION_FULL_BUPOD, 0, 3},
   {  0,127,  4, WP_OPTION_CRAB,SMALL_ARM_POPPER_POS, 2},
   {  0,-60,  4, WP_OPTION_TANK | WP_OPTION_HARD_TURN | WP_ARM_OPTION_SQUEEZE_OUT, SMALL_ARM_POPPER_POS, 2},
#endif
   {  0,  0,  0, WP_OPTION_CRAB, 0, 0}};



/********************* PROGRAM 2 ***************************************/

/* "The Retriever" */
/***** Grab goal and bring it back to our ball drop ******/

WaypointPos g_waypoints_prog2_right[]={
 /*{ x,y  }*/
#ifdef RAISE_BOT_AT_START
   {  1,  1},  /* For timepoints, this is the L,R motor vals */
#endif
#ifdef BUMPERS_AT_START
   {((UINT16)STARTING_POS_RIGHT_X + 170), ((UINT16)STARTING_POS_RIGHT_Y + 3)},
#endif
   {((UINT16)STARTING_POS_RIGHT_X + 170), ((UINT16)STARTING_POS_RIGHT_Y + 3)},
   {((UINT16)STARTING_POS_RIGHT_X + 260), ((UINT16)STARTING_POS_RIGHT_Y + 3)},

   {((UINT16)STARTING_POS_RIGHT_X + 40), ((UINT16)STARTING_POS_RIGHT_Y + 6)},
   {((UINT16)STARTING_POS_RIGHT_X + 40), ((UINT16)STARTING_POS_RIGHT_Y + 6)},
   {((UINT16)STARTING_POS_RIGHT_X + 40), ((UINT16)STARTING_POS_RIGHT_Y + 6)},
   {((UINT16)STARTING_POS_RIGHT_X + 70), ((UINT16)STARTING_POS_RIGHT_Y + 70)},
   {((UINT16)STARTING_POS_RIGHT_X + 40), ((UINT16)STARTING_POS_RIGHT_Y + 70)},
   {  0,  0}}; /* speed can never equal 0 because that means end of prog */

WaypointPos g_waypoints_prog2_left[]={
 /*{ x,y  }*/
#ifdef RAISE_BOT_AT_START
   {  1,  1},  /* For timepoints, this is the L,R motor vals */
#endif
#ifdef BUMPERS_AT_START
   {((UINT16)STARTING_POS_LEFT_X + 170), ((UINT16)STARTING_POS_LEFT_Y - 3)},
#endif
   {((UINT16)STARTING_POS_LEFT_X + 170), ((UINT16)STARTING_POS_LEFT_Y - 3)},
   {((UINT16)STARTING_POS_LEFT_X + 260), ((UINT16)STARTING_POS_LEFT_Y - 3)},

   {((UINT16)STARTING_POS_LEFT_X + 40), ((UINT16)STARTING_POS_LEFT_Y - 6)},
   {((UINT16)STARTING_POS_LEFT_X + 40), ((UINT16)STARTING_POS_LEFT_Y - 6)},
   {((UINT16)STARTING_POS_LEFT_X + 40), ((UINT16)STARTING_POS_LEFT_Y - 6)},
   {((UINT16)STARTING_POS_LEFT_X + 70), ((UINT16)STARTING_POS_LEFT_Y - 70)},
   {((UINT16)STARTING_POS_LEFT_X + 40), ((UINT16)STARTING_POS_LEFT_Y - 70)},
   {  0,  0}}; /* speed can never equal 0 because that means end of prog */

WaypointInfo g_waypoints_prog2_info[]={
 /*{orientation, speed, range, options, small arm pos, max time }*/
#ifdef RAISE_BOT_AT_START
   {  0, WP_TIME_SPEED_1SEC,  0, WP_OPTION_TIMEPOINT | WP_OPTION_SUSP_DOWN, 0, 6},
#endif
#ifdef BUMPERS_AT_START
   {  0,127, 80, WP_OPTION_CRAB | WP_OPTION_BUMPER_OUT | WP_OPTION_FULL_BUPOD, SMALL_ARM_UP_POS, 3},
#endif
   {  0,127,  4, WP_OPTION_CRAB, SMALL_ARM_UP_POS, 3},
   {  0, 30,  4, WP_OPTION_CRAB, SMALL_ARM_UP_POS, 3},

   {  0,-127,180, WP_OPTION_TANK | WP_OPTION_HARD_TURN, SMALL_ARM_UP_POS, 3},
   {  0,-127,100, WP_OPTION_TANK | WP_OPTION_HARD_TURN | WP_ARM_OPTION_SQUEEZE_OUT, SMALL_ARM_GRAB_BB_POS, 4},
   {  0,-127, 10, WP_OPTION_TANK | WP_OPTION_HARD_TURN, SMALL_ARM_GRAB_BB_POS, 4},
   {  0,-127, 10, WP_OPTION_TANK | WP_OPTION_HARD_TURN, SMALL_ARM_GRAB_BB_POS, 4},
   {  0,127, 10, WP_OPTION_TANK | WP_OPTION_HARD_TURN,SMALL_ARM_UP_BB_POS, 4},
   {  0,  0,  0, WP_OPTION_CRAB, 0, 0}};


