/*******************************************************************************
* FILE NAME: user_routines.c <FRC VERSION>
*
* DESCRIPTION:
*  This file contains the default mappings of inputs  
*  (like switches, joysticks, and buttons) to outputs on the RC.  
*
* USAGE:
*  You can either modify this file to fit your needs, or remove it from your 
*  project and replace it with a modified copy. 
*
* OPTIONS:
*  You can choose between one joystick operation or two joystick operation.  
*  Simply delete or comment out line 24 below which says "#define ONE_JOYSTICK" 
*  if you want to use two joysticks for drive.         
*******************************************************************************/

#include "ifi_aliases.h"
#include "ifi_default.h"
#include "ifi_utilities.h"
#include "user_routines.h"
#include "printf_lib.h"
#include "tracker.h"

#define ONE_JOYSTICK /* PWM OUTPUTS 13-16 can be used for one joystick drive */

#ifdef ONE_JOYSTICK
#define pwm02_led_input p1_x
#else
#define pwm02_led_input p2_y
#endif


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
*                limit switch is on (closed).
* CALLED FROM:   this file
* ARGUMENTS:     
*     Argument       Type             IO   Description
*     --------       -------------    --   -----------
*     switch_on      unsigned char    I    limit switch value
*     *input_value   pointer           O   points to PWM byte value to be limited
* RETURNS:       void
*******************************************************************************/
void Limit_Switch_Max(unsigned char switch_on, unsigned char *input_value)
{
  if (switch_on == 1)
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
*     switch_on      unsigned char    I    limit switch value
*     *input_value   pointer           O   points to PWM byte value to be limited
* RETURNS:       void
*******************************************************************************/
void Limit_Switch_Min(unsigned char switch_on, unsigned char *input_value)
{
  if (switch_on == 1)
  { 
    if(*input_value < 127)
      *input_value = 127;
  }
}


/*******************************************************************************
* FUNCTION NAME: Limit_Mix
* PURPOSE:       Limits the mixed value for one joystick drive.
* CALLED FROM:   this file
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
* FUNCTION NAME: User_Initialization
* PURPOSE:       This routine is called first (and only once) in the Main function.  
*                You may modify and add to this function.
* CALLED FROM:   main.c
* ARGUMENTS:     none
* RETURNS:       void
*******************************************************************************/
void User_Initialization (void)
{
  rom const	char *strptr = "IFI User Processor Initialized ...";

  Set_Number_of_Analog_Channels(SIXTEEN_ANALOG);    /* DO NOT CHANGE! */

/* FIRST: Set up the I/O pins you want to use as digital INPUTS. */
  digital_io_01 = digital_io_02 = digital_io_03 = digital_io_04 = INPUT;
  digital_io_05 = digital_io_06 = digital_io_07 = digital_io_08 = INPUT;
  digital_io_09 = digital_io_10 = digital_io_11 = digital_io_12 = INPUT;
    /* 
     Note: digital_io_01 = digital_io_02 = ... digital_io_12 = INPUT; 
           is the same as the following:

           digital_io_01 = INPUT;
           digital_io_02 = INPUT;
           ...
           digital_io_12 = INPUT;
    */

/* SECOND: Set up the I/O pins you want to use as digital OUTPUTS. */
  digital_io_13 = digital_io_14 = digital_io_15 = OUTPUT;
  digital_io_16 = digital_io_17 = digital_io_18 = OUTPUT;

/* THIRD: Initialize the values on the digital outputs. */
  rc_dig_out13 = rc_dig_out14 = rc_dig_out15 = 0;
  rc_dig_out16 = rc_dig_out17 = rc_dig_out18 = 0;

/* FOURTH: Set your initial PWM values.  Neutral is 127. */
  pwm01 = pwm02 = pwm03 = pwm04 = pwm05 = pwm06 = pwm07 = pwm08 = 127;
  pwm09 = pwm10 = pwm11 = pwm12 = pwm13 = pwm14 = pwm15 = pwm16 = 127;

/* FIFTH: Set your PWM output types for PWM OUTPUTS 13-16.
  /*   Choose from these parameters for PWM 13-16 respectively:               */
  /*     IFI_PWM  - Standard IFI PWM output generated with Generate_Pwms(...) */
  /*     USER_CCP - User can use PWM pin as digital I/O or CCP pin.           */
  Setup_PWM_Output_Type(IFI_PWM,IFI_PWM,IFI_PWM,IFI_PWM);

  /* 
     Example: The following would generate a 40KHz PWM with a 50% duty cycle on the CCP2 pin:

         CCP2CON = 0x3C;
         PR2 = 0xF9;
         CCPR2L = 0x7F;
         T2CON = 0;
         T2CONbits.TMR2ON = 1;

         Setup_PWM_Output_Type(USER_CCP,IFI_PWM,IFI_PWM,IFI_PWM);
  */

  /* Add any other initialization code here. */

  Initialize_Serial_Comms();   
  
  Initialize_Tracker();

  Putdata(&txdata);             /* DO NOT CHANGE! */

  printf("%s\n", strptr);

  User_Proc_Is_Ready();         /* DO NOT CHANGE! - last line of User_Initialization */
}

/*******************************************************************************
* FUNCTION NAME: Process_Data_From_Master_uP
* PURPOSE:       Executes every 26.2ms when it gets new data from the master 
*                microprocessor.
* CALLED FROM:   main.c
* ARGUMENTS:     none
* RETURNS:       void
*******************************************************************************/
void Process_Data_From_Master_uP(void)
{
  Getdata(&rxdata); /* Get fresh data from the master microprocessor. */

//  Default_Routine();  /* Optional.  See below. */

//  Generate_Pwms(pwm13,pwm14,pwm15,pwm16);

  Putdata(&txdata);             /* DO NOT CHANGE! */
}

/*******************************************************************************
* FUNCTION NAME: Default_Routine
* PURPOSE:       Performs the default mappings of inputs to outputs for the
*                Robot Controller.
* CALLED FROM:   this file, Process_Data_From_Master_uP routine
* ARGUMENTS:     none
* RETURNS:       void
------------------------------------------------------------------------------*/
void Default_Routine(void)
{
  
 /*---------- Analog Inputs (Joysticks) to PWM Outputs-----------------------
  *--------------------------------------------------------------------------
  *   This maps the joystick axes to specific PWM outputs.
  */
  pwm01 = p1_y;   
  pwm02 = p2_y;   
  pwm03 = p3_y;   
  pwm04 = p4_y;   
  pwm05 = p1_x;   
  pwm06 = p2_x;   
  pwm07 = p3_x;   
  pwm08 = p4_x;   
  pwm09 = p1_wheel;
  pwm10 = p2_wheel;   
  pwm11 = p3_wheel;   
  pwm12 = p4_wheel;   
  
#ifdef  ONE_JOYSTICK
 /*---------- 1 Joystick Drive ----------------------------------------------
  *--------------------------------------------------------------------------
  *  This code mixes the Y and X axis on Port 1 to allow one joystick drive. 
  *  Joystick forward  = Robot forward
  *  Joystick backward = Robot backward
  *  Joystick right    = Robot rotates right
  *  Joystick left     = Robot rotates left
  *  Connect the left  drive motors to PWM15 and/or PWM16 on the RC.
  *  Connect the right drive motors to PWM13 and/or PWM14 on the RC.
  */  
  pwm13 = pwm14 = Limit_Mix(2000 + p1_y + p1_x - 127);
  pwm15 = pwm16 = Limit_Mix(2000 + p1_y - p1_x + 127);
#else
  pwm13 = pwm14 = pwm15 = pwm16 = 127;        /* These are left in neutral. */
#endif
  
 /*---------- Buttons to Relays----------------------------------------------
  *--------------------------------------------------------------------------
  *  This default code maps the joystick buttons to specific relay outputs.  
  *  Relays 1 and 2 use limit switches to stop the movement in one direction.
  *  The & used below is the C symbol for AND                                
  */
  relay1_fwd = p1_sw_trig & rc_dig_in01;  /* FWD only if switch1 is not closed. */
  relay1_rev = p1_sw_top  & rc_dig_in02;  /* REV only if switch2 is not closed. */
  relay2_fwd = p2_sw_trig & rc_dig_in03;  /* FWD only if switch3 is not closed. */
  relay2_rev = p2_sw_top  & rc_dig_in04;  /* REV only if switch4 is not closed. */
  relay3_fwd = p3_sw_trig;
  relay3_rev = p3_sw_top;
  relay4_fwd = p4_sw_trig;
  relay4_rev = p4_sw_top;
  relay5_fwd = p1_sw_aux1;
  relay5_rev = p1_sw_aux2;
  relay6_fwd = p2_sw_aux1;
  relay6_rev = p2_sw_aux2;
  relay7_fwd = p3_sw_aux1;
  relay7_rev = p3_sw_aux2;
  relay8_fwd = p4_sw_aux1;
  relay8_rev = p4_sw_aux2;
  
  /*---------- PWM outputs Limited by Limit Switches  ------------------------*/
  
  Limit_Switch_Max(rc_dig_in05, &pwm03);
  Limit_Switch_Min(rc_dig_in06, &pwm03);
  Limit_Switch_Max(rc_dig_in07, &pwm04);
  Limit_Switch_Min(rc_dig_in08, &pwm04);
  Limit_Switch_Max(rc_dig_in09, &pwm11);
  Limit_Switch_Min(rc_dig_in10, &pwm11);
  Limit_Switch_Max(rc_dig_in11, &pwm12);
  Limit_Switch_Min(rc_dig_in12, &pwm12);
  
 /*---------- ROBOT FEEDBACK LEDs------------------------------------------------
  *------------------------------------------------------------------------------
  *   This section drives the "ROBOT FEEDBACK" lights on the Operator Interface.
  *   The lights are green for joystick forward and red for joystick reverse.
  *   Both red and green are on when the joystick is centered.  Use the
  *   trim tabs on the joystick to adjust the center.     
  *   These may be changed for any use that the user desires.                       
  */	
  
  if (user_display_mode == 0) /* User Mode is Off */
    
  { /* Check position of Port 1 Joystick */
    if (p1_y >= 0 && p1_y <= 56)
    {                     /* Joystick is in full reverse position */
      pwm1_green  = 0;    /* Turn PWM1 green LED - OFF */
      pwm1_red  = 1;      /* Turn PWM1 red LED   - ON  */
    }
    else if (p1_y >= 125 && p1_y <= 129)
    {                     /* Joystick is in neutral position */
      pwm1_green  = 1;    /* Turn PWM1 green LED - ON */
      pwm1_red  = 1;      /* Turn PWM1 red LED   - ON */
    }
    else if (p1_y >= 216 && p1_y <= 255)
    {                     /* Joystick is in full forward position*/
      pwm1_green  = 1;    /* Turn PWM1 green LED - ON  */
      pwm1_red  = 0;      /* Turn PWM1 red LED   - OFF */
    }
    else
    {                     /* In either forward or reverse position */
      pwm1_green  = 0;    /* Turn PWM1 green LED - OFF */
      pwm1_red  = 0;      /* Turn PWM1 red LED   - OFF */
    }  /*END Check position of Port 1 Joystick
    
    /* Check position of Port 2 Y Joystick 
           (or Port 1 X in Single Joystick Drive Mode) */
    if (pwm02_led_input >= 0 && pwm02_led_input <= 56)
    {                     /* Joystick is in full reverse position */
      pwm2_green  = 0;    /* Turn pwm2 green LED - OFF */
      pwm2_red  = 1;      /* Turn pwm2 red LED   - ON  */
    }
    else if (pwm02_led_input >= 125 && pwm02_led_input <= 129)
    {                     /* Joystick is in neutral position */
      pwm2_green  = 1;    /* Turn PWM2 green LED - ON */
      pwm2_red  = 1;      /* Turn PWM2 red LED   - ON */
    }
    else if (pwm02_led_input >= 216 && pwm02_led_input <= 255)
    {                     /* Joystick is in full forward position */
      pwm2_green  = 1;    /* Turn PWM2 green LED - ON  */
      pwm2_red  = 0;      /* Turn PWM2 red LED   - OFF */
    }
    else
    {                     /* In either forward or reverse position */
      pwm2_green  = 0;    /* Turn PWM2 green LED - OFF */
      pwm2_red  = 0;      /* Turn PWM2 red LED   - OFF */
    }  /* END Check position of Port 2 Joystick */
    
    /* This drives the Relay 1 and Relay 2 "Robot Feedback" lights on the OI. */
    Relay1_green = relay1_fwd;    /* LED is ON when Relay 1 is FWD */
    Relay1_red = relay1_rev;      /* LED is ON when Relay 1 is REV */
    Relay2_green = relay2_fwd;    /* LED is ON when Relay 2 is FWD */
    Relay2_red = relay2_rev;      /* LED is ON when Relay 2 is REV */
    Switch1_LED = 0;
    Switch2_LED = 0;

    /* Toggle an LED on the OI to show we're alive. */
    Switch3_LED = !(int)Switch3_LED;
    
  } /* (user_display_mode = 0) (User Mode is Off) */
  
  else  /* User Mode is On */
  {
    txdata.LED_byte1.data = p1_y;
  }   
  
} /* END Default_Routine(); */


/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
