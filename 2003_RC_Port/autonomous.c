/*******************************************************************************
* FILE NAME: ws_code.c
*
* DESCRIPTION:
*
* USAGE:
*
*******************************************************************************/

#ifndef CYGWIN_IFI_SIMULATOR
#include "ifi_aliases.h"
#include "ifi_default.h"
#include "ifi_utilities.h"
#include "ws_code.h"
#include "user_routines.h"
#include "printf_lib.h"
#include "delays.h"       /*defined locally*/
#include "io_code.h"
#else /* CYGWIN_IFI_SIMULATOR include */
#include "RC_Simulator.h"
#include "stdio.h"
#include "ws_code.h"
#endif

#include "autonomous.h"

/********************* AUTONOMOUS PROGRAMS *****************************/

/********************* PROGRAM 1 ***************************************/
/* List of waypoint positions for program 1 when starting on the right side */
static const WaypointPos g_waypoints_prog1_right[]={
 /*{ x,y  }*/
   { 40, 84},
   { 66,104},
   {125,108},
   {196,108},
   {  0,  0}};
        
/* List of waypoint positions for program 1 when starting on the left side */
static const WaypointPos g_waypoints_prog1_left[]={
 /*{ x,y  }*/
   { 40,157},
   { 66,137},
   {125,133},
   {196,133},
   {  0,  0}};
                
/* Info regarding the position points for program 1 */
static const WaypointInfo g_waypoints_prog1_info[]={
 /*{orientation, speed, range }*/
   {  0,127, 12},
   {  0,127,  3},
   {  0,127,  3},
   {  0,127,  3},
   {  0,  0,  0}};
                        
/* List of the waypoints that the robot will drive to in autonomous mode.  
 * waypoint_init will set this variable to the correct waypoints based on the 
 * program selected and the side of the field that the robot is starting on.
 */
static Waypoints s_waypoint_list;

/*******************************************************************************
* FUNCTION NAME: autonomous_init
* PURPOSE:       Initialize the autonomous system...called only once 
* CALLED FROM:   
* ARGUMENTS:     
* RETURNS:       
*******************************************************************************/
void autonomous_init(void)
{
   waypoint_init();  /* Initialize the waypoint information */
}

/*******************************************************************************
* FUNCTION NAME: auto_main
* PURPOSE:       
* CALLED FROM:   
* ARGUMENTS:     
* RETURNS:       
*******************************************************************************/
void auto_main(void)
{
  UINT8 auto_state;
  PositionType curr_pos;
  PositionType target_pos;
  MotorValsType motor_vals;
  CrabTargetsType crab_targets;

  auto_state = input_data(&curr_pos);

  printf("auto_state=%d, AUTO_ON=%d\n",auto_state, AUTO_ON);

  if (auto_state == AUTO_ON)
  {
	 /* waypoint_manager will determine what our target position should be based 
     * on our current position.  The manager will return the auto_state to 
     * AUTO_OFF when the last waypoint is reached. 
	  */
    printf("CurrPosX=%d, CurrPosY=%d\n", curr_pos.x, curr_pos.y);
	 auto_state = waypoint_manager(&curr_pos, &target_pos);
    printf("TargetPosX=%d, TargetPosY=%d\n",target_pos.x, 
                                            target_pos.y);
  }

  /* Can be forced to AUTO_OFF by waypoint_manager when last waypoint reached */
  if (auto_state == AUTO_ON)
  {
    /* Based on the current position and target position, this function will 
     * set the target angle for the crab wheels.
	  */
    target_angle_speed_controller(&curr_pos, &target_pos, &crab_targets);
  }
  else
  {
    /* When auto_state is AUTO_OFF, set all the wheels to no movement and the 
     * crabs forward */
    motor_vals.lf_wheel = 127;
    motor_vals.rf_wheel = 127;
    motor_vals.lb_wheel = 127;
    motor_vals.rb_wheel = 127;
    crab_targets.left_crab_tar = 127;
    crab_targets.right_crab_tar = 127;
  }

  return;
}



/*******************************************************************************
* FUNCTION NAME: input_data
* PURPOSE:       
* CALLED FROM:   
* ARGUMENTS:     
* RETURNS:       
*******************************************************************************/
UINT8 input_data(PositionType *curr_pos)
{
  UINT8 auto_state;
  
  /* Hardcode the curr_pos for testing */
  curr_pos->x=40;
  curr_pos->y=84;

  auto_state = AUTO_ON;

  return(auto_state);
}



/*******************************************************************************
* FUNCTION NAME: waypoint_init
* PURPOSE:       Initalize s_waypoint_list based on the program selected and
*                the side of the field that the robot is starting on.
* CALLED FROM:   
* ARGUMENTS:     
* RETURNS:       
*******************************************************************************/
void waypoint_init(void)
{
  /* Need to add logic here to decode the side of the field and the program. */ 
  s_waypoint_list.p_waypoint_pos = &g_waypoints_prog1_right[0];
  s_waypoint_list.p_waypoint_info = &g_waypoints_prog1_info[0];

  return;
}



/*******************************************************************************
* FUNCTION NAME: waypoint_manager
* PURPOSE:      Determine our target position based on our current position.
*               This function will keep track which waypoint we are trying to
*               drive to and when we reach it, this fucntion will move on to
*               the next waypoint.  No code outside of this function should
*               care about waypoints.
*               The list of waypoints are stored in the globa
* CALLED FROM:   
* ARGUMENTS:    INPUT curr_pos    Current robot position
*               OUTPUT target_pos Position that the robot should be trying
*                                 to reach at this point in time.
*
* RETURNS:      AUTO_ON if we still have waypoints to reach
*               AUTO_OFF if we have reached our last waypoint
*******************************************************************************/
UINT8 waypoint_manager(PositionType *curr_pos, PositionType *target_pos)
{
  UINT8 auto_state;
  static UINT8 s_curr_waypoint_num = 0; 

  const WaypointPos*  p_curr_wp_pos;
  const WaypointInfo* p_curr_wp_info;

  int  dist_from_wp_x;
  int  dist_from_wp_y;

  /* read current waypoint to p_curr_wp_pos */
  p_curr_wp_pos = &s_waypoint_list.p_waypoint_pos[s_curr_waypoint_num]; 
  p_curr_wp_info = &s_waypoint_list.p_waypoint_info[s_curr_waypoint_num];

  /* determine how far we are from the current waypoint */
  if (curr_pos->x < p_curr_wp_pos->x_pos)
  {
     /* current X position is less than the target WP */  
     dist_from_wp_x = p_curr_wp_pos->x_pos - curr_pos->x;
  }
  else
  {
     /* current X position is greater than the target WP */  
     dist_from_wp_x = curr_pos->x - p_curr_wp_pos->x_pos;
  }

  if (curr_pos->y < p_curr_wp_pos->y_pos)
  {
     /* current Y position is less than the target WP */  
     dist_from_wp_y = p_curr_wp_pos->y_pos - curr_pos->y;
  }
  else
  {
     /* current Y position is greater than the target WP */  
     dist_from_wp_y = curr_pos->y - p_curr_wp_pos->y_pos;
  }

  /* Determine if we are within the range around the current WP.
   * If we are, move on to the next WP
   */
  if ((dist_from_wp_x <= p_curr_wp_info->range) &&
      (dist_from_wp_y <= p_curr_wp_info->range)   )
  {
     /* Current position puts us within the waypoint_range for the current
      * waypoint...advance to next waypoint */
     s_curr_waypoint_num++;
  
     /* read our new waypoint to p_curr_wp_pos */
     p_curr_wp_pos = &s_waypoint_list.p_waypoint_pos[s_curr_waypoint_num]; 
     p_curr_wp_info = &s_waypoint_list.p_waypoint_info[s_curr_waypoint_num];
  }

  /* Set the return value (target_pos) to the curr WP value */
  target_pos->x = p_curr_wp_pos->x_pos;
  target_pos->y = p_curr_wp_pos->y_pos;
  target_pos->theta = p_curr_wp_info->orientation;

  if((target_pos->x == 0) && (target_pos->y == 0))
  {
     /* If X and Y are 0 then we are at the last waypoint, so return
      * AUTO_OFF so we don't try to keep driving.
      */
     auto_state = AUTO_OFF;
  }
  else
  {
     auto_state = AUTO_ON;
  }


  return(auto_state);
}



/*******************************************************************************
* FUNCTION NAME: read_waypoint
* PURPOSE:       
* CALLED FROM:   
* ARGUMENTS:     
* RETURNS:       
*******************************************************************************/
void read_waypoint(void)
{

  return;
}



/*******************************************************************************
* FUNCTION NAME: target_angle_speed_controller
* PURPOSE:       
* CALLED FROM:   
* ARGUMENTS:     
* RETURNS:       
*******************************************************************************/
void target_angle_speed_controller(PositionType *curr_pos,
                                   PositionType *target_pos,
                                   CrabTargetsType *crab_targets)
{

  UINT8 target_heading;

  /* autonomous mode is still running */
  if ((target_pos->y >= curr_pos->y) && (target_pos->x >= curr_pos->x))
  {
    if (target_pos->x >= curr_pos->x)
    {
      /* quadrant 1 */
      /* heading = arctan((yT - yC) / (xT - xC)) */
      target_heading = arctan((target_pos->y - curr_pos->y),
                              (target_pos->x - curr_pos->x));
    }
    else
    {
      /* quadrant 2 */
      /* heading = arctan((yT - yC) / (xC - xT)) */
      target_heading = arctan((target_pos->y - curr_pos->y),
                              (curr_pos->x - target_pos->x));
      target_heading = 127 - target_heading;
    }
  }
  else
  {
    if (curr_pos->x >= target_pos->x)
    {
      /* quadrant 3 */
      /* heading = arctan((yC - yT) / (xC - xT)) */
      target_heading = arctan((curr_pos->y - target_pos->y),
                              (curr_pos->x - target_pos->x));
      target_heading = 127 + target_heading;
    }
    else
    {
      /* quadrant 4 */
      /* heading = arctan((yC - yT) / (xT - xC)) */
      target_heading = arctan((curr_pos->y - target_pos->y),
                              (target_pos->x - curr_pos->x));
      target_heading = 256 - target_heading;
    }
  }

  crab_position_controller(target_heading, curr_pos, crab_targets);

  return;
}



/*******************************************************************************
* FUNCTION NAME: crab_position_controller
* PURPOSE:       
* CALLED FROM:   
* ARGUMENTS:     
* RETURNS:       
*******************************************************************************/
void crab_position_controller(UINT8 target_heading, PositionType *curr_pos,
                              CrabTargetsType *crab_targets)
{

  if (target_heading > curr_pos->theta)
  {
    crab_targets->left_crab_tar = 127 + (target_heading - curr_pos->theta);
  }
  else
  {
    crab_targets->left_crab_tar = 127 - (curr_pos->theta - target_heading);
  }

  /* output is in brads, must scale to pot values later */
  crab_targets->right_crab_tar = crab_targets->left_crab_tar;

  return;
}
