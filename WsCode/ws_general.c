/*******************************************************************************
* FILE NAME: ws_general.c
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
#else
#include "RC_Simulator.h"
#include "ifi_aliases.h"
#include "ifi_default.h"
#include "stdio.h"
#include "ws_includes.h"
#endif

#include "ws_general.h"
#include "ws_io.h"
#include "ws_arms.h"
#include "ws_feedback.h"

/* used to store the packet number when the bumper was told to go out and when
   suspension was told to go down-down state */
static UINT8 packet_num_b_bumper_out = 0;
static UINT8 packet_num_f_bumper_out = 0;
static UINT8 packet_num_susp_down = 0;

extern const rom FeedbackConstsType g_small_arm_feedback_constants;

/*******************************************************************************
* FUNCTION NAME: display_data
* PURPOSE:       Displays data on OI user display when in user mode
* ARGUMENTS:     print_data - data to print
*                type
* RETURNS:       none
*******************************************************************************/
void display_oi_data(UINT8 print_data, DisplayDataType type)
{
  if ((Oi_calibrate > OI_CALIBRATE_POTS) &&
      (Oi_calibrate < OI_CALIBRATE_JOYSTICKS))
  {
    /* not in pot or joystick calibration mode */
    if ((competition_mode == ROBOT_ENABLED) && (autonomous_mode == 0))
    {
      /* robot is enabled  and autonomous is disabled */
      if (type == DISPLAY_DATA_PSI)
      {
        /* display the psi data */
        User_Mode_byte = print_data;
      }
    }
    else
    {
      if (type == DISPLAY_DATA_AUTO)
      {
        /* display autonomous data */
        User_Mode_byte = print_data;
      }
    }
  }
  else
  {
    User_Mode_byte = print_data;
  }

  return;
}



/*******************************************************************************
* FUNCTION NAME: test_pots
* PURPOSE:       Range-checks the pots for illegal values signifying a failure
* CALLED FROM:   Process_Data_From_Master_uP()
* ARGUMENTS:     right_pot - value of right pot
*                left_pot  - value of left pot
* RETURNS:       POTS_GOOD if pots are good, POTS_BAD if pots are bad
*******************************************************************************/
int test_pots(unsigned char crab_pot)
{
  if ((crab_pot == 0) || (crab_pot >= 254))
  {
    return (POTS_BAD);
  }

  return (POTS_GOOD);
}



/*******************************************************************************
* FUNCTION NAME: pump_control
* PURPOSE:       turn pump on & off depending on pressure
* ARGUMENTS:     motor_vals - motor speeds
* RETURNS:       none
*******************************************************************************/
void pump_control(MotorValsType *motor_vals)
{
#if 0
  /* real robot has analog pressure sensor */
  UINT8 sensor_val;

  sensor_val = GET_ANALOG_VALUE_SHIFT(Analog_in_pressure_sensor);

  if (sensor_val >= PUMP_HI_VAL)
  {
    motor_vals->pump = PUMP_OFF;
  }
  else if (sensor_val < PUMP_LO_VAL)
  {
    motor_vals->pump = PUMP_ON;
  }
#else

  static int pkts_after_sensor = 0;

  /* proto doesn't have analog pressure sensor */
  /*
  if (Dig_in_pressure == 0)
  {
    motor_vals->pump = PUMP_ON;
  }
  else
  {
    motor_vals->pump = PUMP_OFF;
  }
  */
  if (Dig_in_pressure == 0)
  {
    /* pressure sensor reads less than ~120 */
    /* re-initialize counter */
    pkts_after_sensor = 0;
  }
  else if ((motor_vals->pump == PUMP_ON) &&
           (competition_mode == ROBOT_ENABLED))
  {
    pkts_after_sensor++;
  }
  /* else don't increment */

  if (pkts_after_sensor <= 300) /* run for 7.5 more seconds */
  {
    motor_vals->pump = PUMP_ON;
  }
  else
  {
    motor_vals->pump = PUMP_OFF;
  }

  /*
  printf("sw %d pkt %d pump %d\n", (int)Dig_in_pressure,
         (int)pkts_after_sensor, (int)motor_vals->pump);
  */
#endif

  return;
}



/*******************************************************************************
* FUNCTION NAME: big_arm_control
* PURPOSE:       control the shoulder, elbow, accumulator, and brake for
*                big arm
* ARGUMENTS:     motor_vals - motor speed structure
* RETURNS:       none
*******************************************************************************/
void big_arm_control(MotorValsType *motor_vals,
                     CalibrationValsType *calibration_vals)
{
#if 0
  static int oi_sw_accumulator_fwd_prev = 0;
  static int oi_sw_accumulator_rev_prev = 0;
#endif
  int tmp;
  int dist_from_deadzone_boundry;

  /*
   * arm code assumes that high position of arm is large pot value and
   * low position is small pot value
   */

  if (Oi_sw_accumulator_fwd == 1)
  {
    motor_vals->accumulator = ACCUMULATOR_FWD;
  }
  else if (Oi_sw_accumulator_rev == 1)
  {
    motor_vals->accumulator = ACCUMULATOR_REV;
  }
  else
  {
    motor_vals->accumulator = ACCUMULATOR_STOP;
  }

#if 0
  /**** accumulator ****/
  if ((Oi_sw_accumulator_fwd == 1) && (oi_sw_accumulator_fwd_prev == 0))
  {
    /* accumulator forward button was tapped */
    if (motor_vals->accumulator == ACCUMULATOR_FWD)
    {
      /* if accumulator is running forward make it stop */
      motor_vals->accumulator = ACCUMULATOR_STOP;
    }
    else
    {
      /* else run accumulator forward */
      motor_vals->accumulator = ACCUMULATOR_FWD;
    }
  }

  if ((Oi_sw_accumulator_rev == 1) && (oi_sw_accumulator_rev_prev == 0))
  {
    /* accumulator reverse button was tapped */
    if (motor_vals->accumulator == ACCUMULATOR_REV)
    {
      /* if accumulator is running reverse make it stop */
      motor_vals->accumulator = ACCUMULATOR_STOP;
    }
    else
    {
      /* else run accumulator reverse */
      motor_vals->accumulator = ACCUMULATOR_REV;
    }
  }

  oi_sw_accumulator_fwd_prev = Oi_sw_accumulator_fwd;
  oi_sw_accumulator_rev_prev = Oi_sw_accumulator_rev;
#endif


  /**** shoulder ****/
  if (HAT_RANGE_CHECK(Oi_big_arm_auto, BIG_ARM_AUTO_MOVE_HAT))
  {
    /* auto mode - lock in position to hang */
    if (Get_Analog_Value(Analog_in_big_arm_shoulder_pot) < BIG_ARM_HANG_TGT_10)
    {
      motor_vals->big_arm_shoulder = BIG_ARM_AUTO_MOVE_SPEED;
      motor_vals->arm_brake = BIG_ARM_BRAKE_RELEASE;
    }
    else
    {
      motor_vals->big_arm_shoulder = 0;
      motor_vals->arm_brake = BIG_ARM_BRAKE_ENGAGE;
    }

    /*printf("pos %d tgt %d spd %d\n",
           (int)Get_Analog_Value(Analog_in_big_arm_shoulder_pot),
           (int)BIG_ARM_HANG_TGT_10, (int)motor_vals->big_arm_shoulder);*/
  }
  else
  {
    /* non-auto mode */
    /* disengage brake if stick is past brake deadzone */
    if ((Oi_big_arm_shoulder >= (127 - BIG_ARM_SHOULDER_STICK_DEADZONE_BRAKE)) &&
        (Oi_big_arm_shoulder <= (127 + BIG_ARM_SHOULDER_STICK_DEADZONE_BRAKE)))
    {
      motor_vals->arm_brake = BIG_ARM_BRAKE_ENGAGE;
    }
    else
    {
      motor_vals->arm_brake = BIG_ARM_BRAKE_RELEASE;
    }

    /* set deadzone for shoulder movement */
    if ((Oi_big_arm_shoulder >= (127 - BIG_ARM_SHOULDER_STICK_DEADZONE_MOVE)) &&
        (Oi_big_arm_shoulder <= (127 + BIG_ARM_SHOULDER_STICK_DEADZONE_MOVE)))
    {
      motor_vals->big_arm_shoulder = 0;
    }
    else
    {
      if (Oi_big_arm_shoulder > 127)
      {
        /* should result in a positive number */
        dist_from_deadzone_boundry = (int)Oi_big_arm_shoulder - 127 -
                                     (int)BIG_ARM_SHOULDER_STICK_DEADZONE_MOVE;
      }
      else if (Oi_big_arm_shoulder < 127)
      {
        /* should result in a negative number */
        dist_from_deadzone_boundry = (int)Oi_big_arm_shoulder - 127 +
                                     (int)BIG_ARM_SHOULDER_STICK_DEADZONE_MOVE;
      }

      tmp = ((127 * dist_from_deadzone_boundry) /
             (127 - BIG_ARM_SHOULDER_STICK_DEADZONE_MOVE));
      motor_vals->big_arm_shoulder = tmp;
    }
  }

  /* scale shoulder motor down so it can't run full speed */
#if 0
  /* numerator & denominator are both 1, so skip code */
  tmp = ((((int)motor_vals->big_arm_shoulder - 127) *
          BIG_ARM_SHOULDER_SCALE_NUMERATOR) /
         BIG_ARM_SHOULDER_SCALE_DENOMINATOR) + 127;
  motor_vals->big_arm_shoulder = (UINT8)MAX_RETURN(tmp, 127);
#endif

  /*
  printf("BRAKE = %d\n", (int)motor_vals->arm_brake);
  */

#if 0
  if (motor_vals->big_arm_shoulder > 0)
  {
    /* arm is moving up */
    if ((GET_ANALOG_VALUE_SHIFT(Analog_in_big_arm_shoulder_pot)) >=
         calibration_vals->big_arm_shoulder_hi)
    {
      /* arm is at its upper limit so don't move it up any further */
      motor_vals->big_arm_shoulder = 0;
    }
    else if ((GET_ANALOG_VALUE_SHIFT(Analog_in_big_arm_shoulder_pot)) >=
             (calibration_vals->big_arm_shoulder_hi -
              BIG_ARM_SHOULDER_SLOW_ZONE))
    {
      /* arm is near its upper limit so slow its speed */
      /* H = upper hard stop
       * S = upper slow down point
       * speed = speed * (H - arm_position) / (H - S)
       * then set minimum speed */
      motor_vals->big_arm_shoulder =
        (((int)motor_vals->big_arm_shoulder *
          (calibration_vals->big_arm_shoulder_hi -
           GET_ANALOG_VALUE_SHIFT(Analog_in_big_arm_shoulder_pot))) /
         BIG_ARM_SHOULDER_SLOW_ZONE);
      MIN(motor_vals->big_arm_shoulder, BIG_ARM_SHOULDER_MIN_SPEED);
    }
  }

  if (motor_vals->big_arm_shoulder < 0)
  {
    /* arm is moving down */
    if ((GET_ANALOG_VALUE_SHIFT(Analog_in_big_arm_shoulder_pot)) <=
         calibration_vals->big_arm_shoulder_lo)
    {
      /* arm is at its lower limit so don't move it down any further */
      motor_vals->big_arm_shoulder = 0;
    }
    else if ((GET_ANALOG_VALUE_SHIFT(Analog_in_big_arm_shoulder_pot)) <=
             (calibration_vals->big_arm_shoulder_lo +
              BIG_ARM_SHOULDER_SLOW_ZONE))
    {
      /* arm is near its lower limit so slow its speed */
      /* H = lower hard stop
       * S = lower slow down point
       * speed = speed * (arm_position - H) / (S - H)
       * then set minimum speed
       */
      motor_vals->big_arm_shoulder =
        (((int)motor_vals->big_arm_shoulder *
          (GET_ANALOG_VALUE_SHIFT(Analog_in_big_arm_shoulder_pot) -
           calibration_vals->big_arm_shoulder_lo)) /
         BIG_ARM_SHOULDER_SLOW_ZONE);
      MIN(motor_vals->big_arm_shoulder, BIG_ARM_SHOULDER_MIN_SPEED);
    }
  }
#endif


  /**** elbow ****/
  /* set deadzone for the elbow */
  if ((Oi_big_arm_elbow >= (127 - BIG_ARM_ELBOW_STICK_DEADZONE)) &&
      (Oi_big_arm_elbow <= (127 + BIG_ARM_ELBOW_STICK_DEADZONE)))
  {
    /* make the elbow not move if joystick is within deadzone */
    motor_vals->big_arm_elbow = 0;
  }
  else
  {
    /* set motor speed equal to joystick speed */
    motor_vals->big_arm_elbow = Oi_big_arm_elbow - 127;
  }

#if 0
  /* if the elbow is greater than its high limit, don't move up */
  if ((GET_ANALOG_VALUE_SHIFT(Analog_in_big_arm_elbow_pot) >=
       calibration_vals->big_arm_elbow_hi) &&
      (motor_vals->big_arm_elbow > 0))
  {
    motor_vals->big_arm_elbow = 0;
  }

  /* if the elbow is less than its low limit, don't move down */
  if ((GET_ANALOG_VALUE_SHIFT(Analog_in_big_arm_elbow_pot) <=
       calibration_vals->big_arm_elbow_lo) &&
      (motor_vals->big_arm_elbow < 0))
  {
    motor_vals->big_arm_elbow = 0;
  }
#endif

  return;
}



/*******************************************************************************
* FUNCTION NAME: small_arm_control
* PURPOSE:       control the elbow and squeezer of small arm
* ARGUMENTS:     motor_vals - motor speed structure
* RETURNS:       none
*******************************************************************************/
void small_arm_control(MotorValsType *motor_vals,
                       CalibrationValsType *calibration_vals)
{
  static int oi_sw_squeeze_prev = 0;
  UINT8 arm_tar_pot_vals;

  /* shoulder */
  if (HAT_RANGE_CHECK(Oi_small_arm_auto, SMALL_ARM_AUTO_MOVE_HAT))
  {
    /* auto mode - lock in position to pick big ball of small goal */
    arm_tar_pot_vals = arm_scaling(SMALL_ARM_GRAB_BB_POS,
                                   calibration_vals->small_arm_shoulder_lo,
                                   calibration_vals->small_arm_shoulder_hi);
    motor_vals->small_arm_shoulder = general_pos_feedback(
          GET_ANALOG_VALUE_SHIFT(Analog_in_small_arm_shoulder_pot),
          arm_tar_pot_vals, &g_small_arm_feedback_constants);
  }
  else
  {
    /* non-auto mode */
    /* set deadzone for shoulder movement */
    if ((Oi_small_arm_shoulder >= (127 - SMALL_ARM_SHOULDER_STICK_DEADZONE)) &&
        (Oi_small_arm_shoulder <= (127 + SMALL_ARM_SHOULDER_STICK_DEADZONE)))
    {
      motor_vals->small_arm_shoulder = 0;
    }
    else
    {
      /* set shoulder speed equal to joystick */
      motor_vals->small_arm_shoulder = Oi_small_arm_shoulder - 127;
    }
  }

#if 0
  /* if the shoulder is greater than its high limit, don't move up */
  if ((GET_ANALOG_VALUE_SHIFT(Analog_in_small_arm_shoulder_pot) >=
       calibration_vals->small_arm_shoulder_hi) &&
      (motor_vals->small_arm_shoulder > 0))
  {
    motor_vals->small_arm_shoulder = 0;
  }

  /* if the shoulder is less than its low limit, don't move down */
  if ((GET_ANALOG_VALUE_SHIFT(Analog_in_small_arm_shoulder_pot) <=
       calibration_vals->small_arm_shoulder_lo) &&
      (motor_vals->small_arm_shoulder < 0))
  {
    motor_vals->small_arm_shoulder = 0;
  }
#endif

  if ((Oi_sw_squeeze == 1) && (oi_sw_squeeze_prev == 0))
  {
    if (motor_vals->small_arm_squeeze == SQUEEZE_IN)
    {
      motor_vals->small_arm_squeeze = SQUEEZE_OUT;
    }
    else
    {
      motor_vals->small_arm_squeeze = SQUEEZE_IN;
    }
  }

  oi_sw_squeeze_prev = Oi_sw_squeeze;

  return;
}



/*******************************************************************************
* FUNCTION NAME: winch_control
* PURPOSE:       control the winch
* ARGUMENTS:     motor_vals - motor speed structure
* RETURNS:       none
*******************************************************************************/
#define WINCH_RUNNING     1
#define WINCH_NOT_RUNNING 2
#define NUM_PACKETS_TO_STOP_WINCH 0
void winch_control(MotorValsType *motor_vals)
{
  /*
  printf("hat %3d ", (int)Oi_winch);
  */
  if (HAT_RANGE_CHECK(Oi_winch, WINCH_UP_HAT) == TRUE)
  {
    /* winch cable in */
    motor_vals->winch = 127;
    motor_vals->winch_brake = WINCH_BRAKE_RELEASE;
  }
  else if (HAT_RANGE_CHECK(Oi_winch, WINCH_DOWN_HAT) == TRUE)
  {
    /* let cable out */
    motor_vals->winch = -20;
    motor_vals->winch_brake = WINCH_BRAKE_RELEASE;
  }
  else
  {
    motor_vals->winch = 0;
    motor_vals->winch_brake = WINCH_BRAKE_ENGAGE;
  }

  /*
  printf("WINCH %d, WINCH_BRAKE %d\n", (int)motor_vals->winch,
          (int)motor_vals->winch_brake);
  */
  return;
}

/*******************************************************************************
* FNCTION NAME: bumper_control
* PURPOSE:       control the bumpers
* ARGUMENTS:     motor_vals - motor speed structure
* RETURNS:       none
*******************************************************************************/
void bumper_control(MotorValsType *motor_vals)
{
  static int oi_sw_bumper_front_prev = 0;
  static int oi_sw_bumper_back_prev = 0;

#ifdef AUTOMATIC_BUMPERS
  static AutoManuType bumper_mode = MANUAL;

  if (Oi_sw_bumper_auto_mode == 1)
  {
    bumper_mode = AUTO;
  }
  else if ((Oi_sw_bumper_front == 1) || (Oi_sw_bumper_back == 1))
  {
    bumper_mode = MANUAL;
  }

  if (bumper_mode == MANUAL)
  {
#endif
    if ((Oi_sw_bumper_front == 1) && (oi_sw_bumper_front_prev == 0))
    {
      /* front bumper button was tapped */
      /* toggle front bumper state */
      if (motor_vals->front_bumper == BUMPER_IN)
      {
        motor_vals->front_bumper = BUMPER_OUT;
        /* 0 is a special number to designate that the bumper should be
           physically out by now */
        packet_num_f_bumper_out = (rxdata.packet_num == 0 ? 255 :
                                                            rxdata.packet_num);
      }
      else
      {
        motor_vals->front_bumper = BUMPER_IN;
      }
    }

    if ((Oi_sw_bumper_back == 1) && (oi_sw_bumper_back_prev == 0))
    {
      /* back bumper button was tapped */
      /* toggle back bumper state */
      if (motor_vals->back_bumper == BUMPER_IN)
      {
        motor_vals->back_bumper = BUMPER_OUT;
        /* 0 is a special number to designate that the bumper should be
           physically out by now */
        packet_num_b_bumper_out = (rxdata.packet_num == 0 ? 255 :
                                                            rxdata.packet_num);
      }
      else
      {
        motor_vals->back_bumper = BUMPER_IN;
      }
    }
#ifdef AUTOMATIC_BUMPERS
  }
  else
  {
    /* automatic bumpers */
    if (motor_vals->suspension_front == SUSPENSION_UP)
    {
      motor_vals->back_bumper = BUMPER_IN;
    }
    else
    {
      motor_vals->back_bumper = BUMPER_OUT;
    }

    if (motor_vals->suspension_back == SUSPENSION_UP)
    {
      motor_vals->front_bumper = BUMPER_IN;
    }
    else
    {
      motor_vals->front_bumper = BUMPER_OUT;
    }
  }
#else

  oi_sw_bumper_front_prev = Oi_sw_bumper_front;
  oi_sw_bumper_back_prev = Oi_sw_bumper_back;

  return;
}



/*******************************************************************************
* FUNCTION NAME: suspension_control
* PURPOSE:       automatically raise & lower suspension to climb platform
* ARGUMENTS:     motor_vals - suspension relays
* RETURNS:       none
*******************************************************************************/
void suspension_control(MotorValsType *motor_vals)
{
  /* start out on the ground with both wheels up */
  static int state = FRONT_UP_BACK_UP_FLOOR;
  static int oi_sw_ascend_prev = 0;
  static int oi_sw_manu_susp_front_prev;
  static int oi_sw_manu_susp_back_prev;
  static AutoManuType auto_manu_state = AUTO;
  static LevelType level = FLOOR;
  static UINT8 oi_platform_selector_prev = 127;

  int dig_in_plat_fl_wheel_f_local, dig_in_plat_fr_wheel_f_local;
  int dig_in_plat_fl_wheel_b_local, dig_in_plat_fr_wheel_b_local;
  int dig_in_plat_bl_wheel_f_local, dig_in_plat_br_wheel_f_local;
  int dig_in_plat_bl_wheel_b_local, dig_in_plat_br_wheel_b_local;

  if ((Oi_sw_manu_susp_front == 1) || (Oi_sw_manu_susp_back == 1))
  {
    auto_manu_state = MANUAL;
  }

  if ((Oi_sw_ascend == 1) && (auto_manu_state == MANUAL))
  {
    auto_manu_state = AUTO;
    state = FRONT_UP_BACK_UP_FLOOR;
    oi_sw_ascend_prev = 1;
  }


  if ((HAT_RANGE_CHECK(Oi_platform_selector, PLATFORM_SEL_DOWN_HAT)) &&
      !(HAT_RANGE_CHECK(oi_platform_selector_prev, PLATFORM_SEL_DOWN_HAT)))
  {
    level = MIN_RETURN(level - 1, FLOOR);
  }
  else if ((HAT_RANGE_CHECK(Oi_platform_selector, PLATFORM_SEL_UP_HAT)) &&
           !(HAT_RANGE_CHECK(oi_platform_selector_prev, PLATFORM_SEL_UP_HAT)))
  {
    level = MAX_RETURN(level + 1, PLATFORM_2);
  }

  oi_platform_selector_prev = Oi_platform_selector;


  if (auto_manu_state == MANUAL)
  {
    /* Manual control */
    if ((Oi_sw_manu_susp_front == 1) && (oi_sw_manu_susp_front_prev == 0))
    {
      if (motor_vals->suspension_front == SUSPENSION_DOWN)
      {
        motor_vals->suspension_front = SUSPENSION_UP;
      }
      else
      {
        motor_vals->suspension_front = SUSPENSION_DOWN;
      }
    }

    if ((Oi_sw_manu_susp_back == 1) && (oi_sw_manu_susp_back_prev == 0))
    {
      if (motor_vals->suspension_back == SUSPENSION_DOWN)
      {
        motor_vals->suspension_back = SUSPENSION_UP;
      }
      else
      {
        motor_vals->suspension_back = SUSPENSION_DOWN;
      }
    }
  }
  else
  {
#if 1
    if ((motor_vals->front_bumper == BUMPER_IN) ||
        ((motor_vals->front_bumper == BUMPER_OUT) &&
         (packet_num_f_bumper_out != 0) &&
         ((rxdata.packet_num - packet_num_f_bumper_out) <
          NUM_PACKETS_TO_EXTEND_BUMPERS)))
    {
      /* ignore sensor reading if bumper is in or if bumper was recently
         extended */
      dig_in_plat_fl_wheel_f_local = NOT_SEE_PLATFORM;
      dig_in_plat_fr_wheel_f_local = NOT_SEE_PLATFORM;
    }
    else
    {
      /* must set local copy to sensor reading if bumper is out */
      dig_in_plat_fl_wheel_f_local = Dig_in_plat_fl_wheel_f;
      dig_in_plat_fr_wheel_f_local = Dig_in_plat_fr_wheel_f;
      packet_num_f_bumper_out = 0;
    }

    if ((motor_vals->back_bumper == BUMPER_IN) ||
        ((motor_vals->back_bumper == BUMPER_OUT) &&
         (packet_num_b_bumper_out != 0) &&
         ((rxdata.packet_num - packet_num_b_bumper_out) <
          NUM_PACKETS_TO_EXTEND_BUMPERS)))
    {
      /* ignore sensor reading if bumper is in or if bumper was recently
         extended */
      dig_in_plat_bl_wheel_b_local = NOT_SEE_PLATFORM;
      dig_in_plat_br_wheel_b_local = NOT_SEE_PLATFORM;
    }
    else
    {
      /* must set local copy to sensor reading if bumper is out */
      dig_in_plat_bl_wheel_b_local = Dig_in_plat_bl_wheel_b;
      dig_in_plat_br_wheel_b_local = Dig_in_plat_br_wheel_b;
      packet_num_b_bumper_out = 0;
    }


    if ((state == FRONT_DN_BACK_DN) &&
        (packet_num_susp_down != 0) &&
        ((rxdata.packet_num - packet_num_susp_down) <
         NUM_PACKETS_TO_RAISE_ROBOT))
    {
      /* ignore sensor readings if suspension was just moved to down-down
         state because it takes some time for the robot to actually raise
         itself off the ground*/
      dig_in_plat_fl_wheel_f_local = NOT_SEE_PLATFORM;
      dig_in_plat_fr_wheel_f_local = NOT_SEE_PLATFORM;
      dig_in_plat_fl_wheel_b_local = NOT_SEE_PLATFORM;
      dig_in_plat_fr_wheel_b_local = NOT_SEE_PLATFORM;
      dig_in_plat_bl_wheel_f_local = NOT_SEE_PLATFORM;
      dig_in_plat_br_wheel_f_local = NOT_SEE_PLATFORM;
      dig_in_plat_bl_wheel_b_local = NOT_SEE_PLATFORM;
      dig_in_plat_br_wheel_b_local = NOT_SEE_PLATFORM;
    }
    else
    {
      /* set local copy to sensor reading */
      dig_in_plat_fl_wheel_f_local = Dig_in_plat_fl_wheel_f;
      dig_in_plat_fr_wheel_f_local = Dig_in_plat_fr_wheel_f;
      dig_in_plat_fl_wheel_b_local = Dig_in_plat_fl_wheel_b;
      dig_in_plat_fr_wheel_b_local = Dig_in_plat_fr_wheel_b;
      dig_in_plat_bl_wheel_f_local = Dig_in_plat_bl_wheel_f;
      dig_in_plat_br_wheel_f_local = Dig_in_plat_br_wheel_f;
      dig_in_plat_bl_wheel_b_local = Dig_in_plat_bl_wheel_b;
      dig_in_plat_br_wheel_b_local = Dig_in_plat_br_wheel_b;
      packet_num_susp_down = 0;
    }


    /* change states */
    if (state == FRONT_UP_BACK_UP_FLOOR)
    {
#if 0
      int num_sensors = 0;

      num_sensors += (Dig_in_plat_fl_wheel_b == SEE_PLATFORM) ? 1 : 0;
      num_sensors += (Dig_in_plat_fr_wheel_b == SEE_PLATFORM) ? 1 : 0;
      num_sensors += (Dig_in_plat_bl_wheel_f == SEE_PLATFORM) ? 1 : 0;
      num_sensors += (Dig_in_plat_br_wheel_f == SEE_PLATFORM) ? 1 : 0;

      if (num_sensors >= 3)
      {
        /* 3 of the 4 inner sensors see the platform so we must be on it */
        state = FRONT_UP_BACK_UP_PLATFORM;
      }
      else if ((Oi_sw_ascend == 1) && (oi_sw_ascend_prev == 0))
#endif
      if ((Oi_sw_ascend == 1) && (oi_sw_ascend_prev == 0))
      {
        /* tap button */
        state = FRONT_DN_BACK_DN;
        packet_num_susp_down = (rxdata.packet_num == 0 ? 255 :
                                                         rxdata.packet_num);
      }
    }
    else if (state == FRONT_DN_BACK_DN)
    {
      if ((Oi_sw_ascend == 1) && (oi_sw_ascend_prev == 0))
      {
        /* tapped button */
        if (level == FLOOR)
        {
          /* on floor so transition to floor state */
          state = FRONT_UP_BACK_UP_FLOOR;
        }
        else
        {
          /* on platform so transition to platform state */
          state = FRONT_UP_BACK_UP_PLATFORM;
        }
      }
      else if ((dig_in_plat_fl_wheel_f_local == SEE_PLATFORM) ||
               (dig_in_plat_fr_wheel_f_local == SEE_PLATFORM))
      {
        /* either of the front sensors on the front wheel sense the platform */
        state = FRONT_UP_BACK_DN;
      }
      else if ((dig_in_plat_bl_wheel_b_local == SEE_PLATFORM) ||
               (dig_in_plat_br_wheel_b_local == SEE_PLATFORM))
      {
        /* either of the back sensors on the back wheel sense the platform */
        state = FRONT_DN_BACK_UP;
      }
    }
    else if (state == FRONT_UP_BACK_DN)
    {
      if ((dig_in_plat_fl_wheel_f_local == NOT_SEE_PLATFORM) &&
          (dig_in_plat_fr_wheel_f_local == NOT_SEE_PLATFORM))
      {
        /* neither of the front sensors on the front wheel sense the platform */
        state = FRONT_DN_BACK_DN;
        packet_num_susp_down = 0;
      }
      else if ((dig_in_plat_fl_wheel_b_local == SEE_PLATFORM) &&
               (dig_in_plat_fr_wheel_b_local == SEE_PLATFORM) &&
               ((dig_in_plat_bl_wheel_f_local == SEE_PLATFORM) ||
                (dig_in_plat_br_wheel_f_local == SEE_PLATFORM)))
      {
        /* both the back sensors on the front wheel and either of the front
           sensors on the back wheel sense the platform */
        state = FRONT_UP_BACK_UP_PLATFORM;
        level++;
      }
      else if ((Oi_sw_ascend == 1) && (oi_sw_ascend_prev == 0))
      {
        /* tap button */
        state = FRONT_UP_BACK_UP_FLOOR;
      }
    }
    else if (state == FRONT_DN_BACK_UP)
    {
      if ((dig_in_plat_bl_wheel_f_local == SEE_PLATFORM) &&
          (dig_in_plat_br_wheel_f_local == SEE_PLATFORM) &&
          ((dig_in_plat_fl_wheel_b_local == SEE_PLATFORM) ||
           (dig_in_plat_fr_wheel_b_local == SEE_PLATFORM)))
      {
        /* both the front sensors on the back wheel and either of the back
           sensors on the front wheels sense the platform */
        state = FRONT_UP_BACK_UP_PLATFORM;
        level++;
      }
      else if ((dig_in_plat_bl_wheel_b_local == NOT_SEE_PLATFORM) &&
               (dig_in_plat_br_wheel_b_local == NOT_SEE_PLATFORM))
      {
        /* neither of the back sensors on the back wheels sense the platform */
        state = FRONT_DN_BACK_DN;
        packet_num_susp_down = 0;
      }
      else if ((Oi_sw_ascend == 1) && (oi_sw_ascend_prev == 0))
      {
        /* tap button */
        state = FRONT_UP_BACK_UP_FLOOR;
      }
    }
    else if (state == FRONT_UP_BACK_UP_PLATFORM)
    {
      if ((dig_in_plat_fl_wheel_b_local == NOT_SEE_PLATFORM) &&
          (dig_in_plat_fr_wheel_b_local == NOT_SEE_PLATFORM) &&
          (dig_in_plat_fl_wheel_f_local == NOT_SEE_PLATFORM) &&
          (dig_in_plat_fr_wheel_f_local == NOT_SEE_PLATFORM))
      {
        /* none of the sensors on the front wheels sense the platform */
        state = FRONT_DN_BACK_UP;
        level--;
      }
      else if ((dig_in_plat_bl_wheel_f_local == NOT_SEE_PLATFORM) &&
               (dig_in_plat_br_wheel_f_local == NOT_SEE_PLATFORM) &&
               (dig_in_plat_bl_wheel_b_local == NOT_SEE_PLATFORM) &&
               (dig_in_plat_br_wheel_b_local == NOT_SEE_PLATFORM))
      {
        /* none of the sensors on the back wheels sense the platform */
        state = FRONT_UP_BACK_DN;
        level--;
      }
      else if ((Oi_sw_ascend == 1) && (oi_sw_ascend_prev == 0))
      {
        /* tap button */
        state = FRONT_DN_BACK_DN;
        packet_num_susp_down = 0;
        packet_num_susp_down = (rxdata.packet_num == 0 ? 255 :
                                                         rxdata.packet_num);
      }
    }
#endif

    oi_sw_ascend_prev = Oi_sw_ascend;

    /*
    printf("state %d level %d\n", (int)state, (int)level);
    */

    /* perform actions */
    if ((state == FRONT_UP_BACK_UP_FLOOR) ||
        (state == FRONT_UP_BACK_UP_PLATFORM))
    {
      motor_vals->suspension_front = SUSPENSION_UP;
      motor_vals->suspension_back = SUSPENSION_UP;
    }
    else if (state == FRONT_DN_BACK_DN)
    {
      motor_vals->suspension_front = SUSPENSION_DOWN;
      motor_vals->suspension_back = SUSPENSION_DOWN;
    }
    else if (state == FRONT_UP_BACK_DN)
    {
      motor_vals->suspension_front = SUSPENSION_UP;
      motor_vals->suspension_back = SUSPENSION_DOWN;
    }
    else if (state == FRONT_DN_BACK_UP)
    {
      motor_vals->suspension_front = SUSPENSION_DOWN;
      motor_vals->suspension_back = SUSPENSION_UP;
    }
  }

  oi_sw_manu_susp_front_prev = Oi_sw_manu_susp_front;
  oi_sw_manu_susp_back_prev = Oi_sw_manu_susp_back;

  return;
}


