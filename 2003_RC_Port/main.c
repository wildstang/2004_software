/*******************************************************************************
* FILE NAME: main.c <FRC VERSION>
*
* DESCRIPTION:
*  This file contains the main program loop.
*
* USAGE:
*  You should not need to modify this file.
*  Note the different loop speed for the two routines:
*     Process_Data_From_Master_uP
*     Process_Data_From_Local_IO
*******************************************************************************/

#ifndef CYGWIN_IFI_SIMULATOR
#include "ifi_aliases.h"     /* Real target build includes */
#include "ifi_default.h"
#include "ifi_utilities.h"
#include "ws_code.h"
#include "user_routines.h"
#include "printf_lib.h"
#include "io_code.h"

#else
#define MS_IN_A_NANO_SEC (1000*1000)
#include "RC_Simulator.h"
#include "ifi_aliases.h"
#include "ifi_default.h"
#include "ws_code.h"
#include "io_code.h"
#include "stdio.h"
#include <time.h>
#endif

tx_data_record txdata;          /* DO NOT CHANGE! */
rx_data_record rxdata;          /* DO NOT CHANGE! */
packed_struct statusflag;       /* DO NOT CHANGE! */

/*******************************************************************************
* FUNCTION NAME: main
* PURPOSE:       Main program function.
* CALLED FROM:   ifi_startup.c
* ARGUMENTS:     none
* RETURNS:       void
* DO NOT DELETE THIS FUNCTION 
*******************************************************************************/
#ifdef CYGWIN_IFI_SIMULATOR
int ifi_main(void) /* In the simulator, ifi_main can be called */
#else
void main (void)   /* This is main for IFI, not for the simulator */
#endif
{
  MotorValsType motor_vals;
  CrabTargetsType crab_targets;
  int pot_state;

#ifdef UNCHANGEABLE_DEFINITION_AREA
  IFI_Initialization ();        /* DO NOT CHANGE! */
#endif

  User_Initialization();

  statusflag.NEW_SPI_DATA = 0;  /* DO NOT CHANGE! */ 

  motor_vals.pump = RELAY_REV;
  motor_vals.foot = RELAY_REV;

  while (1)
  {
#ifdef _SIMULATOR
    statusflag.NEW_SPI_DATA = 1;
#endif

    pot_state = test_pots(GET_ANALOG_VALUE_SHIFT(Analog_in_right_pot),
                          GET_ANALOG_VALUE_SHIFT(Analog_in_left_pot));

    /* TEMPORARY FOR TESTING */
    pot_state = POTS_GOOD;

    if (statusflag.NEW_SPI_DATA)
    {
      /* 26.2ms loop area */
      /* I'm slow!  I only execute every 26.2ms because */
      /* that's how fast the Master uP gives me data. */
      Process_Data_From_Master_uP(&motor_vals, &crab_targets, pot_state);

      if (autonomous_mode)
      {
        User_Autonomous_Code();
      }
    }

    /* You edit this in user_routines_fast.c */
    /* I'm fast!  I execute during every loop.*/
    Process_Data_From_Local_IO(&motor_vals, &crab_targets, pot_state);


    /*
    printf(" pwm1 %d", (int)pwm01);
    printf(" Lcrab speed %d", (int)motor_vals.left_crab);
    printf(" Rcrab speed %d", (int)motor_vals.right_crab);
    printf("\n");
    */

#ifdef CYGWIN_IFI_SIMULATOR
   {
   struct timespec  sleep_time;
   sleep_time.tv_sec=0;
   sleep_time.tv_nsec=MS_IN_A_NANO_SEC*2; /* A quick 2ms nap */
   /* For a simulator build, let the processor rest for a bit or my battery will be drained in a sec */
   nanosleep(&sleep_time,NULL);
   }
#endif

  } /* while (1) */
}  /* END of Main */


/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
