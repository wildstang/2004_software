/*******************************************************************************
* FILE NAME: ws_waypoints_56.c
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
#include "ws_waypoints_56.h"


/********************* PROGRAM 5 ***************************************/

/* "The Popper-Slammer" */
/****** Drive straight out and turn to hit 10 pt ball, then grab goal and
        push it under opponent's ball drop *****/

WaypointPos g_waypoints_prog5_right[]={
 /*{ x,y  }*/
   /* drive straight out and a little to the right */
   {((UINT16)STARTING_POS_RIGHT_X + 240), ((UINT16)STARTING_POS_RIGHT_Y - 51)},
   {((UINT16)STARTING_POS_RIGHT_X + 240), ((UINT16)STARTING_POS_RIGHT_Y - 51)},
   {((UINT16)STARTING_POS_RIGHT_X + 223), ((UINT16)STARTING_POS_RIGHT_Y - 13)},
   /* back up a bit to line up with goal */
   {((UINT16)STARTING_POS_RIGHT_X + 190), ((UINT16)STARTING_POS_RIGHT_Y - 10)},
   /* drive into goal & grab it */
   {((UINT16)STARTING_POS_RIGHT_X + 400), ((UINT16)STARTING_POS_RIGHT_Y)},
   {((UINT16)STARTING_POS_RIGHT_X + 400), ((UINT16)STARTING_POS_RIGHT_Y)},
   /* push goal under opponent's ball drop */
   {((UINT16)STARTING_POS_RIGHT_X + 530), ((UINT16)STARTING_POS_RIGHT_Y + 55)},
   {  0,  0}};

WaypointPos g_waypoints_prog5_left[]={
 /*{ x,y  }*/
   /* Drive straight out and a little to the left */
   {((UINT16)STARTING_POS_LEFT_X + 240), ((UINT16)STARTING_POS_LEFT_Y + 51)},
   {((UINT16)STARTING_POS_LEFT_X + 240), ((UINT16)STARTING_POS_LEFT_Y + 51)},
   {((UINT16)STARTING_POS_LEFT_X + 223), ((UINT16)STARTING_POS_LEFT_Y + 13)},
   /* back up a bit to line up with goal */
   {((UINT16)STARTING_POS_LEFT_X + 190), ((UINT16)STARTING_POS_LEFT_Y + 10)},
   /* drive into goal & grab it */
   {((UINT16)STARTING_POS_LEFT_X + 400), ((UINT16)STARTING_POS_LEFT_Y)},
   {((UINT16)STARTING_POS_LEFT_X + 400), ((UINT16)STARTING_POS_LEFT_Y)},
   /* push goal under opponent's ball drop */
   {((UINT16)STARTING_POS_LEFT_X + 530), ((UINT16)STARTING_POS_LEFT_Y - 55)},
   {  0,  0}};

WaypointInfo g_waypoints_prog5_info[]={
 /*{orientation, speed, range, options, small arm pos, max time }*/
   {  0,127,100, WP_OPTION_CRAB | WP_OPTION_FULL_BUPOD, 0, 3},
   {  0,127,  4, WP_OPTION_CRAB,SMALL_ARM_POPPER_POS, 2},
   {  0,-60,  4, WP_OPTION_TANK | WP_OPTION_HARD_TURN | WP_ARM_OPTION_SQUEEZE_OUT, SMALL_ARM_POPPER_POS, 2},
   {  0,-50,  5, WP_OPTION_TANK | WP_OPTION_HARD_TURN | WP_OPTION_BUMPER_OUT, SMALL_ARM_UP_POS, 3},
   {  0, 50,180, WP_OPTION_TANK | WP_OPTION_HARD_TURN, SMALL_ARM_UP_POS, 5},
   {  0,127,  4, WP_OPTION_TANK | WP_OPTION_HARD_TURN, SMALL_ARM_UP_POS, 5},
   {  0,127,  4, WP_OPTION_TANK | WP_OPTION_HARD_TURN | WP_ARM_OPTION_SQUEEZE_OUT, SMALL_ARM_GRAB_BB_POS, 5},
   {  0,  0,  0, WP_OPTION_CRAB, 0, 0}};



/********************* PROGRAM 6 ***************************************/

/* "The Popper-Retriever" */
/****** Drive straight out and turn to hit 10 pt ball, then grab goal and
        bring it back to our side of field *****/

WaypointPos g_waypoints_prog6_right[]={
 /*{ x,y  }*/
   {  0,  0}};

WaypointPos g_waypoints_prog6_left[]={
 /*{ x,y  }*/
   {  0,  0}};

WaypointInfo g_waypoints_prog6_info[]={
 /*{orientation, speed, range, options, small arm pos, max time }*/
   {  0,  0,  0, WP_OPTION_CRAB, 0, 0}};


