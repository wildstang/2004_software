/*******************************************************************************
* FILE NAME: ws_autonomous.c
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
#include "ws_includes.h"
#include "user_routines.h"
#include "printf_lib.h"
#include "delays.h"       /*defined locally*/
#include "ws_io.h"
#include "ws_serial2.h"
#include "ws_sync_serial.h"
#else /* CYGWIN_IFI_SIMULATOR include */
#include "RC_Simulator.h"
#include "stdio.h"
#include "ws_includes.h"
#endif

#include "ws_general.h"
#include "ws_trig.h"
#include "ws_autonomous.h"
#include "ws_crab_feedback.h"
#include "ws_arms.h"
#include "ws_feedback.h"
#include "ws_cc.h"
#include "ws_waypoints_012.h"
#include "ws_waypoints_34.h"
#include "ws_waypoints_56.h"

#define BUPOD_MOTOR_SCALE       2  /* was 3 for competition robot */

UINT8 g_forced_auto = FALSE;
static UINT8 s_prog_num = 0;
static UINT8 s_curr_waypoint_num = 0;
static UINT8 prev_competition_mode = ROBOT_DISABLED;


/* List of the waypoints that the robot will drive to in autonomous mode.  
 * waypoint_init will set this variable to the correct waypoints based on the 
 * program selected and the side of the field that the robot is starting on.
 */
static Waypoints s_waypoint_list;

/* constants for small arm feedback */
const rom FeedbackConstsType g_small_arm_feedback_constants={
  SMALL_ARM_POSITION_NEAR_UP,
  SMALL_ARM_POSITION_CLOSE_UP,
  SMALL_ARM_POSITION_FAR_UP,
  SMALL_ARM_POSITION_NEAR_DOWN,
  SMALL_ARM_POSITION_CLOSE_DOWN,
  SMALL_ARM_POSITION_FAR_DOWN,
  SMALL_ARM_SLOW_UP,
  SMALL_ARM_FAST_UP,
  SMALL_ARM_SLOW_DOWN,
  SMALL_ARM_FAST_DOWN};

/*******************************************************************************
* FUNCTION NAME: autonomous_init
* PURPOSE:       Initialize the autonomous system...called only once 
* CALLED FROM:   
* ARGUMENTS:     
* RETURNS:       
*******************************************************************************/
void autonomous_init(void)
{
  s_waypoint_list.p_waypoint_pos = &g_waypoints_prog1_right[3];
  s_waypoint_list.p_waypoint_info = &g_waypoints_prog1_info[3];

  return;
}


/*******************************************************************************
* FUNCTION NAME: auto_chooser
* PURPOSE:       set autonomous program & side, and lock in forced auto
* ARGUMENTS:     none
* RETURNS:       none
*******************************************************************************/
void auto_chooser(void)
{
  static UINT8 prev_user_display_mode = 0;

  if (user_display_mode == 0)
  {
    PositionType starting_pos;

    /* when we leave user mode we turn off forced auto */
    g_forced_auto = FALSE;

    /*
    printf("auto selector %3d, field_side=%d\n", (int)Oi_auton_selector,
           (int)Dig_in_field_side );
    */

    /* read selector knob and side switch to get autonomous program number */
    if ((Oi_auton_selector >= (WAYPOINT_OI_SEL1 - WAYPOINT_OI_DIFF)) &&
        (Oi_auton_selector <= (WAYPOINT_OI_SEL1 + WAYPOINT_OI_DIFF)))
    {
      if (Dig_in_field_side == LEFT_SIDE)
      {
        s_waypoint_list.p_waypoint_pos = &g_waypoints_prog1_left[0];
      }
      else
      {
        s_waypoint_list.p_waypoint_pos = &g_waypoints_prog1_right[0];
      }
      s_waypoint_list.p_waypoint_info = &g_waypoints_prog1_info[0];
      s_prog_num = 1;
    }
    else if ((Oi_auton_selector >= (WAYPOINT_OI_SEL2 - WAYPOINT_OI_DIFF)) &&
             (Oi_auton_selector <= (WAYPOINT_OI_SEL2 + WAYPOINT_OI_DIFF)))
    {
      if (Dig_in_field_side == LEFT_SIDE)
      {
        s_waypoint_list.p_waypoint_pos = &g_waypoints_prog2_left[0];
      }
      else
      {
        s_waypoint_list.p_waypoint_pos = &g_waypoints_prog2_right[0];
      }
      s_waypoint_list.p_waypoint_info = &g_waypoints_prog2_info[0];
      s_prog_num = 2;
    }
    else if ((Oi_auton_selector >= (WAYPOINT_OI_SEL3 - WAYPOINT_OI_DIFF)) &&
             (Oi_auton_selector <= (WAYPOINT_OI_SEL3 + WAYPOINT_OI_DIFF)))
    {
      if (Dig_in_field_side == LEFT_SIDE)
      {
        s_waypoint_list.p_waypoint_pos = &g_waypoints_prog3_left[0];
      }
      else
      {
        s_waypoint_list.p_waypoint_pos = &g_waypoints_prog3_right[0];
      }
      s_waypoint_list.p_waypoint_info = &g_waypoints_prog3_info[0];
      s_prog_num = 3;
    }
    else if ((Oi_auton_selector >= (WAYPOINT_OI_SEL4 - WAYPOINT_OI_DIFF)) &&
             (Oi_auton_selector <= (WAYPOINT_OI_SEL4 + WAYPOINT_OI_DIFF)))
    {
      if (Dig_in_field_side == LEFT_SIDE)
      {
        s_waypoint_list.p_waypoint_pos = &g_waypoints_prog4_left[0];
      }
      else
      {
        s_waypoint_list.p_waypoint_pos = &g_waypoints_prog4_right[0];
      }
      s_waypoint_list.p_waypoint_info = &g_waypoints_prog4_info[0];
      s_prog_num = 4;
    }
    else if ((Oi_auton_selector >= (WAYPOINT_OI_SEL5 - WAYPOINT_OI_DIFF)) &&
             (Oi_auton_selector <= (WAYPOINT_OI_SEL5 + WAYPOINT_OI_DIFF)))
    {
      if (Dig_in_field_side == LEFT_SIDE)
      {
        s_waypoint_list.p_waypoint_pos = &g_waypoints_prog5_left[0];
      }
      else
      {
        s_waypoint_list.p_waypoint_pos = &g_waypoints_prog5_right[0];
      }
      s_waypoint_list.p_waypoint_info = &g_waypoints_prog5_info[0];
      s_prog_num = 5;
    }
    else if ((Oi_auton_selector >= (WAYPOINT_OI_SEL6 - WAYPOINT_OI_DIFF)) &&
             (Oi_auton_selector <= (WAYPOINT_OI_SEL6 + WAYPOINT_OI_DIFF)))
    {
      if (Dig_in_field_side == LEFT_SIDE)
      {
        s_waypoint_list.p_waypoint_pos = &g_waypoints_prog6_left[0];
      }
      else
      {
        s_waypoint_list.p_waypoint_pos = &g_waypoints_prog6_right[0];
      }
      s_waypoint_list.p_waypoint_info = &g_waypoints_prog6_info[0];
      s_prog_num = 6;
    }
    else
    {
      /* default to dead auto program */
      if (Dig_in_field_side == LEFT_SIDE)
      {
        s_waypoint_list.p_waypoint_pos = &g_waypoints_prog0_left[0];
      }
      else
      {
        s_waypoint_list.p_waypoint_pos = &g_waypoints_prog0_right[0];
      }
      s_waypoint_list.p_waypoint_info = &g_waypoints_prog0_info[0];
      s_prog_num = 0;
    }

    /* send starting postition to CC */
    if (Dig_in_field_side == LEFT_SIDE)
    {
      starting_pos.x=STARTING_POS_LEFT_X;
      starting_pos.y=STARTING_POS_LEFT_Y;
      starting_pos.theta=STARTING_POS_LEFT_T;
    }
    else
    {
      starting_pos.x=STARTING_POS_RIGHT_X;
      starting_pos.y=STARTING_POS_RIGHT_Y;
      starting_pos.theta=STARTING_POS_RIGHT_T;
    }
    cc_set_position(&starting_pos);  /* Set the position to the CC */

  }
  else if ((user_display_mode == 1) && (prev_user_display_mode == 0) &&
           (Oi_sw_auton_lock1 == 1) && (Oi_sw_auton_lock2 == 1))
  {
    /* if we just entered user mode, and the joystick buttons are pressed,
       enable forced auto */
    g_forced_auto = TRUE;

    /* recalibrate gyro */
    cc_gyro_calibrate();
  }

  prev_user_display_mode = user_display_mode;

  return;
}


/*******************************************************************************
* FUNCTION NAME: auto_main
* PURPOSE:       main loop for autonomous
* ARGUMENTS:     none
* RETURNS:       none
*******************************************************************************/
void auto_main(CalibrationValsType *calibration_vals)
{
  UINT8 auto_state;
  PositionType curr_pos;
  PositionType target_pos;
  WaypointDataType waypoint_data;
  MotorValsType motor_vals;
  CrabTargetsType crab_targets;
  UINT8 arm_tar_pot_vals;

  while ((autonomous_mode) || (g_forced_auto == TRUE))
  {
    CcReturnType  cc_ret_val;

    if (statusflag.NEW_SPI_DATA)      /* 26.2ms loop area */
    {
      Getdata(&rxdata); /* DO NOT DELETE, or it will be stuck here forever */

      if ((autonomous_mode == 1) || (user_display_mode == 0))
      {
        g_forced_auto = FALSE;
      }

      if ((competition_mode == ROBOT_ENABLED) &&
          (prev_competition_mode == ROBOT_DISABLED))
      {
        /* send starting position to CC just before we start moving */
        PositionType starting_pos;

        if (Dig_in_field_side == LEFT_SIDE)
        {
          starting_pos.x=STARTING_POS_LEFT_X;
          starting_pos.y=STARTING_POS_LEFT_Y;
          starting_pos.theta=STARTING_POS_LEFT_T;
        }
        else
        {
          starting_pos.x=STARTING_POS_RIGHT_X;
          starting_pos.y=STARTING_POS_RIGHT_Y;
          starting_pos.theta=STARTING_POS_RIGHT_T;
        }
        cc_set_position(&starting_pos);  /* Set the position to the CC */
      }

      /* Wildstang autonomous code */

      /* Get the robot's current position from the CC */
      cc_ret_val = cc_get_position(&curr_pos);
      if(cc_ret_val == CC_SUCCESS)
      {
         auto_state = AUTO_ON;
      }
      else
      {
         auto_state = AUTO_OFF;
      }

      /*printf("-auto_state=%d, AUTO_ON=%d\n",(int)auto_state, (int)AUTO_ON);*/

      /* If input_data returns AUTO_ON, that means it talked to the CC, if not
       * then the only way we should continue is if the WP is a timepoint which
       * doesn't need the CC.
       * TODO: This func should have no knowledge of s_curr_waypoint_num!!!
       */
      if ((auto_state == AUTO_ON) ||
          ((s_waypoint_list.p_waypoint_info[s_curr_waypoint_num].option_flags &
            WP_OPTION_TIMEPOINT) == WP_OPTION_TIMEPOINT))
      {
        /* waypoint_manager will determine what our target position should be
         * based on our current position.  The manager will return the
         * auto_state to AUTO_OFF when the last waypoint is reached. 
         */
        /*
        printf("CPos X %d Y %d T %d", (int)curr_pos.x, (int)curr_pos.y,
               (int)curr_pos.theta);
        */

        User_Byte1.allbits = (UINT8)(curr_pos.x & 0x00FF); /* lower byte of X */
        User_Byte2.allbits = (UINT8)(curr_pos.y & 0x00FF); /* lower byte of Y */
        /* lower nibble of top byte of X in lower nibble
         * lower nibble of top byte of Y in upper nibble */
        User_Byte3 = (UINT8)(((curr_pos.x >> 8) & 0x000F) |
                             ((curr_pos.y >> 4) & 0x00F0));
        User_Byte4 = curr_pos.theta;               /* theta */
        User_Byte5 = (autonomous_mode&0x1) | ((competition_mode&0x1) << 1) |
                     ((auto_state&0x1) << 2) | ((cc_ret_val&0x1) << 3);

        auto_state = waypoint_manager(&curr_pos, &target_pos, &waypoint_data);
        /*
        printf(" TPos X %d Y %d T %d",(int)target_pos.x,
               (int)target_pos.y, (int)target_pos.theta);
        */
      }

      /* Can be forced to AUTO_OFF by waypoint_manager when last waypoint
         reached */
      if (auto_state == AUTO_ON)
      {
        /* Based on the current position and target position, this function will
         * set the target angle for the crab wheels.
         */
        target_angle_speed_controller(&curr_pos, &target_pos, &crab_targets,
                                      &motor_vals, &waypoint_data,
                                      calibration_vals);

        /*
        printf(" cTar %d s %d ", (int)crab_targets.crab_tar,
            (int)motor_vals.crab);
        printf(" lf %d rf %d lb %d rb %d", (int)motor_vals.lf_wheel,
               (int)motor_vals.rf_wheel, (int)motor_vals.lb_wheel,
               (int)motor_vals.rb_wheel);
        */

#if 1
        /* set small arm squeezer position */
        if ((waypoint_data.option_flags & WP_ARM_OPTION_SQUEEZE_OUT) ==
            WP_ARM_OPTION_SQUEEZE_OUT)
        {
          motor_vals.small_arm_squeeze = SQUEEZE_OUT;
        }
        else
        {
          motor_vals.small_arm_squeeze = SQUEEZE_IN;
        }

        /* feedback for small arm */
        /*printf("lo %d hi %d ", (int)calibration_vals->small_arm_shoulder_lo,
               (int)calibration_vals->small_arm_shoulder_hi);*/
        arm_tar_pot_vals = arm_scaling(waypoint_data.s_arm_pos,
                                       calibration_vals->small_arm_shoulder_lo,
                                       calibration_vals->small_arm_shoulder_hi);
        /*printf("tgt %d curr %d ", (int)arm_tar_pot_vals,
               (int)GET_ANALOG_VALUE_SHIFT(Analog_in_small_arm_shoulder_pot));*/

        motor_vals.small_arm_shoulder = general_pos_feedback(
              GET_ANALOG_VALUE_SHIFT(Analog_in_small_arm_shoulder_pot),
              arm_tar_pot_vals, &g_small_arm_feedback_constants);
        /*printf("spd %d ", (int)motor_vals.small_arm_shoulder);*/
#else
        motor_vals.small_arm_shoulder = 0;
        motor_vals.small_arm_squeeze = SQUEEZE_IN;
#endif


#if 1
        if ((waypoint_data.option_flags & WP_OPTION_SUSP_DOWN) ==
            WP_OPTION_SUSP_DOWN)
        {
          motor_vals.suspension_front = SUSPENSION_DOWN;
          motor_vals.suspension_back = SUSPENSION_DOWN;
        }
        else
        {
          motor_vals.suspension_front = SUSPENSION_UP;
          motor_vals.suspension_back = SUSPENSION_UP;
        }

        if ((waypoint_data.option_flags & WP_OPTION_BUMPER_OUT) ==
            WP_OPTION_BUMPER_OUT)
        {
          motor_vals.front_bumper = BUMPER_OUT;
        }
        else
        {
          motor_vals.front_bumper = BUMPER_IN;
        }
#else

#endif
      }
      else
      {
        /* When auto_state is AUTO_OFF, set all the wheels & arms to no
           movement and the crabs forward */
        motor_vals.lf_wheel = 0;
        motor_vals.rf_wheel = 0;
        motor_vals.lb_wheel = 0;
        motor_vals.rb_wheel = 0;
        crab_targets.crab_tar = 127;
        crab_system(&crab_targets, &motor_vals, calibration_vals);
        motor_vals.small_arm_shoulder = 0;
        motor_vals.small_arm_squeeze = SQUEEZE_IN;
        motor_vals.front_bumper = BUMPER_IN;
      }

      if ((waypoint_data.option_flags & WP_OPTION_SUSP_DOWN) !=
          WP_OPTION_SUSP_DOWN)
      {
        motor_vals.suspension_front = SUSPENSION_UP;
        motor_vals.suspension_back = SUSPENSION_UP;
      }

      motor_vals.big_arm_shoulder = 0;
      motor_vals.big_arm_elbow = 0;
      motor_vals.winch = 0;
      motor_vals.accumulator = ACCUMULATOR_STOP;
      motor_vals.back_bumper = BUMPER_IN;
      pump_control(&motor_vals);

      /*
      printf(" wheels lf %d rf %d lb %d rb %d", (int)motor_vals.lf_wheel,
             (int)motor_vals.rf_wheel, (int)motor_vals.rb_wheel,
             (int)motor_vals.lb_wheel);
      */

      printf("\n");

      prev_competition_mode = competition_mode;

      assign_outputs_slow(&motor_vals);
      Putdata(&txdata);   /* DO NOT DELETE, or you will get no PWM outputs! */
    }

    assign_outputs_fast(&motor_vals);
    Generate_Pwms(pwm13,pwm14,pwm15,pwm16);

    display_auto_data();
  }

  return;
}



/*******************************************************************************
* FUNCTION NAME: waypoint_manager
* PURPOSE:      Determine our target position based on our current position.
*               This function will keep track which waypoint we are trying to
*               drive to and when we reach it, this fucntion will move on to
*               the next waypoint.  No code outside of this function should
*               care about waypoints.
*               The list of waypoints are stored in the global section
* CALLED FROM:   
* ARGUMENTS:    INPUT curr_pos    Current robot position
*               OUTPUT target_pos Position that the robot should be trying
*                                 to reach at this point in time.
*
* RETURNS:      AUTO_ON if we still have waypoints to reach
*               AUTO_OFF if we have reached our last waypoint
*******************************************************************************/
UINT8 waypoint_manager(PositionType *curr_pos, PositionType *target_pos,
                       WaypointDataType *waypoint_data)
{
  UINT8 auto_state=AUTO_ON;

  WaypointPos*  p_curr_wp_pos;
  WaypointInfo* p_curr_wp_info;

  int  dist_from_wp_x;
  int  dist_from_wp_y;

  static UINT8 s_starting_pkt_num = 0;  /* Used for TIMEPOINTs and max time for
                                           crab & tank points */
  static UINT8 max_time_reached = FALSE; /* Remembers when auto should be off */

  /* read current waypoint to p_curr_wp_pos */
  p_curr_wp_pos = &s_waypoint_list.p_waypoint_pos[s_curr_waypoint_num]; 
  p_curr_wp_info = &s_waypoint_list.p_waypoint_info[s_curr_waypoint_num];

  /*printf("WP=%d, XT=%d, YT=%d ",(int)s_curr_waypoint_num,
         p_curr_wp_pos->x_pos, p_curr_wp_pos->y_pos);*/

  if((p_curr_wp_pos->x_pos == 0) && (p_curr_wp_pos->x_pos == 0))
  {
     /* If X and Y are 0 then we are at the last waypoint, so return
      * AUTO_OFF so we don't try to keep driving.
      */
     auto_state = AUTO_OFF;
     printf(" OFF1");
  }

  /******************** TIMEPOINT *********************/
  if((auto_state == AUTO_ON) &&
     ((p_curr_wp_info->option_flags & WP_OPTION_TIMEPOINT) ==
      WP_OPTION_TIMEPOINT) &&
     (competition_mode == ROBOT_ENABLED))   /* must be enabled */
  {
    printf("pkt %d str %d ", (int)rxdata.packet_num,
           (int)s_starting_pkt_num);

     /* This WP is a timepoint */
     if (s_starting_pkt_num == 0)
     {
        /* We just started in a timepoint, set the curr packet num and
         * wait until we get that packet + wpinfo.speed packet.  This
         * means we have waited the needed amount of time.
         */
        s_starting_pkt_num = rxdata.packet_num;
     }
     else if((rxdata.packet_num - s_starting_pkt_num) >
             (p_curr_wp_info->speed*10) )
     {
        /* We have reached our desired time, move to the next WP */
        printf("adv1 ");

        /* Advance to next waypoint */
        s_curr_waypoint_num++;

        /* read our new waypoint to p_curr_wp_pos */
        p_curr_wp_pos = &s_waypoint_list.p_waypoint_pos[s_curr_waypoint_num]; 
        p_curr_wp_info = &s_waypoint_list.p_waypoint_info[s_curr_waypoint_num];
     }
     /* else keep waiting until the desired time is complete */
  }

  /* Check again to see if this is not a timepoint since it is possible that
   * we just finished a timepoint and are moving on to a real waypoint
   */
  /******************** WAYPOINT *********************/
  if((auto_state == AUTO_ON) &&
     (p_curr_wp_info->option_flags & WP_OPTION_TIMEPOINT) !=
      WP_OPTION_TIMEPOINT)
  {
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
     printf(" dx=%d dy=%d ", (int)dist_from_wp_x, (int)dist_from_wp_y);
     if ((dist_from_wp_x <= p_curr_wp_info->range) &&
         (dist_from_wp_y <= p_curr_wp_info->range)   )
     {
        /* Current position puts us within the waypoint_range for the current
         * waypoint...advance to next waypoint */
        printf("adv2 ");
        s_curr_waypoint_num++;

        /* Sets new starting packet when waypoint is reached */
        s_starting_pkt_num = rxdata.packet_num;

        /* read our new waypoint to p_curr_wp_pos */
        p_curr_wp_pos = &s_waypoint_list.p_waypoint_pos[s_curr_waypoint_num]; 
        p_curr_wp_info = &s_waypoint_list.p_waypoint_info[s_curr_waypoint_num];
     }

     if ((competition_mode == ROBOT_ENABLED) &&
         (prev_competition_mode == ROBOT_DISABLED))
     {
       s_starting_pkt_num = rxdata.packet_num;
     }

     /* this subtraction works even if s_starting_pkt_num is greater than
        current packet number */
     if ((competition_mode == ROBOT_ENABLED) &&
         ((rxdata.packet_num - s_starting_pkt_num) >=
         (p_curr_wp_info->max_time * NUM_PACKETS_PER_SEC)))
     {
       max_time_reached = TRUE;
     }

     if (max_time_reached == TRUE)
     {
       auto_state = AUTO_OFF;
       printf(" OFF2");
     }
  }


  /* Set the return value (target_pos) to the curr WP value */
  target_pos->x = p_curr_wp_pos->x_pos;
  target_pos->y = p_curr_wp_pos->y_pos;
  target_pos->theta = p_curr_wp_info->orientation;
  waypoint_data->speed = p_curr_wp_info->speed;
  waypoint_data->option_flags = p_curr_wp_info->option_flags;
  waypoint_data->s_arm_pos = p_curr_wp_info->s_arm_pos;

  if((target_pos->x == 0) && (target_pos->y == 0))
  {
     /* If X and Y are 0 then we are at the last waypoint, so return
      * AUTO_OFF so we don't try to keep driving.
      */
     auto_state = AUTO_OFF;
     printf(" OFF3");
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
* PURPOSE:      Determine how to move from the current position to 
*               the target position. 
* CALLED FROM:  auto_main 
* ARGUMENTS:    INPUT curr_pos       Current robot position  
*                     target_pos     Position that the robot should be trying
*                                    to reach at this point in time.
*               OUTPUT crab_targets  Crab wheel positioning to reach target pos
*
* RETURNS:       
*******************************************************************************/
void target_angle_speed_controller(PositionType *curr_pos,
                                   PositionType *target_pos,
                                   CrabTargetsType *crab_targets,
                                   MotorValsType *motor_vals,
                                   WaypointDataType *waypoint_data,
                                   CalibrationValsType *calibration_vals)
{

  UINT8 target_heading;
  UINT8 raw_target_heading;
  UINT16 dist_from_target;
  UINT16 y_diff;
  UINT16 x_diff;
  INT16  cos_to_targ;

  /* autonomous mode is still running */

  /*
   * Refer to this diagram when reading the following code.
   *
   * Quadrants on the field compass-wise.
   *
   *            (positive y)
   *
   *                 63
   *         Quad 2  |   Quad 1
   *                 |
   *                 |
   *        127 ----------- 0 / 255  (positive x)
   *                 |
   *                 |
   *         Quad 3  |   Quad 4
   *                191
   */


  /* Determine the target heading. */
  if (target_pos->y >= curr_pos->y)
  {
    if (target_pos->x >= curr_pos->x)
    {
      /* quadrant 1 (x is positive, y is positive) */
      y_diff = target_pos->y - curr_pos->y;
      x_diff = target_pos->x - curr_pos->x;

      /* heading = arctan((deltaY) / (deltaX))...use 16bit arctan */
      raw_target_heading = arctan16(y_diff,
                                    x_diff);
      target_heading = raw_target_heading;
    }
    else
    {
      /* quadrant 2 (x is negative, y is positive) */
      y_diff = target_pos->y - curr_pos->y;
      x_diff = curr_pos->x - target_pos->x;

      /* heading = arctan((deltaY) / (deltaX))...use 16bit arctan */
      raw_target_heading = arctan16(y_diff,
                                    x_diff);

      /* Quadrant 2 encompasses 63 to 127 brads with 127 being at the x-axis. */
      target_heading = 127 - raw_target_heading;
    }
  }
  else
  {
    if (target_pos->x < curr_pos->x)
    {
      /* quadrant 3 (x is negative, y is negative) */
      y_diff = curr_pos->y - target_pos->y;
      x_diff = curr_pos->x - target_pos->x;

      /* heading = arctan((deltaY) / (deltaX))...use 16bit arctan */
      raw_target_heading = arctan16(y_diff,
                                    x_diff);

      /* Quadrant 3 encompasses 127 to 191 brads with 127 being at the x-axis.*/
      target_heading = 127 + raw_target_heading;
    }
    else
    {
      /* quadrant 4 (x is positive, y is negative) */
      y_diff = curr_pos->y - target_pos->y;
      x_diff = target_pos->x - curr_pos->x;

      /* heading = arctan((deltaY) / (deltaX))...use 16bit arctan */
      raw_target_heading = arctan16(y_diff,
                                    x_diff);

      /* Quadrant 4 encompasses 191 to 255 brads with 255 being at the x-axis.*/
      target_heading = 255 - raw_target_heading;
    }
  }

  /* Cap x_diff at 255 so we don't overflow our int */
  MAX(x_diff, 255);
  cos_to_targ = cos(raw_target_heading);

  if(cos_to_targ == 0)
  {
    /* Don't div by 0, dist is just the y component */
    dist_from_target = y_diff;
  }
  else
  {
    /* cos(theta) = adj/hyp, so hyp=adj/cos(theta)...cos returns ans*255 */
    dist_from_target = ((int)255*(x_diff)) /
                        ((int)cos_to_targ);
  }

  printf(" hdg %3d ", (int)target_heading);

  if (waypoint_data->speed < 0)
  {
    /* make target 180 degrees from target since we are driving rev */
    target_heading = target_heading + 127;  /* Wrap around is OK here */
  }

  /* set wheel speed */
  motor_vals->lf_wheel = waypoint_data->speed;
  motor_vals->rf_wheel = waypoint_data->speed;

  if ((waypoint_data->option_flags & WP_OPTION_FULL_BUPOD) ==
      WP_OPTION_FULL_BUPOD)
  {
    motor_vals->lb_wheel = waypoint_data->speed;
    motor_vals->rb_wheel = waypoint_data->speed;
  }
  else
  {
    motor_vals->lb_wheel = (waypoint_data->speed / BUPOD_MOTOR_SCALE);
    motor_vals->rb_wheel = (waypoint_data->speed / BUPOD_MOTOR_SCALE);
  }

  if ((waypoint_data->option_flags & WP_OPTION_CRAB) == WP_OPTION_CRAB)
  {
    /* crab steering */
    /* Now use the target heading to determine the crab targets. */
    printf("#");
    crab_position_controller(target_heading, curr_pos, crab_targets);
  }
  else if((waypoint_data->option_flags & WP_OPTION_TIMEPOINT) ==
                                                          WP_OPTION_TIMEPOINT)
  {
    /* Timepoints set the wheels to X,Y and crabs to straight ahead */
    crab_targets->crab_tar = 127;
    motor_vals->lf_wheel = target_pos->x;
    motor_vals->lb_wheel = target_pos->x;

    if ((waypoint_data->option_flags & WP_OPTION_FULL_BUPOD) ==
        WP_OPTION_FULL_BUPOD)
    {
      motor_vals->rf_wheel = target_pos->y;
      motor_vals->rb_wheel = target_pos->y;
    }
    else
    {
      motor_vals->rf_wheel = (target_pos->y / BUPOD_MOTOR_SCALE);
      motor_vals->rb_wheel = (target_pos->y / BUPOD_MOTOR_SCALE);
    }

    printf("$");
  }
  else  /* Else assume OPTION_TANK */
  {
    /* For now, can't deal with going to 1" resolution so just go back to 2"
     * for SetHeading.
     */
    UINT16 tmp_dist_from_target;

    if ((waypoint_data->option_flags & WP_OPTION_HARD_TURN) ==
        WP_OPTION_HARD_TURN)
    {
      tmp_dist_from_target = (dist_from_target / 8);
    }
    else
    {
      tmp_dist_from_target = (dist_from_target / 2);
    }

    /* Cap dist from target at 255 before using it as a byte */
    MAX(tmp_dist_from_target,255);

    /* tank steering */
    SetHeading(curr_pos->theta , target_heading, 
               motor_vals, (UINT8)tmp_dist_from_target);
    crab_targets->crab_tar = 127;
    printf("@");
  }

  crab_system(crab_targets, motor_vals, calibration_vals);

  return;
}



/*******************************************************************************
* FUNCTION NAME: crab_position_controller
* PURPOSE:     Determine the crab wheel positioning needed to 
*              reach the target heading.
* CALLED FROM: target_angle_speed_controller
* ARGUMENTS:  INPUT target_heading   Angle from the robot's current position
*                                    to the target waypoint.
*                   curr_pos         Current robot position
*             OUTPUT crab_targets    Crab wheel positioning to reach target pos
* RETURNS:
*******************************************************************************/
void crab_position_controller(UINT8 target_heading, PositionType *curr_pos,
                              CrabTargetsType *crab_targets)
{

  /*
   * Refer to this diagram when reading the following code.
   *
   * Quadrants for crab positioning. 
   *
   *                 127
   *         Quad 2  |   Quad 1
   *                 |
   *                 |
   *        191 ----------- 63
   *                 |
   *                 |
   *         Quad 3  |   Quad 4
   *                 255
   */


  /* Adjust the target heading to the crab positioning quadrants. */
  if (target_heading > curr_pos->theta)
  {
    crab_targets->crab_tar = 127 + (target_heading - curr_pos->theta);
  }
  else
  {
    crab_targets->crab_tar = 127 - (curr_pos->theta - target_heading);
  }

  /* output is in brads, must scale to pot values later */

  return;
}

/*******************************************************************************
* FUNCTION NAME: SetHeading
* PURPOSE: Will set the left and right wheel PWM motor values to cause
*          the robot to turn to the target_heading based on its heading
* CALLED FROM: 
* ARGUMENTS:  INPUT  curr_head    Heading at which we are at
*
*                    target_head  Heading we want to go to
*             OUTPUT motor_vals   New speed and direction for wheel to run
* RETURNS:
*******************************************************************************/

/* Make the speed diference between wheels be this fraction times the dist
 * from target.
 */
#define DIST_SCALING_NUMERATOR   1
#define DIST_SCALING_DENOMINATOR 40

void SetHeading(INT8 current_heading, INT8 target_heading,
               MotorValsType *p_motor_vals,
               UINT8 dist_from_target)
{
  INT16 speed_diff;
  INT8  diff_heading;
  INT16 temp_left;
  INT16 temp_right;
  UINT16 speed_scale;

  /* Invert the dist from target so 0=far, 255=close */
  printf("d=%d ",(int)dist_from_target);
  dist_from_target = 255 - dist_from_target;
  MIN(dist_from_target,1);  /* This is used for div, don't allow a zero */

  speed_scale = ((int)(dist_from_target*DIST_SCALING_NUMERATOR)/
                  DIST_SCALING_DENOMINATOR);
  printf("c %d t %d ts=%d ", (int)current_heading, (int)target_heading,
                            (int)speed_scale);

  /* take difference between current & target headings */
  diff_heading = target_heading - current_heading;

  if (p_motor_vals->lf_wheel > 0)
  {
    /* Left front is moving forward,  */
    /* Notice, the div by 4, and div by 32...same as a div by 127 but it
     * prevents intermediate overflow.
     */
    speed_diff = ((((int)diff_heading * (int)(p_motor_vals->lf_wheel)) /
                  4 * (int)speed_scale) / 32);
  }
  else
  {
    /* Left front is moving backward */
    /* The robot handles very differently backward.  It has a difficult time 
     * turning in reverse.  Increase the differential so it can adjust.
     */
    /* Notice, the div by 4, and div by 32...same as a div by 127 but it
     * prevents intermediate overflow.
     */
    speed_diff = ((((int)diff_heading * (int)(0 - p_motor_vals->lf_wheel)) /
                  4 * (int)speed_scale) / 32);
  }

  /*
  printf("L1 %d R1 %d ", (int)p_motor_vals->lf_wheel,
         (int)p_motor_vals->rf_wheel);
  */
  printf("dff %d ", speed_diff);

  temp_left = p_motor_vals->lf_wheel - speed_diff;
  temp_right = p_motor_vals->rf_wheel + speed_diff;

  /* Cap PWMs at 127,-127 */
  MAX(temp_left, 127);
  MIN(temp_left, -127);
  MAX(temp_right, 127);
  MIN(temp_right, -127);

  /*
  printf("tr %d tl %d ", (int)temp_right, (int)temp_left);
  */

  /* change motor speeds used the signed value */
  p_motor_vals->lf_wheel = (UINT8)temp_left;
  p_motor_vals->lb_wheel = (UINT8)((int)temp_left / (int)BUPOD_MOTOR_SCALE);
  p_motor_vals->rf_wheel = (UINT8)temp_right;
  p_motor_vals->rb_wheel = (UINT8)((int)temp_right / (int)BUPOD_MOTOR_SCALE);

  printf("L %d R %d ", (int)p_motor_vals->lf_wheel,
         (int)p_motor_vals->rf_wheel);

  return;
}



/*******************************************************************************
* FUNCTION NAME: display_auto_data
* PURPOSE:       display autonomous data on the OI user display
* ARGUMENTS:     none
* RETURNS:       none
*******************************************************************************/
void display_auto_data(void)
{
  UINT8 display_data;

#if 0
  if (competition_mode == ROBOT_ENABLED)
  {
    /* if enabled, display waypoint number only */
  }
  else
  {
#endif
    /* when not enabled, display auton info */
    /* info encoded in user bit (currently only 9 progs work with this scheme)
     * UserByte = (Left * 100) + (prog# * 10) + (current waypoint index) +
     *            (9 if forced auto)
     * so, 120 = Left Prog2 Index0
     *     155 = Left Prog5 Index5
     *     023 = Right Prog2 Index3
     *     029 = Right Prog2 Index9 or Right Prog2 Index0 Forced Auto
     */

    /* set to (10 * program) so 10 = prog 1, 20 = prog 2, 30 = prog 3, etc */
    display_data = 10 * s_prog_num;

    /* if we are working on the left side, add 100 */
    if (Dig_in_field_side == LEFT_SIDE)
    {
      display_data += 100;
    }

    /* add the waypoint index we are currently at */
    display_data += s_curr_waypoint_num;

    if ((g_forced_auto == TRUE) && (competition_mode == ROBOT_DISABLED))
    {
      /* increment waypoint index by 9 when in forced auto */
      display_data += 9;
    }

    User_Mode_byte = display_data;
#if 0
  }
#endif
}

