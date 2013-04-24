/*******************************************************************************
* FILE NAME: eeprom.h
*
* DESCRIPTION:
*  This is the include file which corresponds to eeprom.c
*
*  It contains some aliases and function prototypes used in those files.
*
* USAGE:
*  If you add your own routines to those files, this is a good place to add
*  your custom macros (aliases), type definitions, and function prototypes.
*******************************************************************************/

#ifndef __eeprom_h_
#define __eeprom_h_

/******************************* TYPEDEFS *************************************/

/******************************** MACROS **************************************/

/***************************** DEFINITIONS ************************************/
#define ADDR_KNOWN_BYTE1          0
#define ADDR_KNOWN_BYTE2          1

#define ADDR_DATA_BITMASK_CRAB    2
#define ADDR_DATA_BITMASK_ARMS    3
#define ADDR_DATA_BITMASK3        4
#define ADDR_DATA_BITMASK4        5

#define ADDR_CRAB_FWD             10
#define ADDR_CRAB_180_LEFT        11
#define ADDR_CRAB_180_RIGHT       12

#define ADDR_B_ARM_SHOULDER_HI    13
#define ADDR_B_ARM_SHOULDER_LO    14
#define ADDR_B_ARM_ELBOW_HI       15
#define ADDR_B_ARM_ELBOW_LO       16
#define ADDR_S_ARM_SHOULDER_HI    17
#define ADDR_S_ARM_SHOULDER_LO    18


typedef enum
{
  EE_EMPTY,
  EE_NOT_EMPTY
} EE_QUEUE_STATE;

/****************************** STRUCTURES ************************************/
typedef struct {
  unsigned writeInProgress:1;
  unsigned bufferNotEmpty:1;
  unsigned bufferFull:1;
  unsigned :5;
  unsigned bufferPtr:4;
  unsigned bufferEnd:4;
  unsigned char dataBuffer[16];
  unsigned short addressBuffer[16];
} eeControlStruct;


/************************* FUNCTION PROTOTYPES ********************************/
extern unsigned char readEE(unsigned short);
extern void writeEE(unsigned short, unsigned char);
extern void processEEQueue(void);
extern UINT8 checkEEQueue(void);

#endif /* __eeprom_h_ */

