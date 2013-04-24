/*******************************************************************************
* FILE NAME ws_serial2.c
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

#ifndef CYGWIN_IFI_SIMULATOR
/* Real target build */
#include <usart.h>
#include <string.h>
#include "delays.h"       /*defined locally*/
#include "ifi_aliases.h"
#include "ifi_default.h"
#include "ifi_utilities.h"
#include "ws_includes.h"
#include "ws_serial2.h"
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

/* These are from usart.h, but don't matter in the simulator */
#define USART_TX_INT_OFF  0
#define USART_RX_INT_OFF  0
#define USART_ASYNCH_MODE 0
#define USART_EIGHT_BIT   0
#define USART_CONT_RX     0
#define USART_BRGH_HIGH   0
#define baud_115          0
#endif

#define CC_baud_76K  32
#define CC_baud_125K 19

/* Use these defines to add more serial port support */
/*#define WS_SERIAL2_PORT2_ADDTL_SUPPORT*/  /* various print2 routines */
/*#define WS_SERIAL2_PORT1_SUPPORT*/        /* WS spec routines for port1 */

#ifdef WS_SERIAL2_PORT2_ADDTL_SUPPORT
char            ifi_printfBufr2[80];
unsigned char   *ptr2;
unsigned char   ifi_count2;
#endif


/*******************************************************************************
* FUNCTION NAME: Initialize_Serial_Comms2
* PURPOSE:       Opens the serial port 2 for communicating with your TTL port at
*                115k baud, 8 bits, no parity, one stop bit, and no flow control.
* CALLED FROM:   user_routines.c
* ARGUMENTS:     none
* RETURNS:       void
*******************************************************************************/
void Initialize_Serial_Comms2 (void)
{
    /* Received this from David Maxwell at IFI to fix the problem where we
     * were constantly getting RX packets from the TTL port even when nothing
     * was connected to it.  This will configure the RX pin as an input.  Do
     * this prior to the Open to make sure we don't get flooded with RX ints.
     */
    TRISGbits.TRISG1 = 0;
    TRISGbits.TRISG2 = 1;

  Open2USART(USART_TX_INT_OFF &
    USART_RX_INT_ON &
    USART_ASYNCH_MODE &
    USART_EIGHT_BIT &
    USART_CONT_RX &
    USART_BRGH_HIGH,
    /* baud_38 is actually 9600 when BRGH is LOW (above) */
    /*baud_38*/ CC_baud_125K /*32*/);    /* 31=80K, 32=74K* , HC08 wants 76.8K(use 32) */

    /* Configure the pins correctly again after the Open in case that call
     * sets them incorrectly.
     */
    TRISGbits.TRISG1 = 0;
    TRISGbits.TRISG2 = 1;

  Delay1KTCYx( 50 ); /* Settling time */
}

/*******************************************************************************
* FUNCTION NAME: Wait4TXEmpty2
* PURPOSE:       Wait for serial transmit buffer to be empty.
* CALLED FROM:   anywhere
* ARGUMENTS:     none
* RETURNS:       void
*******************************************************************************/
/* Used when transmitting data serially.  It waits for each byte to finish.   */
void Wait4TXEmpty2(void)
{
#ifndef _SIMULATOR
  while (!TXINTF2)
  {
    continue;
  }
#endif
}

/*******************************************************************************
* FUNCTION NAME: ClearRxBuffer2
* PURPOSE:       Read all the bytes out of the serial2 RX buffer so the buffer
*                is emptied.
* CALLED FROM:   anywhere
* ARGUMENTS:     none
* RETURNS:       void
*******************************************************************************/
void ClearRxBuffer2(void)
{
  unsigned char temp;
   while(GetRxByte2(&temp) > 0)
   {
      /* Just keep looping until GetRxByte1 returns 0, meaning there are no
       * bytes waiting to be received
       */
   }
}

/*******************************************************************************
* FUNCTION NAME: GetRxByte2
* PURPOSE:       Try to receive a byte on serial port2.  If one is available,
*                put it in p_rx_byte.
* CALLED FROM:   anywhere
* ARGUMENTS:     none
* RETURNS:       Number of bytes read (0=no byte available, 1=one byte read)
*******************************************************************************/
int GetRxByte2(unsigned char* p_rx_byte)
{
   int ret_bytes=1;  /* Default to one received byte */

   if(sp_rx2_buffer_head != sp_rx2_buffer_tail)
   {
      *p_rx_byte = *sp_rx2_buffer_tail;
      /*printf("-------------------RX2 DATA = 0x%X(err=0x%X,OR=0x%X)------------\n",
           (int)*p_rx_byte, (int)s_rx2_errors, (int)s_rx2_buffer_overrun_errors);*/
      if(sp_rx2_buffer_tail != &(s_rx2_buffer[SIZE_RX2_BUFFER-1]))
      {
        /* Increment the tail to the next byte in the buffer */ 
        sp_rx2_buffer_tail++;
      }
      else
      {
        /* Just read from the last byte, now wrap back to the byte 0 */ 
        sp_rx2_buffer_tail = &(s_rx2_buffer[0]);
      }
   }
   else
   {
      /*printf("NO SERIAL2 DATA AVAIL (RXERR=0x%X, BUFF_OR=0x%X)\n",
             s_rx2_errors, s_rx2_buffer_overrun_errors);*/
      ret_bytes = 0;  /* Indicate no bytes received */
   }
   return ret_bytes;
}

/*******************************************************************************
* FUNCTION NAME: SendSerialByte2
* PURPOSE:       Sends the passed in byte directly out serial port 2 (no ASCII conversion)

* CALLED FROM:   anywhere
* ARGUMENTS:     
*     Argument       Type        IO   Description
*     --------       --------    --   -----------
*     data           byte        I    points to beginning of buffer to transmit
* RETURNS:       void
*******************************************************************************/
void SendSerialByte2(unsigned char data)
{
  Wait4TXEmpty2();     /* Make sure TX is complete before sending new byte */
  WRITE_TO_TX2(data);  /* Send the data out serial port 2 */
  /*printf("'0x%X'",(int)data);*/
}

#ifdef WS_SERIAL2_PORT2_ADDTL_SUPPORT

/*******************************************************************************
* FUNCTION NAME: Hex_output2
* PURPOSE:       Convert the passed in byte to 2 ASCII bytes and send
*                them out serial port2.
* CALLED FROM:   anywhere
* ARGUMENTS:     none
* RETURNS:       void
*******************************************************************************/
void Hex_output2(unsigned char temp)
{
   /* A byte can be represented by 2 ASCII digits.  Each ASCII digit represents
    * 4-bits.
    */
   int i;

   /* Loop twice, once for the first 4 bits, then the second 4 bits */
   for(i=0;i<2;i++) 
   {
      int    hi_4bits;
      char   ascii_char;

      /* Only look at the high 4 bits by shifting them to the low 4 bits
       * which will zero out the high 4 bits.
       */
      hi_4bits = (int)(temp >> 4);

      /* Figure out how to display the 4-bits as an ASCII hex char */
      if(hi_4bits < 10)
      {
         /* The number is 0-9, so just add the value to ASCII '0' */
         ascii_char = '0' + hi_4bits;
      }
      else
      {
         /* The number is A-F, so just add the value to ASCII 'A' but
          * subtract off the additional 10.  Subtract it off the 'A' so
          * it will happen at compile time instead of run-time
          */
         ascii_char = ('A'-10) + hi_4bits;
      }

      /* Now print out our ASCII char */
      Wait4TXEmpty2();
      WRITE_TO_TX2(ascii_char);

      /* Shift the low 4-bits up to the high 4-bits and decode again */
      temp = temp << 4;
   }
}

/*******************************************************************************
* FUNCTION NAME: PrintByte2
* PURPOSE:       A simple way to print a byte of data to the serial port.
* CALLED FROM:   anywhere
* ARGUMENTS:     none
*     Argument       Type             IO   Description
*     --------       -------------    --   -----------
*     odata          unsigned char    I    byte of data to be transmitted
* RETURNS:       void
*******************************************************************************/
void PrintByte2(unsigned char odata)
{
  Hex_output2((unsigned char) odata);
  Wait4TXEmpty2(); /* Make sure TX is complete before sending new byte */
  WRITE_TO_TX2(13);  /* a carriage return */
}


/*******************************************************************************
* FUNCTION NAME: PrintWord2
* PURPOSE:       A simple way to print a word of data to the serial port.
* CALLED FROM:   anywhere
* ARGUMENTS:     none
*     Argument       Type             IO   Description
*     --------       -------------    --   -----------
*     odata          unsigned int     I    word of data to be transmitted
* RETURNS:       void
*******************************************************************************/
void PrintWord2(unsigned int odata)
{
  ptr2 = (unsigned char *) &odata;
  Hex_output2(ptr2[1]);
  Hex_output2(ptr2[0]);
  Wait4TXEmpty2(); /* Make sure TX is complete before sending new byte */
  WRITE_TO_TX2(13);  /* add a carriage return */
}


/*******************************************************************************
* FUNCTION NAME: PrintString2
* PURPOSE:       Prints a string to the serial port.
* CALLED FROM:   anywhere
* ARGUMENTS:     none
*     Argument       Type             IO   Description
*     --------       -------------    --   -----------
*     bufr           pointer          I    word of data to be transmitted
* RETURNS:       void
*******************************************************************************/
void PrintString2(char *bufr)
{
  static int len,I;

  strcpypgm2ram (ifi_printfBufr2,(rom char *) bufr); /*Move from flash to ram*/
  len = (int) strlen((const char *)ifi_printfBufr2);
  if (len > 80) len = 80;

  for (I=0;I<len;I++)
  {
    Wait4TXEmpty2(); /* Make sure TX is complete before sending new byte */
    WRITE_TO_TX2(ifi_printfBufr2[I]);
  }
}


/*******************************************************************************
* FUNCTION NAME: DisplayBufr2
* PURPOSE:       Print the entire transmit or receive buffer over the serial 
*                port for viewing in a terminal program on your PC.
* CALLED FROM:   anywhere
* ARGUMENTS:     
*     Argument       Type        IO   Description
*     --------       --------    --   -----------
*     *bufr          pointer     I    points to beginning of buffer to transmit
* RETURNS:       void
*******************************************************************************/
void DisplayBufr2(unsigned char *bufr)
{
  for (ifi_count2=0;ifi_count2<26;ifi_count2++)
  {
    Hex_output2((unsigned char) *bufr++);
  }
  Wait4TXEmpty2(); /* Make sure TX is complete before sending new byte */
  WRITE_TO_TX2(13);  /* add a carriage return */
}

#endif /*WS_SERIAL2_PORT2_ADDTL_SUPPORT*/

#ifdef WS_SERIAL2_PORT1_SUPPORT

/*******************************************************************************
* FUNCTION NAME: ClearRxBuffer1
* PURPOSE:       Read all the bytes out of the serial1 RX buffer so the buffer
*                is emptied.
* CALLED FROM:   anywhere
* ARGUMENTS:     none
* RETURNS:       void
*******************************************************************************/
void ClearRxBuffer1(void)
{
  unsigned char temp;
   while(GetRxByte1(&temp) > 0)
   {
      /* Just keep looping until GetRxByte1 returns 0, meaning there are no
       * bytes waiting to be received
       */
   }
}

/*******************************************************************************
* FUNCTION NAME: SendSerialByte1
* PURPOSE:       Sends the passed in byte directly out serial port 1 (no ASCII conversion)

* CALLED FROM:   anywhere
* ARGUMENTS:     
*     Argument       Type        IO   Description
*     --------       --------    --   -----------
*     data           byte        I    points to beginning of buffer to transmit
* RETURNS:       void
*******************************************************************************/
void SendSerialByte1(unsigned char data)
{
  Wait4TXEmpty1();     /* Make sure TX is complete before sending new byte */
  WRITE_TO_TX1(data);  /* Send the data out serial port 2 */
}

/*******************************************************************************
* FUNCTION NAME: GetRxByte1
* PURPOSE:       Try to receive a byte on serial port1.  If one is available,
*                put it in p_rx_byte.
* CALLED FROM:   anywhere
* ARGUMENTS:     none
* RETURNS:       Number of bytes read (0=no byte available, 1=one byte read)
*******************************************************************************/
int GetRxByte1(unsigned char* p_rx_byte)
{
   int ret_bytes=1;  /* Default to one received byte */

   if(sp_rx1_buffer_head != sp_rx1_buffer_tail)
   {
      *p_rx_byte = *sp_rx1_buffer_tail;
      /*printf("-------------------RX1 DATA = 0x%X------------\n",(int)ret_byte);*/
      if(sp_rx1_buffer_tail != &(s_rx1_buffer[SIZE_RX1_BUFFER-1]))
      {
        /* Increment the tail to the next byte in the buffer */ 
        sp_rx1_buffer_tail++;
        /*printf("INC1\n");*/
      }
      else
      {
        /* Just read from the last byte, now wrap back to the byte 0 */ 
        sp_rx1_buffer_tail = &(s_rx1_buffer[0]);
        /*printf("WRAP1\n");*/
      }
   }
   else
   {
      /*printf("NO SERIAL1 DATA AVAIL (RXERR=0x%X, BUFF_OR=0x%X)\n",
             s_rx1_errors, s_rx1_buffer_overrun_errors);*/
      ret_bytes = 0;  /* Indicate no bytes received */
   }
   return ret_bytes;
}

#endif  /* PORT1 SUPPORT */

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
