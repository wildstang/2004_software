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
#include "delays.h"       /*defined locally*/
#include "oi_data.h" /* For OI Data from slave RC */

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
  IO1 = IO2 = INPUT;        /* Used for analog inputs. */
      /* 
       Note: IO1 = IO2 = IO3 = IO4 = INPUT 
             is the same as the following:
    
             IO1 = INPUT;
             IO2 = INPUT;
             IO3 = INPUT;
             IO4 = INPUT;
      */

/* SECOND: Configure the number of analog channels. */
  Set_Number_of_Analog_Channels(TWO_ANALOG);     /* See ifi_aliases.h */

/* THIRD: Set up any extra digital inputs. */
      /* The six INTERRUPTS are already digital inputs. */
      /* If you need more then set them up here. */
      /* IOxx = IOyy = INPUT; */
  IO6 = IO8 = IO10 = INPUT;      /* Used for limit switch inputs. */
  IO12 = IO14 = IO16 = INPUT;    /* Used for limit switch inputs. */

/* FOURTH: Set up the pins you want to use as digital OUTPUTs. */
  IO3 = IO4 = OUTPUT;
  IO5 = IO7 = IO9 = OUTPUT;     /* For connecting to adjacent limit switches. */
  IO11 = IO13 = IO15 = OUTPUT;  /* For connecting to adjacent limit switches. */

/* FIFTH: Initialize the values on the digital outputs. */
  rc_dig_out03 = rc_dig_out04 = 0;
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
  int i;
  oi_data OIData;
        
  Getdata(&rxdata);   /* Get fresh data from the master microprocessor. */

  if(GetOIData(&OIData))
  {
      /* Only call the default routine if OIData was actually filled in */
      Default_Routine(&OIData);
  }
  
  Putdata(&txdata);             /* DO NOT CHANGE! */
}


/*******************************************************************************
* FUNCTION NAME: Default_Routine
* PURPOSE:       Performs the default mappings of inputs to outputs for the
*                Robot Controller.
* CALLED FROM:   this file, Process_Data_From_Master_uP routine
* ARGUMENTS:     none
* RETURNS:       void
*******************************************************************************/
void Default_Routine(oi_data *OIData)
{
 /*********** OIData Variables for use in this function ************************
     OIData->p1_x                   Joystick 1 X Axis
     OIData->p1_y                   Joystick 1 Y Axis
     OIData->p1_wheel               Joystick 1 Wheel
     OIData->p1_aux                 Joystick 1 Aux
     OIData->swA.p1_p3_sw.p12_trig  Joystick 1 Trigger
     OIData->swA.p1_p3_sw.p12_top   Joystick 1 Top Button
     OIData->swA.p1_p3_sw.p12_aux1  Joystick 1 Aux 1
     OIData->swA.p1_p3_sw.p12_aux2  Joystick 1 Aux 2

     OIData->p2_x                   Joystick 2 X Axis
     OIData->p2_y                   Joystick 2 Y Axis
     OIData->p2_wheel               Joystick 2 Wheel
     OIData->p2_aux                 Joystick 2 Aux
     OIData->swB.p2_p4_sw.p12_trig  Joystick 2 Trigger
     OIData->swB.p2_p4_sw.p12_top   Joystick 2 Top Button
     OIData->swB.p2_p4_sw.p12_aux1  Joystick 2 Aux 1
     OIData->swB.p2_p4_sw.p12_aux2  Joystick 2 Aux 2
     
     OIData->p3_x                   Joystick 3 X Axis
     OIData->p3_y                   Joystick 3 Y Axis
     OIData->p3_wheel               Joystick 3 Wheel
     OIData->p3_aux                 Joystick 3 Aux
     OIData->swA.p1_p3_sw.p34_trig  Joystick 3 Trigger
     OIData->swA.p1_p3_sw.p34_top   Joystick 3 Top Button
     OIData->swA.p1_p3_sw.p34_aux1  Joystick 3 Aux 1
     OIData->swA.p1_p3_sw.p34_aux2  Joystick 3 Aux 2
     
     OIData->p4_x                   Joystick 4 X Axis
     OIData->p4_y                   Joystick 4 Y Axis
     OIData->p4_wheel               Joystick 4 Wheel
     OIData->p4_aux                 Joystick 4 Aux
     OIData->swB.p2_p4_sw.p34_trig  Joystick 4 Trigger
     OIData->swB.p2_p4_sw.p34_top   Joystick 4 Top Button
     OIData->swB.p2_p4_sw.p34_aux1  Joystick 4 Aux 1
     OIData->swB.p2_p4_sw.p34_aux2  Joystick 4 Aux 2
     
     OIData->PB_mode.comp_mode      Competition Mode (0 = enabled, 1 = disabled)
     OIData->PB_mode.auton_mode     Autonomous Mode (0 = normal, 1 = autonomous)
     
 ******************************************************************************/    
  
 /*---------- 1 Joystick Drive -------------------------------------------------
  *-----------------------------------------------------------------------------
  *  This code mixes the Y and X axis on one joystick for driving. 
  *  Joystick forward  = Robot forward
  *  Joystick backward = Robot backward
  *  Joystick right    = Robot rotates right
  *  Joystick left     = Robot rotates left
  *  Connect the left  drive motors to PWM OUT 1 and/or PWM OUT 3 on the RC.
  *  Connect the right drive motors to PWM OUT 2 and/or PWM OUT 4 on the RC.
  */  
   pwm01 = pwm03 = Limit_Mix(2000 + OIData->p1_x + OIData->p1_y - 127);   /* LEFT  WHEELS */
   pwm02 = pwm04 = Limit_Mix(2000 + OIData->p1_y - OIData->p1_x + 127);   /* RIGHT WHEELS */
   pwm01 = pwm03 = 255 - pwm01;    /* reverse direction of left side */


 /* ------ Other PWM OUTPUT Mapping (can be used for two-joystick drive ------*/
  pwm05 = OIData->p3_x;    /* limited by digital inputs 1 & 2 below */
  pwm06 = OIData->p3_y;    /* limited by digital inputs 3 & 4 below */
  pwm07 = OIData->p4_x;
  pwm08 = OIData->p4_y;


 /*---------- Limit Switches to limit PWM OUTPUTs 3 & 4 -------------------*/
  Limit_Switch_Max(rc_dig_in06, &pwm05);  /* PWM05 won't go FWD if rc_dig_in06 is low. */
  Limit_Switch_Min(rc_dig_in08, &pwm05);  /* PWM05 won't go REV if rc_dig_in08 is low. */
  Limit_Switch_Max(rc_dig_in10, &pwm06);  /* PWM06 won't go FWD if rc_dig_in10 is low. */
  Limit_Switch_Min(rc_dig_in12, &pwm06);  /* PWM06 won't go REV if rc_dig_in12 is low. */


 /*---------- OI Joystick buttons toggle solenoids ------------------------*/

  solenoid1 = OIData->swA.p1_p3_sw.p12_trig;
  solenoid2 = OIData->swA.p1_p3_sw.p12_top;
  
  solenoid3 = OIData->swA.p1_p3_sw.p34_trig;
  solenoid4 = OIData->swA.p1_p3_sw.p34_top;
  

 /*---------- Switch inputs toggle solenoids ------------------------------*/
  solenoid5 = !rc_dig_in14;
  solenoid6 = !rc_dig_in16;


} /* END Default_Routine(); */


/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
