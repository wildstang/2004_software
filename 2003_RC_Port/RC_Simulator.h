/*******************************************************************************
* This file defines the prototypes needed for simulating the IFI code that
* is provided for the target build.  For the simulator build, RC_Simulator
* implements the IFI functionality.
*******************************************************************************/

/* 'near' and 'far' are not needed in WIN32 so just define them to nothing to
 * avoid a compiler error when they are used
 */
#define near
#define far

#define rom


#include "ws_code.h"
#include "user_routines.h"
#include "ifi_aliases.h"
#include "ifi_default.h"
#include "io_code.h"



/*****************  From ifi_library.c *******************************/
void Getdata(rx_data_ptr ptr);
void Putdata(tx_data_ptr ptr);
void Setup_PWM_Output_Type(int pwmSpec1,int pwmSpec2,int pwmSpec3,int pwmSpec4);
void User_Proc_Is_Ready (void);
void IFI_Initialization (void);

/*****************  From ifi_utilities.c *******************************/

/*******************************************************************************
* FUNCTION NAME: Initialize_Serial_Comms
* PURPOSE:       Opens the serial port 1 for communicating with your PC at
*                115k baud, 8 bits, no parity, one stop bit, and no flow control.
* CALLED FROM:   user_routines.c
* ARGUMENTS:     none
* RETURNS:       void
*******************************************************************************/
void Initialize_Serial_Comms (void);

/*******************************************************************************
* FUNCTION NAME: Set_Number_of_Analog_Channels
* PURPOSE:       Sets the variable used in Get_Analog_Value routine (below)
*                to the number of analog channels desired by the user.
* CALLED FROM:   user_routines.c initialization, typically
* ARGUMENTS:     
*      Argument            Type    IO   Description
*     -----------          -----   --   -----------
*     number_of_channels   alias   I    choose alias from ifi_aliases.h
* RETURNS:       void
*******************************************************************************/
void Set_Number_of_Analog_Channels (unsigned char number_of_channels);

/*******************************************************************************
* FUNCTION NAME: Get_Analog_Value
* PURPOSE:       Reads the analog voltage on an A/D port and returns the
*                10-bit value read stored in an unsigned int.
* CALLED FROM:   user_routines.c, typically
* ARGUMENTS:     
*      Argument         Type        IO   Description
*     -----------   -------------   --   -----------
*     ADC_channel       alias       I    alias found in ifi_aliases.h
* RETURNS:       unsigned int
*******************************************************************************/
unsigned int Get_Analog_Value (unsigned char ADC_channel);

/********************************** From user_routines_fast.c ********************************/


/*******************************************************************************
* FUNCTION NAME: User_Autonomous_Code
* PURPOSE:       Execute user's code during autonomous robot operation.
* You should modify this routine by adding code which you wish to run in
* autonomous mode.  It will be executed every program loop, and not
* wait for or use any data from the Operator Interface.
* CALLED FROM:   main.c file, main() routine when in Autonomous mode
* ARGUMENTS:     none
* RETURNS:       void
*******************************************************************************/
void User_Autonomous_Code(void);

/*******************************************************************************
* FUNCTION NAME: Process_Data_From_Local_IO
* PURPOSE:       Execute user's realtime code.
* You should modify this routine by adding code which you wish to run fast.
* It will be executed every program loop, and not wait for fresh data 
* from the Operator Interface.
* CALLED FROM:   main.c
* ARGUMENTS:     none
* RETURNS:       void
*******************************************************************************/
void Process_Data_From_Local_IO(MotorValsType *motor_vals,
                                CrabTargetsType *crab_targets,
                                int pot_state);


/****************************** From Util_lib.asm ************************************************/
#ifdef _FRC_BOARD
/* located in ifi_library.lib */
void Generate_Pwms(unsigned char pwm_13,unsigned char pwm_14,
                   unsigned char pwm_15,unsigned char pwm_16);
#else
/* located in ifi_library.lib */
void Generate_Pwms(unsigned char pwm_1,unsigned char pwm_2,
                   unsigned char pwm_3,unsigned char pwm_4,
                   unsigned char pwm_5,unsigned char pwm_6,
                   unsigned char pwm_7,unsigned char pwm_8);
#endif



