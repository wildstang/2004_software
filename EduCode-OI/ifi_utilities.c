/*******************************************************************************
* FILE NAME: ifi_utilities.c
*
* DESCRIPTION:
*  This file contains some useful functions that you can call in your program.
*
* USAGE:
*  The user should NOT modify this file, so that if a new version is released
*  by Innovation First then it can be easily replaced.
*  The user should add their own functions to either user_routines.c or another
*  custom file.
*
*******************************************************************************/

#include <usart.h>
#include <spi.h>
#include <adc.h>
#include <capture.h>
#include <timers.h>
#include <string.h>
#include <pwm.h>
#include "delays.h"       /*defined locally*/
#include "ifi_aliases.h"
#include "ifi_default.h"
#include "ifi_utilities.h"
#include "user_routines.h"
#include "oi_data.h"

int             ifi_packet_num1 = 0;
int             ifi_last_packet1 = 0;
unsigned char   *ptr;
unsigned char   ifi_count;
unsigned char   ifi_analog_channels;

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
void Set_Number_of_Analog_Channels (unsigned char number_of_channels)
{
  ifi_analog_channels = number_of_channels;
}


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
unsigned int Get_Analog_Value (unsigned char ADC_channel)
{
  unsigned int result;

  OpenADC( ADC_FOSC_RC & ADC_RIGHT_JUST & ifi_analog_channels,
          ADC_channel & ADC_INT_OFF & ADC_VREFPLUS_VDD & ADC_VREFMINUS_VSS );
  Delay10TCYx( 10 );
  ConvertADC();
  while( BusyADC() );
  ReadADC();
  CloseADC();
  result = (int) ADRESH << 8 | ADRESL;
  return result;
}


/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
