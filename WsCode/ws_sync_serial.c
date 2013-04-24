/*******************************************************************************
* FILE NAME ws_sync_serial.c
*
* DESCRIPTION:
*  Implements functions to communicate with a sync serial device  
*
* USAGE:
*
*******************************************************************************/

#ifndef CYGWIN_IFI_SIMULATOR
/* Real target build */
#include <usart.h>
#include <string.h>
#include "delays.h"
#include "ifi_aliases.h"
#include "ifi_default.h"
#include "ifi_utilities.h"
#include "ws_includes.h"
#include "ws_sync_serial.h"
#include "user_routines.h"
#include "printf_lib.h"
#else
/* Simulator build */
#include "RC_Simulator.h"
#include "ifi_aliases.h"
#include "ifi_default.h"
#include "ws_includes.h"
#include "ws_serial2.h"
#include <string.h>
#endif

/* Delays:
 *   :b2
 *    Delay10TCY() =~   1us
 *    DelayKTCY(0) =~   5us?
 *    Delay10KTCY(0) =~ long...sec?
 */
#define SYNC_SERIAL_CLOCK_DELAY() Delay10TCY()
#define SYNC_SERIAL_CLOCK      rc_dig_out03
#define SYNC_SERIAL_CLOCK_DIR  digital_io_03 

#define SYNC_SERIAL_SEND_RECV_DELAY(){int i;for(i=0;i<50;i++)Delay10TCY();} /*340us*/
#define SYNC_SERIAL_DATA_OUT  rc_dig_out04
#define SYNC_SERIAL_DATA_IN   rc_dig_in04
#define SYNC_SERIAL_DATA_DIR  digital_io_04    /* Set to OUTPUT or INPUT to control
                                               * if rc_dig_16 is an input or output
                                               */
/*******************************************************************************
* FUNCTION NAME: ReadSyncSerialData
* PURPOSE:       Read a register on the device connected to the sync serial port.
* CALLED FROM:   anywhere
* ARGUMENTS:
*     Argument       Type        IO   Description
*     --------       --------    --   -----------
*     addr           byte        I    Register to be read
* RETURNS:       Data read from the register
*******************************************************************************/
unsigned char ReadSyncSerialData(unsigned char addr)
{
  unsigned char data;  
 
  /* For a read operation, the part will change the data on a falling edge of clock
   * and we should read the data on rising edge of clock.
   * Also, the MSB of the address must be '0' to indicate a write.
   */
  SYNC_SERIAL_CLOCK = 0;   /* Make sure the mouse is HighZ before making data and output*/
  SYNC_SERIAL_DATA_DIR = OUTPUT;  /* SYNC_SERIAL_DATA bit is now an output */
  SYNC_SERIAL_CLOCK_DIR = OUTPUT;

  /* Send the address to be read out to the part */
  SendSyncSerialByte(addr & 0x7F); /* <- The MSB has to be 0 (indicates read) */

  /* Must wait  100 us to read data */
  SYNC_SERIAL_DATA_DIR = INPUT;  /* SYNC_SERIAL_DATA bit is now an input */
  SYNC_SERIAL_SEND_RECV_DELAY(); /* Must wait at least 100us sec between send/recv */
  
  /* Now get the response from the part */
  data = RecvSyncSerialByte();
  
  SYNC_SERIAL_CLOCK = 0;   /* This last transition puts the part in HighZ */

  return(data);  
}
       
/*
*****************************************************************************
* FUNCTION NAME: WriteSyncSerialData
* PURPOSE:       Sends the passed in address and data to the device connected
*                to the sync serial port.
*
* CALLED FROM:   anywhere
* ARGUMENTS:     
*     Argument       Type        IO   Description
*     --------       --------    --   -----------
*     addr           byte        I    Register to be written
*     data           byte        I    Data to be written to the register
* RETURNS:       void
*******************************************************************************/
void WriteSyncSerialData(unsigned char addr, unsigned char data)
{   
  SYNC_SERIAL_CLOCK = 0;   /* Make sure the mouse is HighZ before making data and output*/
  SYNC_SERIAL_DATA_DIR = OUTPUT;  /* SYNC_SERIAL_DATA bit is now an output */
  SYNC_SERIAL_CLOCK_DIR = OUTPUT;
 
  /* For a write operation, the part will read the data on a rising edge of clock.
   * Also, the MSB of the address must be '1' to indicate a write.
   */

  /* First write the address byte to the part */
  SendSyncSerialByte(addr | 0x80); /* <- Sets the MSB to 1 (indicates write) */ 
 
  /* Now write the data to the part */
  SendSyncSerialByte(data);
}

/*******************************************************************************
* FUNCTION NAME: RecvSyncSerialByte
* PURPOSE:       Receive a byte from the emulated sync serial port
* CALLED FROM:   anywhere
* ARGUMENTS:     none
* RETURNS:       void
*******************************************************************************/
unsigned char RecvSyncSerialByte(void)
{
  unsigned char data=0;  
  int i;

  /* Recv all the bits in the data, one at a time */  
  for(i = 0; i < 8; i++)
  {   
    /* Bring the clock low to tell the part to change the data */
    SYNC_SERIAL_CLOCK = 0; 
    SYNC_SERIAL_CLOCK_DELAY();
  
    /* Bring the clock high to tell the part we are reading the data */
    SYNC_SERIAL_CLOCK = 1; 

    SYNC_SERIAL_CLOCK_DELAY();

    /* Shift up previous data then OR in the new LSB data bit */
    data = data << 1;
    data = data & 0xFE;
    data = data | (SYNC_SERIAL_DATA_IN & 0x01);
    SYNC_SERIAL_CLOCK_DELAY();
  }

  SYNC_SERIAL_CLOCK = 0;   /* This last transition puts the part in HighZ */

  return(data);  
}

/*******************************************************************************
* FUNCTION NAME: SendSyncSerialByte
* PURPOSE:       Sends out passed in byte out the sync serial port
* CALLED FROM:   anywhere
* ARGUMENTS:
*     Argument       Type        IO   Description
*     --------       --------    --   -----------
*     data           byte        I    Byte to send
* RETURNS:       void
*******************************************************************************/
void SendSyncSerialByte(unsigned char data)
{
  int i;

  /* Loop through all the bits in the data, one at a time */  
  for(i = 7; i >= 0; i--)
  {
    /* Bring to clock low so we can change the data */
    SYNC_SERIAL_CLOCK = 0;

    /* Send the bits out one at a time, MSB bit first */
    SYNC_SERIAL_DATA_OUT = (data >> i ) & 0x01;
    SYNC_SERIAL_CLOCK_DELAY();
    
    /* Rising edge of clock causes the part to read the data */
    SYNC_SERIAL_CLOCK = 1;
    SYNC_SERIAL_CLOCK_DELAY();
  }
  return;
}

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
