#include <usart.h>
#include "serial_utils.h"
#include "delays.h"
#include "oi_data.h"


/*******************************************************************************
* FUNCTION NAME: ClearRXError
* PURPOSE:       Clear any serial receiver errors that are present
* ARGUMENTS:     none
* RETURNS:       void
*******************************************************************************/
void ClearRXError(void)
{
    volatile unsigned char temp = RCREG;
    
    RCSTA1bits.CREN = 0;
    RCSTA1bits.CREN = 1;
}

/*******************************************************************************
* FUNCTION NAME: Wait4TXEmpty
* PURPOSE:       Wait for serial transmit buffer to be empty.
* CALLED FROM:   anywhere
* ARGUMENTS:     none
* RETURNS:       void
*******************************************************************************/
/* Used when transmitting data serially.  It waits for each byte to finish.   */
void Wait4TXEmpty(void)
{
#ifndef _SIMULATOR
  while (!TXINTF)
  {
    continue;
  }
#endif
}

/*******************************************************************************
* FUNCTION NAME: Initialize_Serial_Comms
* PURPOSE:       Opens the serial port 1 for communicating with your PC at
*                115k baud, 8 bits, no parity, one stop bit, and no flow control.
* CALLED FROM:   user_routines.c
* ARGUMENTS:     none
* RETURNS:       void
*******************************************************************************/
void InitializeSerialComms(void)
{
    OpenUSART(USART_TX_INT_OFF &
            USART_RX_INT_OFF &
            USART_ASYNCH_MODE &
            USART_EIGHT_BIT &
            USART_CONT_RX &
            USART_BRGH_HIGH,
            baud_115);   

    Delay1KTCYx(50); /* Settling time */
}

/*******************************************************************************
* FUNCTION NAME: GetByte
* PURPOSE:       Receive a byte from the serial port
* ARGUMENTS:     buf: pointer to memory in which to store received byte
* RETURNS:       1 if successful, 0 if timeout occurs
*******************************************************************************/
char GetByte(unsigned char *buf)
{
    int i = 0;
    
    /* Wait until the receive buffer has a character or timeout occurs */
    while(PIR1bits.RC1IF == 0)
    {
        if(i > 2000)
        {
            /* Timed out waiting for data */
            return 0;
        }
        i++;
    }

    /* Return the byte in the receive buffer */    
    *buf = RCREG;
    return 1;
}

/*******************************************************************************
* FUNCTION NAME: PutByte
* PURPOSE:       Sends a byte out the serial port
* ARGUMENTS:     c: byte to send
* RETURNS:       void
*******************************************************************************/
void PutByte(char c)
{
    TXREG = c;
    Wait4TXEmpty();
}

/*******************************************************************************
* FUNCTION NAME: SerRead
* PURPOSE:       Attempt to read up to 'length' bytes from serial port
* ARGUMENTS:     dest: pointer to memory to store the data received
*                length: number of bytes to attempt to read
* RETURNS:       number of bytes read
*******************************************************************************/
int SerRead(void *dest, int length)
{
    unsigned char *buf = dest;
    int i;
    
    for(i = 0; i < length; i++)
    {
        if(GetByte(&(buf[i])) == 0)
        {
            /* GetByte timed out */
            return i;
        }
    }

    /* Return number of bytes read */    
    return i;
}
