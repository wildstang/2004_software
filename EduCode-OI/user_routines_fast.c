/*******************************************************************************
* FILE NAME: user_routines_fast.c <EDU VERSION>
*
* DESCRIPTION:
*  This file is where the user can add their custom code within the framework
*  of the routines below. 
*
* USAGE:
*  You can either modify this file to fit your needs, or remove it from your 
*  project and replace it with a modified copy. 
*
* OPTIONS:  Interrupts are disabled and not used by default.
*******************************************************************************/

#include <usart.h>
#include <adc.h>
#include <capture.h>
#include <timers.h>
#include "ifi_aliases.h"
#include "ifi_default.h"
#include "ifi_utilities.h"
#include "user_routines.h"


/* DEFINE USER VARIABLES HERE */


/*******************************************************************************
* FUNCTION NAME: InterruptVectorLow
* PURPOSE:       Low priority interrupt vector
* CALLED FROM:   nowhere by default
* ARGUMENTS:     none
* RETURNS:       void
* DO NOT MODIFY OR DELETE THIS FUNCTION 
*******************************************************************************/
#pragma code InterruptVectorLow = LOW_INT_VECTOR
void InterruptVectorLow (void)
{
  _asm
    goto InterruptHandlerLow  /*jump to interrupt routine*/
  _endasm
}


/*******************************************************************************
* FUNCTION NAME: InterruptHandlerLow
* PURPOSE:       Low priority interrupt handler
* If you want to use these two external low priority interrupts or any of the
* peripheral interrupts then you must enable them in your initialization
* routine.  Innovation First, Inc. will not provide support for using these
* interrupts, so be careful.
* CALLED FROM:   this file, InterruptVectorLow routine
* ARGUMENTS:     none
* RETURNS:       void
*******************************************************************************/
#pragma code
#pragma interrupt InterruptHandlerLow

void InterruptHandlerLow ()     
{                               
  unsigned char int_byte;       
  if (INTCON3bits.INT2IF)         /* The INT2 pin is RB2/INTERRUPTS 1. */
  { 
    INTCON3bits.INT2IF = 0;
  }
  else if (INTCON3bits.INT3IF)    /* The INT3 pin is RB3/INTERRUPTS 2. */
  {
    INTCON3bits.INT3IF = 0;
  }
  else if (INTCONbits.RBIF)       /* INTERRUPTS 3-4 (RB4, RB5, RB6, or RB7) changed. */
  {
    int_byte = PORTB;             /* You must read or write to PORTB   */
    INTCONbits.RBIF = 0;          /* and clear the interrupt flag      */
  }                               /* to clear the interrupt condition. */
}


/*******************************************************************************
* FUNCTION NAME: Process_Data_From_Local_IO
* PURPOSE:       Execute user's realtime code.
* You should modify this routine by adding code which you wish to run fast.
* It will be executed every instruction cycle, and not wait for fresh data 
* from the master processor.
* CALLED FROM:   main.c
* ARGUMENTS:     none
* RETURNS:       void
*******************************************************************************/
void Process_Data_From_Local_IO(void)
{
  /* Add code here that you want to be executed every instruction cycle. */
}


/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
