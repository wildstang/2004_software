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
#include "ws_includes.h"
#include "user_routines.h"
#include "printf_lib.h"
#include "eeprom.h"

#else
#include "RC_Simulator.h"
#include "ifi_aliases.h"
#include "ifi_default.h"
#include "ws_includes.h"
#include "eeprom.h"
#include "stdio.h"
#include <time.h>
#endif

#include "ws_calibrate.h"
#include "ws_general.h"
#include "ws_io.h"
#include "ws_autonomous.h"

tx_data_record txdata;          /* DO NOT CHANGE! */
rx_data_record rxdata;          /* DO NOT CHANGE! */
packed_struct statusflag;       /* DO NOT CHANGE! */

extern UINT8 g_forced_auto;

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
  MotorValsType        motor_vals;
  CrabTargetsType      crab_targets;
  CalibrationValsType  calibration_vals;
  int                  pot_state;

#ifdef UNCHANGEABLE_DEFINITION_AREA
  IFI_Initialization ();        /* DO NOT CHANGE! */
#endif

  User_Initialization();

  statusflag.NEW_SPI_DATA = 0;  /* DO NOT CHANGE! */ 

  retrieve_calibration(&calibration_vals);

  motor_vals.small_arm_squeeze = SQUEEZE_IN;

  while (1)
  {
#ifdef _SIMULATOR
    statusflag.NEW_SPI_DATA = 1;
#endif

    pot_state = test_pots(GET_ANALOG_VALUE_SHIFT(Analog_in_crab_pot));

    if (statusflag.NEW_SPI_DATA)
    {
      /* 26.2ms loop area */
      /* I'm slow!  I only execute every 26.2ms because */
      /* that's how fast the Master uP gives me data. */

      Process_Data_From_Master_uP(&motor_vals, &crab_targets,
                                  &calibration_vals, pot_state);

      if ((autonomous_mode) || (g_forced_auto == TRUE))
      {
        auto_main(&calibration_vals);
      }
    }

    /* You edit this in user_routines_fast.c */
    /* I'm fast!  I execute during every loop.*/
    Process_Data_From_Local_IO(&motor_vals, &crab_targets, &calibration_vals,
                               pot_state);

#ifdef CYGWIN_IFI_SIMULATOR
   simulator_end_main_loop();  /* Give the simulator a chance to send out some
                                * data and to rest for a bit.
                                */
#endif

  } /* while (1) */
}  /* END of Main */


/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
