/*******************************************************************************
*
*	TITLE:		user_routines_fast.c 
*
*	VERSION:	0.4 (Beta)                           
*
*	DATE:		22-Dec-2003
*
*	AUTHOR:		R. Kevin Watson
*				kevinw@jpl.nasa.gov
*
*	COMMENTS:	This file is a drop-in replacement for innovation
*				first's user_routines_fast.c file.
*
*				If you use timer 0, you must compile your code
*				with ifi_alltimers.lib instead of ifi_library.lib.
*
*				Numbers within brackets refer to the PIC18F8520
*				data sheet page number where more information can
*				be found.
*
*				This code is specific to the full-size FRC robot controller.
*
********************************************************************************
*
*	CHANGE LOG:
*
*	DATE         REV  DESCRIPTION
*	-----------  ---  ----------------------------------------------------------
*	08-Dec-2003  0.1  RKW Original
*	10-Dec-2003  0.2  RKW -Modified for use on the full-size FRC robot controller
*	15-Dec-2003  0.3  RKW - Changed "interrupt" pragma on line 90 to "interruptlow"
*	22-Dec-2003  0.4  RKW - Fixed bug where timer 3/4 interrupts wound never be
*	                  handled because the wrong interrupt flags were used.
*
*******************************************************************************/
#include "ifi_aliases.h"
#include "ifi_default.h"
#include "ifi_utilities.h"
#include "user_routines.h"
#include "tracker.h"

/*******************************************************************************
*
*	FUNCTION:		InterruptVectorLow()
*
*	PURPOSE:		Installs the low priority interrupt code at the low
*					priority interrupt vector, which is a fixed place in
*					memory where the microcontroller will start executing
*					code when it detects an interrupt condition. Because
*					this place in memory, at address 24/0x18, is intended 
*					to contain only a very small amount of code, general
*					practice is to place a "goto" instruction here that
*					will point to the real interrupt handler somewhere else
*					in memory. More information on interrupts can be found
*					in section nine [87] of the PIC18F8520 data sheet.
* 
*	CALLED FROM:	Called in response to a hardware generated interrupt
*
*	PARAMETERS:		None
*
*	RETURNS:		Nothing
* 
*******************************************************************************/
#pragma code InterruptVectorLow = LOW_INT_VECTOR

void InterruptVectorLow (void)
{
  _asm
    goto InterruptHandlerLow  /*jump to interrupt routine*/
  _endasm
}

#pragma code

/*******************************************************************************
*
*	FUNCTION:		InterruptHandlerLow()
*
*	PURPOSE:		Determines which individual interrupt handler
*					should be called, clears the interrupt flag and
*					then calls the interrupt handler.
* 
*	CALLED FROM:	InterruptVectorLow()
*
*	PARAMETERS:		None
*
*	RETURNS:		Nothing
* 
*******************************************************************************/
#pragma interruptlow InterruptHandlerLow

void InterruptHandlerLow ()     
{                               
	unsigned char Port_B;
	unsigned char Port_B_Delta;       
	static unsigned char Old_Port_B = 0xFF; // state of port b the last time
											// this function was called  
	
	if (INTCONbits.TMR0IF) // timer 0 Interrupt
	{
		INTCONbits.TMR0IF = 0; // clear the interrupt flag
		// if used, call the timer 0 interrupt handler here
	}

	else if (PIR1bits.TMR1IF) // timer 1 interrupt - used by the IR receiver
	{
		PIR1bits.TMR1IF = 0; // clear the interrupt flag
		Timer_1_Int_Handler(); // call the timer 1 interrupt handler (in receiver.c)
	}  

	else if (INTCON3bits.INT2IF) // external interrupt 1
	{ 
		INTCON3bits.INT2IF = 0; // clear the interrupt flag
		// if used, call the interrupt 1 handler here
	}

	else if (INTCON3bits.INT3IF) // external interrupt 2
	{
		INTCON3bits.INT3IF = 0; // clear the interrupt flag
		// if used, call the interrupt 2 handler here
	}

	else if (INTCONbits.RBIF) // external interrupts 3 through 6
	{
		Port_B = PORTB; // remove the "mismatch condition" by reading port b            
		INTCONbits.RBIF = 0; // clear the interrupt flag
		Port_B_Delta = Port_B ^ Old_Port_B; // determine which bits have changed
		Old_Port_B = Port_B; // save a copy of port b for next time around
	 
		if(Port_B_Delta & 0x10) // did external interrupt 3 change state? - IR sensor 1
		{
			Int_3_Handler(Port_B & 0x10 ? 1 : 0); // call the interrupt 3 handler (in receiver.c)
		}

		if(Port_B_Delta & 0x20) // did external interrupt 4 change state? - IR sensor 2
		{
			Int_4_Handler(Port_B & 0x20 ? 1 : 0); // call the interrupt 4 handler (in receiver.c)
		}

		if(Port_B_Delta & 0x40) // did external interrupt 5 change state? - IR sensor 3
		{
			Int_5_Handler(Port_B & 0x40 ? 1 : 0); // call the interrupt 5 handler (in receiver.c)
		}
		
		if(Port_B_Delta & 0x80) // did external interrupt 6 change state? - IR sensor 4
		{
			Int_6_Handler(Port_B & 0x80 ? 1 : 0); // call the interrupt 6 handler (in receiver.c)

		}
	}
}

/*******************************************************************************
*
*	FUNCTION:		User_Autonomous_Code()
*
*	PURPOSE:		Executes user's code during autonomous robot operation.
* 					You should modify this routine by adding code which you
*					wish to run in autonomous mode. This executes every 26.2ms.
*
*	CALLED FROM:	main.c/main()
*
*	PARAMETERS:		None
*
*	RETURNS:		Nothing
* 
*******************************************************************************/
void User_Autonomous_Code(void)
{
  while(autonomous_mode)
  {
    if(statusflag.NEW_SPI_DATA)
    {
        Getdata(&rxdata);   // bad things will happen if you move or delete this

        // autonomous code goes here

        Putdata(&txdata);   //even more bad things will happen if you mess with this
    }
  }
}

/*******************************************************************************
*
*	FUNCTION:		Process_Data_From_Local_IO()
*
*	PURPOSE:		Execute real-time code here. Code located here will
*					execute continuously as opposed to code located in 
*					user_routines.c/Process_Data_From_Master_uP(), which
*					only executes when the master processor	sends a new 
*					data packet.
* 
*	CALLED FROM:	main.c/main()
*
*	PARAMETERS:		None
*
*	RETURNS:		Nothing
* 
*******************************************************************************/
void Process_Data_From_Local_IO(void)
{
	static unsigned int Old_Clock = 0;

	if (Clock > Old_Clock) // stay synchronized to beacon data updates (this is important)
	{
		Track_Beacon(LEFT); // allow the left beacon tracker state machine to run
		Track_Beacon(RIGHT); // allow the right beacon tracker state machine to run
		
		if ((Tracker_Data[LEFT].Status == BOTH_IN_VIEW) &&
			(Tracker_Data[RIGHT].Status == BOTH_IN_VIEW))
		{
			// both trackers have an angular solution; calculate beacon vector
			// here using the known baseline distance between the trackers and
			// angles derived from the two Tracker_Data[].Position variables

			// calculate new drive motor PWM values here

		}
	// finally, update the servo motor positions
	LEFT_TRACKER_SERVO = Tracker_Data[LEFT].Position;
	RIGHT_TRACKER_SERVO = Tracker_Data[RIGHT].Position;
	
	Old_Clock = Clock; // take a snapshot of the clock
	} 

}
