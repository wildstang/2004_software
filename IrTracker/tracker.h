/*******************************************************************************
*
*	TITLE:		tracker.h 
*
*	VERSION:	0.4 (Beta)                           
*
*	DATE:		24-Dec-2003
*
*	AUTHOR:		R. Kevin Watson
*
*	COMMENTS:
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
#ifndef _tracker_h
#define _tracker_h

#include "receiver.h"

// type of beacon to search for
#define DESIRED_BEACON_TYPE rc_dig_in07 // this assumes that rc_dig_in07 is a digital input

// assign sensors to the trackers
#define LEFT_TRACKER_LEFT_SENSOR 0		// entry 0 of Sensor_Stats[] will provide this beacon data
#define LEFT_TRACKER_RIGHT_SENSOR 1		// entry 1 of Sensor_Stats[] will provide this beacon data
#define RIGHT_TRACKER_LEFT_SENSOR 2		// entry 2 of Sensor_Stats[] will provide this beacon data
#define RIGHT_TRACKER_RIGHT_SENSOR 3	// entry 3 of Sensor_Stats[] will provide this beacon data

// define the PWM channels to use for the tracker servo motors
#define LEFT_TRACKER_SERVO pwm01
#define RIGHT_TRACKER_SERVO pwm02

// define the starting position of the servo motors
#define INITIAL_SERVO_POSITION 127

// define the initial servo motor step size
// this may be a negitive value for some types of servo motors 
#define INITIAL_SERVO_STEP_SIZE 3

// define the beacon types
#define BEACON_TYPE_0 0
#define BEACON_TYPE_1 1

// define the tracker identification numbers
#define LEFT 0
#define RIGHT 1

// define the tracker states
#define NONE_IN_VIEW 0
#define RIGHT_IN_VIEW 1
#define LEFT_IN_VIEW 2
#define BOTH_IN_VIEW 3

// this expression is the state of the current tracker
#define TRACKER_STATUS Tracker_Data[Tracker].Status

// this expression is the current tracker servo motor position
#define TRACKER_POSITION Tracker_Data[Tracker].Position

// this expression is the step increment for the current trackers servo motor
#define SEARCH_INCREMENT Tracker_Data[Tracker].Search_Increment

// this lengthy expression is just the number of IR beacon pulses received 
// by the current trackers left sensor in the last clock period.
#define LEFT_SENSOR Sensor_Stats[Tracker_Data[Tracker].Left_Sensor_Stats_Index].Beacon_Quality[Tracker_Data[Tracker].Beacon_Type]

// this lengthy expression is just the number of IR beacon pulses received
// by the current trackers right sensor in the last clock period.
#define RIGHT_SENSOR Sensor_Stats[Tracker_Data[Tracker].Right_Sensor_Stats_Index].Beacon_Quality[Tracker_Data[Tracker].Beacon_Type]

// function prototypes
void Initialize_Tracker(void);
void Track_Beacon(unsigned char); 

// define the tracker data structure
typedef struct tagTracker_Data
{
	unsigned char Status;					// tracker state (defined above)
	unsigned char Position;					// current commanded servo motor position
	unsigned char Search_Increment;			// servo motor search step increment
	unsigned char Beacon_Type;				// beacon type to search for
	unsigned char Left_Sensor_Stats_Index;	// which sensor entry, in sensor stats, will provide data
	unsigned char Right_Sensor_Stats_Index;	// for the left and right sensors of this tracker
}Tracker_Data_Struct;

extern Tracker_Data_Struct Tracker_Data[2];


#endif // _tracker_h
