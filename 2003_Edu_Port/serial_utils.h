#ifndef SERIAL_UTILS_H
#define SERIAL_UTILS_H

#define RXINTF              PIR1bits.RCIF
#define RXINTE              PIE1bits.RCIE
#define TXINTF              PIR1bits.TXIF 
#define TXINTE              PIE1bits.TXIE
#define RCSTAbits           RCSTA1bits
#define RCSTA               RCSTA1
#define TXSTA               TXSTA1
#define TXREG               TXREG1
#define RCREG               RCREG1
#define SPBRG               SPBRG1
#define OpenUSART           Open1USART
typedef enum
{
  baud_19 = 15,
  baud_38 = 64,
  baud_56 = 42,
  baud_115 = 21
} SERIAL_SPEED;


void ClearRXError(void);
char GetByte(unsigned char *buf);
void PutByte(char c);
int SerRead(void *dest, int length);
void InitializeSerialComms(void);

#endif
