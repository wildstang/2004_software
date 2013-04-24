/*******************************************************************************
* FILE NAME ws_rtc.c
*
* DESCRIPTION:
*  This file contains the access function to the real-time clock (RTC).
*
* USAGE:
*
*******************************************************************************/

#ifndef CYGWIN_IFI_SIMULATOR
/* Real target build */
#include "ws_rtc.h"
#include "ifi_picdefs.h"

#else
/* Simulator build */
#include "sim_picdefs.h"

#endif

static void Initialize_Timer_1(void);

/* In order to provide RTC deltas, we must keep the previous value of the RTC */
static UINT16 s_rtcPrev[MAX_RTC_DELTAS];

/*******************************************************************************
* FUNCTION NAME: initRtc
* PURPOSE:       Initialize the RTC.
* CALLED FROM:   anywhere
* ARGUMENTS:     none
* RETURNS:       none
*******************************************************************************/
void initRtc(void)
{
   int i;

   /* Init all the delta values to zero */
   for(i=0;i<MAX_RTC_DELTAS;i++)
   {
      s_rtcPrev[i]=0;
   }

   /* The RTC relies on Timer1 to keep time, init it now */
   Initialize_Timer_1();

   return;
}

/*******************************************************************************
* FUNCTION NAME: getRtc
* PURPOSE:       Return the current value of the RTC.
* CALLED FROM:   anywhere
* ARGUMENTS:     none
* RETURNS:       The current value of the RTC.
*******************************************************************************/
UINT16 getRtc(void)
{
   //    In 16-bit mode, reading TMR1L will latch a copy of TMR1H into a buffer
   //    mapped to the TMR1H memory address. Conversely, a write to the buffer
   //    followed by a write to the TMR1L register will update the entire 16-bit
   //    timer at once. This solves the problem where the timer may overflow
   //    between two 8-bit accesses.
   UINT8  tmp;      // 8-bit temporary buffer
   UINT16 rtc_val; // 16-bit variable

   tmp = TMR1L;     // TMR1L must be read before TMR1H
   rtc_val = TMR1H;
   rtc_val <<= 8;   // move TMR1H data to the upper half of the variable
   rtc_val += tmp;  // we now have all sixteen bits  

   return(rtc_val);
}

/*******************************************************************************
* FUNCTION NAME: getRtcDelta
* PURPOSE:       Returns the number of RTC ticks that have expired since last
*                calling this function
* CALLED FROM:   anywhere
* ARGUMENTS:     timer_num - The number of the timer being requested.  This
*                allows multiple different chunks of code to call this function
*                and they will each get the correct delta.  The max number of
*                RTC deltas kept is MAX_RTC_DELTAS, and timer_num is zero based.
* RETURNS:       The current value of the RTC.
*******************************************************************************/
UINT16 getRtcDelta(UINT8 timer_num)
{
   UINT16 rtc_val;
   UINT16 rtc_delta;

   if(timer_num <= (MAX_RTC_DELTAS - 1))
   {
      rtc_val = getRtc();  /* Get the current RTC value */
      if(rtc_val < s_rtcPrev[timer_num])
      {
         /* The curr rtc_val is less than the last RTC value so we must of wrapped
          * around.  In this case, calculate the wrap around difference 
          */
         rtc_delta = MAX_RTC_TIMER_VAL - s_rtcPrev[timer_num];
         rtc_delta = rtc_delta + rtc_val;
      }
      else
      {
         /* Figure out the difference between the previous and current RTC */
         rtc_delta = rtc_val - s_rtcPrev[timer_num]; 
      }
      
      /* Update the RTC value for this timer so next time around we can compare it */
      s_rtcPrev[timer_num] = rtc_val;
   }
   else
   {
      /* Invalid timer value, log an error...can't */
      rtc_delta = 0;
   }

   return(rtc_delta);
}

/*******************************************************************************
*
*   FUNCTION:   Initialize_Timer_1()
*
*   PURPOSE:      Initializes the timer 1 hardware.
*
*   CALLED FROM:   
*
*   PARAMETERS:   None
*
*   RETURNS:      Nothing
*
*   COMMENTS:   
*
*              Timer 1 documentation starts on page 135 of the data sheet.
*
*******************************************************************************/
static void Initialize_Timer_1(void)  
{
   TMR1L = 0x00;            // least significant 8-bits of the timer 1 register (this is readable and writable)
   TMR1H = 0x00;            // most significant 8-bits of the timer 1 register (this is readable and writable)

   T1CONbits.T1CKPS0 = 1;   // T1CSP1 T1CSP0
   T1CONbits.T1CKPS1 = 1;   //   0      0      1:1 prescaler (clock=10MHz/each tick=100ns)
                            //   0      1      1:2 prescaler (clock=5MHz/each tick=200ns)
                            //   1      0      1:4 prescaler (clock=2.5MHz/each tick=400ns)
                            //   1      1      1:8 prescaler (clock=1.25MHz/each tick=800ns)

   T1CONbits.T1OSCEN = 0;   // 0: timer 1 oscillator disabled (leave at 0 to allow the use of an external clock)
                            // 1: timer 1 oscillator enabled (can't be used because of hardware constraints)

   T1CONbits.TMR1CS = 0;    // 0: use the internal clock
                            // 1: use an external clock on RC0/T1OSO/T13CLK (rc_dig_in14 on full-size controller)

   T1CONbits.RD16 = 1;      // 0: timer 1 register operations are done in two 8-bit accesses
                            // 1: timer 1 register operations are done in one 16-bit access
                            //    In this mode, reading TMR1L will latch a copy of TMR1H into a buffer
                            //    mapped to the TMR1H memory address. Conversely, a write to the buffer
                            //    followed by a write to the TMR1L register will update the entire 16-bit
                            //    timer at once. This solves the problem where the timer may overflow
                            //    between two 8-bit accesses. Here's an example of how to do a 16-bit read:

                            //    unsigned char Temp_Buf; // 8-bit temporary buffer
                            //    unsigned int Timer_Snapshot; // 16-bit variable

                            //    Temp_Buf = TMR1L; // TMR1L must be read before TMR1H
                            //    Timer_Snapshot = TMR1H;
                            //    Timer_Snapshot <<= 8; // move TMR1H data to the upper half of the variable
                            //    Timer_Snapshot += Temp_Buf; // we now have all sixteen bits  

   PIE1bits.TMR1IE = 0;     // 0: disable timer 1 interrupt on overflow (i.e., a transition from FFFF->0)
                            // 1: enable timer 1 interrupt on overflow (i.e., a transition from FFFF->0)

   IPR1bits.TMR1IP = 0;     // 0: timer 1 overflow interrupt is low priority (leave at 0 on IFI controllers)
                            // 1: timer 1 overflow interrupt is high priority

   T1CONbits.TMR1ON = 1;    // 0: timer 1 is disabled
                            // 1: timer 1 is enabled (running)

   return;
}

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
