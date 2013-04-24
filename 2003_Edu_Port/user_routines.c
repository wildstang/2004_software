/*******************************************************************************
* FILE NAME: user_routines.c <EDU VERSION>
*
* DESCRIPTION:
*  This file contains the default mappings of inputs  
*  (like switches, joysticks, and buttons) to outputs on the EDU RC.  
*
* USAGE:
*  You can either modify this file to fit your needs, or remove it from your 
*  project and replace it with a modified copy. 
*
*******************************************************************************/

#include "ifi_aliases.h"
#include "ifi_default.h"
#include "ifi_utilities.h"
#include "serial_utils.h"
#include "user_routines.h"
#include "printf_lib.h"
#include "delays.h"       /*defined locally*/
#include "oi_data.h" /* For OI Data from slave RC */
#include "ws_code.h"

/*** DEFINE USER VARIABLES AND INITIALIZE THEM HERE ***/
/* EXAMPLES: (see MPLAB C18 User's Guide, p.9 for all types)
unsigned char wheel_revolutions = 0; (can vary from 0 to 255)
unsigned int  delay_count = 7;       (can vary from 0 to 65,535)
int           angle_deviation = 142; (can vary from -32,768 to 32,767)
unsigned long very_big_counter = 0;  (can vary from 0 to 4,294,967,295)
*/


/*******************************************************************************
* FUNCTION NAME: Limit_Switch_Max
* PURPOSE:       Sets a PWM value to neutral (127) if it exceeds 127 and the
*                limit switch is on.
* CALLED FROM:   this file
* ARGUMENTS:     
*     Argument       Type             IO   Description
*     --------       -------------    --   -----------
*     switch_state   unsigned char    I    limit switch state
*     *input_value   pointer           O   points to PWM byte value to be limited
* RETURNS:       void
*******************************************************************************/
void Limit_Switch_Max(unsigned char switch_state, unsigned char *input_value)
{
  if (switch_state == CLOSED)
  { 
    if(*input_value > 127)
      *input_value = 127;
  }
}


/*******************************************************************************
* FUNCTION NAME: Limit_Switch_Min
* PURPOSE:       Sets a PWM value to neutral (127) if it's less than 127 and the
*                limit switch is on.
* CALLED FROM:   this file
* ARGUMENTS:     
*     Argument       Type             IO   Description
*     --------       -------------    --   -----------
*     switch_state   unsigned char    I    limit switch state
*     *input_value   pointer           O   points to PWM byte value to be limited
* RETURNS:       void
*******************************************************************************/
void Limit_Switch_Min(unsigned char switch_state, unsigned char *input_value)
{
  if (switch_state == CLOSED)
  { 
    if(*input_value < 127)
      *input_value = 127;
  }
}


/*******************************************************************************
* FUNCTION NAME: Limit_Mix
* PURPOSE:       Limits the mixed value for one joystick drive.
* CALLED FROM:   Default_Routine, this file
* ARGUMENTS:     
*     Argument             Type    IO   Description
*     --------             ----    --   -----------
*     intermediate_value    int    I    
* RETURNS:       unsigned char
*******************************************************************************/
unsigned char Limit_Mix (int intermediate_value)
{
  static int limited_value;
  
  if (intermediate_value < 2000)
  {
    limited_value = 2000;
  }
  else if (intermediate_value > 2254)
  {
    limited_value = 2254;
  }
  else
  {
    limited_value = intermediate_value;
  }
  return (unsigned char) (limited_value - 2000);
}


/*******************************************************************************
* FUNCTION NAME: Setup_Who_Controls_Pwms
* PURPOSE:       Each parameter specifies what processor will control the pwm.  
*                 
* CALLED FROM:   User_Initialization
*     Argument             Type    IO   Description
*     --------             ----    --   -----------
*     pwmSpec1              int     I   USER/MASTER (defined in ifi_aliases.h)
*     pwmSpec2              int     I   USER/MASTER
*     pwmSpec3              int     I   USER/MASTER
*     pwmSpec4              int     I   USER/MASTER
*     pwmSpec5              int     I   USER/MASTER
*     pwmSpec6              int     I   USER/MASTER
*     pwmSpec7              int     I   USER/MASTER
*     pwmSpec8              int     I   USER/MASTER
* RETURNS:       void
*******************************************************************************/
static void Setup_Who_Controls_Pwms(int pwmSpec1,int pwmSpec2,int pwmSpec3,int pwmSpec4,
                                    int pwmSpec5,int pwmSpec6,int pwmSpec7,int pwmSpec8)
{
  txdata.pwm_mask = 0xFF;         /* Default to master controlling all PWMs. */
  if (pwmSpec1 == USER)           /* If User controls PWM1 then clear bit0. */
    txdata.pwm_mask &= 0xFE;      /* same as txdata.pwm_mask = txdata.pwm_mask & 0xFE; */
  if (pwmSpec2 == USER)           /* If User controls PWM2 then clear bit1. */
    txdata.pwm_mask &= 0xFD;
  if (pwmSpec3 == USER)           /* If User controls PWM3 then clear bit2. */
    txdata.pwm_mask &= 0xFB;
  if (pwmSpec4 == USER)           /* If User controls PWM4 then clear bit3. */
    txdata.pwm_mask &= 0xF7;
  if (pwmSpec5 == USER)           /* If User controls PWM5 then clear bit4. */
    txdata.pwm_mask &= 0xEF;
  if (pwmSpec6 == USER)           /* If User controls PWM6 then clear bit5. */
    txdata.pwm_mask &= 0xDF;
  if (pwmSpec7 == USER)           /* If User controls PWM7 then clear bit6. */
    txdata.pwm_mask &= 0xBF;
  if (pwmSpec8 == USER)           /* If User controls PWM8 then clear bit7. */
    txdata.pwm_mask &= 0x7F;
}


/*******************************************************************************
* FUNCTION NAME: User_Initialization
* PURPOSE:       This routine is called first in the Main loop.  
*                You may modify and add to this function.
*                The primary purpose is to set up the DIGITAL IN/OUT - ANALOG IN
*                pins as analog inputs, digital inputs, and digital outputs.
* CALLED FROM:   main.c
* ARGUMENTS:     none
* RETURNS:       void
*******************************************************************************/
void User_Initialization (void)
{
/* FIRST: Set up the pins you want to use as analog INPUTs. */
  IO1 = IO2 = IO3 = IO4 = INPUT;        /* Used for analog inputs. */
      /* 
       Note: IO1 = IO2 = IO3 = IO4 = INPUT 
             is the same as the following:
             IO1 = INPUT;
             IO2 = INPUT;
             IO3 = INPUT;
             IO4 = INPUT;
      */

/* SECOND: Configure the number of analog channels. */
  Set_Number_of_Analog_Channels(FOUR_ANALOG);     /* See ifi_aliases.h */

/* THIRD: Set up any extra digital inputs. */
      /* The six INTERRUPTS are already digital inputs. */
      /* If you need more then set them up here. */
      /* IOxx = IOyy = INPUT; */
  IO6 = IO8 = IO10 = INPUT;      /* Used for limit switch inputs. */
  IO12 = IO14 = IO16 = INPUT;    /* Used for limit switch inputs. */

/* FOURTH: Set up the pins you want to use as digital OUTPUTs. */
  IO5 = IO7 = IO9 = OUTPUT;     /* For connecting to adjacent limit switches. */
  IO11 = IO13 = IO15 = OUTPUT;  /* For connecting to adjacent limit switches. */

/* FIFTH: Initialize the values on the digital outputs. */
  rc_dig_out05 = rc_dig_out07 = rc_dig_out09 = 0;
  rc_dig_out11 = rc_dig_out13 = rc_dig_out15 = 0;

/* SIXTH: Set your initial PWM values.  Neutral is 127. */
  pwm01 = pwm02 = pwm03 = pwm04 = pwm05 = pwm06 = pwm07 = pwm08 = 127;

/* SEVENTH: Choose which processor will control which PWM outputs. */
  Setup_Who_Controls_Pwms(MASTER,MASTER,MASTER,MASTER,MASTER,MASTER,MASTER,MASTER);

  /* Add any other initialization code here. */

  InitializeSerialComms();

  Putdata(&txdata);             /* DO NOT CHANGE! */

  User_Proc_Is_Ready();         /* DO NOT CHANGE! - last line of User_Initialization */
}

/*******************************************************************************
* FUNCTION NAME: Process_Data_From_Master_uP
* PURPOSE:       Executes every 17ms when it gets new data from the master 
*                microprocessor.
* CALLED FROM:   main.c
* ARGUMENTS:     none
* RETURNS:       void
*******************************************************************************/
void Process_Data_From_Master_uP(void)
{
  oi_data OIData;
  MotorValsType motor_vals;
  CrabTargetsType crab_targets;
  unsigned char pots_good = TRUE;

  pwm01 = 127;
  pwm02 = 127;
  pwm03 = 127;
  pwm04 = 127;
  pwm05 = 127;
  pwm06 = 127;

  Getdata(&rxdata);   /* Get fresh data from the master microprocessor. */

  /* Only call the default routine if OIData was actually filled in */
  if (GetOIData(&OIData))
  {
    pots_good = test_pots(Get_Analog_Value(ANALOG_IN_RIGHT_POT),
                          Get_Analog_Value(ANALOG_IN_LEFT_POT));

    printf("\n");
    /*
    printf("driveX %d", (int)OIData.p4_wheel);
    printf(" driveY %d", (int)OIData.p4_aux);
    */
    printf(" crabX %d", (int)OIData.p4_x);
    printf(" crabY %d", (int)OIData.p4_y);
    /*
    printf("\n");
    */

    printf(" Lpot %d", (int)Get_Analog_Value(ANALOG_IN_LEFT_POT));
    /*
    printf(" Rpot %d", (int)Get_Analog_Value(ANALOG_IN_RIGHT_POT));
    printf(" in03 %d", (int)Get_Analog_Value(ANALOG_IN_03));
    printf(" in04 %d", (int)Get_Analog_Value(ANALOG_IN_04));
    */
    printf("\n");

    drive_stick_input(&OIData, &motor_vals);

    pots_good = TRUE;
    if (pots_good == TRUE) /* and if override button isn't pressed */
    {
      crab_stick_input(&OIData, &crab_targets);

      crab_system(&crab_targets, &motor_vals);
    }
    else
    {
      crab_manu_override(&OIData, &motor_vals);
    }

    pwm01 = motor_vals.lf_wheel;
    pwm02 = motor_vals.rf_wheel;
    pwm03 = motor_vals.lb_wheel;
    pwm04 = motor_vals.rb_wheel;
    pwm05 = motor_vals.left_crab;
    pwm06 = motor_vals.right_crab;
  }

  /*
  printf(" pwm1 %d", (int)pwm01);
  printf(" Lcrab speed %d", (int)pwm05);
  printf(" Rcrab speed %d", (int)pwm06);
  printf("\n");
  */

  Putdata(&txdata);             /* DO NOT CHANGE! */
}



