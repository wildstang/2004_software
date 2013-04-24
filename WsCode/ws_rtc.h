/*******************************************************************************
* FILE NAME: ws_rtc.h
*
* DESCRIPTION: 
*  This file contains the access function to the real-time clock (RTC).
*
* USAGE:
*******************************************************************************/

#ifndef __WS_RTC_H_
#define __WS_RTC_H_

#include "compiler.h"

#ifndef CYGWIN_IFI_SIMULATOR
  /* Real target build */

#else
  /* Simulator build */

#endif

#define MAX_RTC_DELTAS 2  /* Number of RTC delta timers supported */
#define MAX_RTC_TIMER_VAL 0xFFFF  /* Timers are 16-bit, so the max is FFFF */

/* Define who uses each of the RTC delta timers */
#define RTC_DELTA_BIG_ARM   0
#define RTC_DELTA_UNUSED    1

/*******************************************************************************/
/*                         GLOBAL VARIABLES                                    */
/*******************************************************************************/

/*******************************************************************************
                           FUNCTION PROTOTYPES
*******************************************************************************/

/* NOTE: The time from the beginning of one 26ms loop to the next is about
 * 0x7700 RTC ticks, so the RTC needs to be read fairly often to prevent
 * a wrap around.
 */
void initRtc(void);    /* Initialize the RTC */
UINT16 getRtc(void);   /* Return the current RTC value */
UINT16 getRtcDelta(UINT8 timer_num);  /* Get the diff in RTC since the last call */

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
#endif
