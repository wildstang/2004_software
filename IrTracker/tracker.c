/*******************************************************************************
*
*	TITLE:		tracker.c 
*
*	VERSION:	0.4 (Beta)                           
*
*	DATE:		24-Dec-2003
*
*	AUTHOR:		R. Kevin Watson
*
*	COMMENTS:	This code assumes IR sensor 1 is the left sensor on the left tracker
*				         "        IR sensor 2 is the right sensor on the left tracker
*				         "        IR sensor 3 is the left sensor on the right tracker
*				         "        IR sensor 4 is the right sensor on the right tracker
*
*				         "        left IR beacon tracking servo motor is on PWM 1
*				         "        right IR beacon tracking servo motor is on PWM 2
*
*				         "        beacon type selector switch is on digital I/O 7
*
********************************************************************************
*
*	CHANGE LOG:
*
*	DATE         REV  DESCRIPTION
*	-----------  ---  ----------------------------------------------------------
*	03-Dec-2003  0.1  RKW Original
*	07-Dec-2003  0.2  RKW - Modified Track_Beacon() to handle an arbitrary number
*	                  of IR beam tracking assemblies. All state machine
*	                  information is now kept in the Tracker_Data data structure.
*	                  Cleaned-up Track_Beacon to make it understandable, hiding
*	                  most of the tough-to-grasp code in tracker.h.
*	23-Dec-2003  0.3  RKW - Added dynamic beacon type selection.
*	24-Dec-2003  0.4  RKW - Modified the tracking state machine to guarantee a new
*	                  servo position is calculated each iteration if one or both 
*	                  sensors don't see the beacon.
*
*******************************************************************************/

#include "ifi_picdefs.h"
#include "ifi_default.h"
#include "ifi_aliases.h"
#include "printf_lib.h"
#include "user_routines.h"
#include "tracker.h"

Tracker_Data_Struct Tracker_Data[2]; // two trackers: left and right

/*******************************************************************************
*
*	FUNCTION:		Initialize_Tracker()
*
*	PURPOSE:		Initializes the infrared beacon tracker code.
*
*	CALLED FROM:	User_Initialization()
*
*	PARAMETERS:		None
*
*	RETURNS:		Nothing
*
*	COMMENTS:		Adjustable tracker parameters are located in tracker.h

*******************************************************************************/
void Initialize_Tracker(void)
{
	// initialize servo PWM channels
	LEFT_TRACKER_SERVO = INITIAL_SERVO_POSITION; // rotate left tracker fully clockwise
	RIGHT_TRACKER_SERVO = INITIAL_SERVO_POSITION; // rotate right tracker fully clockwise

	// initialize the left tracker's state machine
	Tracker_Data[LEFT].Status = NONE_IN_VIEW; // defined in tracker.h
	Tracker_Data[LEFT].Position = INITIAL_SERVO_POSITION; // defined in tracker.h
	Tracker_Data[LEFT].Search_Increment = INITIAL_SERVO_STEP_SIZE; // defined in tracker.h
	Tracker_Data[LEFT].Beacon_Type = DESIRED_BEACON_TYPE; // defined in tracker.h
	Tracker_Data[LEFT].Left_Sensor_Stats_Index = LEFT_TRACKER_LEFT_SENSOR; // defined in tracker.h
	Tracker_Data[LEFT].Right_Sensor_Stats_Index = LEFT_TRACKER_RIGHT_SENSOR; // defined in tracker.h

	// initialize the right tracker's state machine
	Tracker_Data[RIGHT].Status = NONE_IN_VIEW; // defined in tracker.h
	Tracker_Data[RIGHT].Position = INITIAL_SERVO_POSITION; // defined in tracker.h
	Tracker_Data[RIGHT].Search_Increment = INITIAL_SERVO_STEP_SIZE; // defined in tracker.h
	Tracker_Data[RIGHT].Beacon_Type = DESIRED_BEACON_TYPE; // defined in tracker.h
	Tracker_Data[RIGHT].Left_Sensor_Stats_Index = RIGHT_TRACKER_LEFT_SENSOR; // defined in tracker.h
	Tracker_Data[RIGHT].Right_Sensor_Stats_Index = RIGHT_TRACKER_RIGHT_SENSOR; // defined in tracker.h

	Initialize_Receiver();
}

/*******************************************************************************
*
*	FUNCTION:		Track_Beacon()
*
*	PURPOSE:		Controls the servo motor(s) such that the pair of
*					infrared beacon sensors mounted on it will always
*					point at the selected IR beacon. Using two or more
*					beacon trackers, separated by a known baseline
*					distance, and some trigonometry will allow the user
*					to calculate the heading and distance to the beacon.
*					See user_routines_fast.c/Process_Data_From_Local_IO()
*					for more information.
*
*	CALLED FROM:	user_routines_fast.c/Process_Data_From_Local_IO()
*
*	PARAMETERS:		unsigned char that identifies which tracker to use
*
*	RETURNS:		nothing		
*
*	COMMENTS:		There must be one Tracker_Data structure per tracker.
*					More than two trackers can be used.
*
*******************************************************************************/
void Track_Beacon(unsigned char Tracker)
{
	// still looking for the same beacon type?
	if(Tracker_Data[Tracker].Beacon_Type != DESIRED_BEACON_TYPE)
	{
		// beacon selector switch has changed
		TRACKER_STATUS = NONE_IN_VIEW;
		Tracker_Data[Tracker].Beacon_Type = DESIRED_BEACON_TYPE;
		printf("Tracker %u: Now looking for beacon type %u\n", (unsigned int)Tracker, (unsigned int)Tracker_Data[Tracker].Beacon_Type);
	}

	// which state was tracker <Tracker#> in the last time Track_Beacon() was called?
	switch(TRACKER_STATUS)
	{
		case NONE_IN_VIEW: // neither sensor sees the beacon
		{
			if(LEFT_SENSOR) // is the beacon in view of the left sensor?
			{
				// left sensor sees beacon; change state and rotate left
				TRACKER_STATUS = LEFT_IN_VIEW;
				TRACKER_POSITION -= SEARCH_INCREMENT; // rolling over is okay
				printf("Tracker %u: from none to left\n", (unsigned int)Tracker);
			}
			
			else if(RIGHT_SENSOR) // is the beacon in view of the right sensor?
			{
				// right sensor sees beacon; change state and rotate right
				TRACKER_STATUS = RIGHT_IN_VIEW;
				TRACKER_POSITION += SEARCH_INCREMENT; // rolling over is okay
				printf("Tracker %u: from none to right\n", (unsigned int)Tracker);
			}
			else
			{
				// neither sensor sees the beacon; continue searching
				TRACKER_POSITION -= SEARCH_INCREMENT; // rolling over is okay
				printf("Tracker %u: still searching\n", (unsigned int)Tracker);
			}
		}
		case LEFT_IN_VIEW: // left sensor sees beacon, but the right sensor doesn't
		{			
			if(RIGHT_SENSOR)
			{
				// found the beacon
				TRACKER_STATUS = BOTH_IN_VIEW;
				printf("Tracker %u: from left to both\n", (unsigned int)Tracker);
			}
			else
			{
				// continue to rotate left until the beacon comes into view of the right sensor
				TRACKER_POSITION -= SEARCH_INCREMENT; // rolling over is okay
			}
			break;
		}	
		case RIGHT_IN_VIEW:	// right sensor sees beacon, but the left sensor doesn't
		{
			if(LEFT_SENSOR)
			{
				// found the beacon
				TRACKER_STATUS = BOTH_IN_VIEW;
				printf("Tracker %u: from right to both\n", (unsigned int)Tracker);
			}
			else
			{
				// continue to rotate right until the beacon comes into view of the left sensor
				TRACKER_POSITION += SEARCH_INCREMENT; // rolling over is okay
			}
			break;
		}
		case BOTH_IN_VIEW: // both sensors see the beacon
		{
			if((LEFT_SENSOR) &&	(RIGHT_SENSOR)) // do both sensors still see the beacon?
			{
				// do nothing because both sensors see the beacon
				printf("Tracker %u: both\n", (unsigned int)Tracker);
			}
			else
			{
				if(LEFT_SENSOR) // is the beacon in view of the left sensor?
				{
					// left sensor sees beacon; change state and rotate left
					TRACKER_STATUS = LEFT_IN_VIEW;
					TRACKER_POSITION -= SEARCH_INCREMENT; // rolling over is okay
					printf("Tracker %u: from both to left\n", (unsigned int)Tracker);
				}
				
				else if(RIGHT_SENSOR) // is the beacon in view of the right sensor?
				{
					// right sensor sees beacon; change state and rotate right
					TRACKER_STATUS = RIGHT_IN_VIEW;
					TRACKER_POSITION += SEARCH_INCREMENT; // rolling over is okay
					printf("Tracker %u: from both to right\n", (unsigned int)Tracker);
				}
				else
				{
					// neither sensor sees the beacon; change state and start searching
					TRACKER_STATUS = NONE_IN_VIEW;
					TRACKER_POSITION -= SEARCH_INCREMENT; // rolling over is okay
					printf("Tracker %u: from both to none\n", (unsigned int)Tracker);
				}
			}
			break;
		} 
	}// end switch(TRACKER_STATUS)
}
