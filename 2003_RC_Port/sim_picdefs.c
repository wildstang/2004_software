/*******************************************************************************
* Definitions of variables from ifi_picdef.h for the Simulator build.
* This file in essense creates memory for all the registers in the
* processor.
*******************************************************************************/

/* Undefine near and far since they don't make sense for WIN32 */
#define near  
#define far

/* 'short long' does not exist with gcc, so just use an in */
typedef signed int SINT24;
typedef unsigned int UINT24;


volatile near unsigned char       RCSTA2;
volatile near union {
  struct {
    unsigned RX9D:1;
    unsigned OERR:1;
    unsigned FERR:1;
    unsigned ADEN:1;
    unsigned CREN:1;
    unsigned SREN:1;
    unsigned RX9:1;
    unsigned SPEN:1;
  };
  struct {
    unsigned RCD8:1;
    unsigned :5;
    unsigned RC9:1;
  };
  struct {
    unsigned :6;
    unsigned NOT_RC8:1;
  };
  struct {
    unsigned :6;
    unsigned RC8_9:1;
  };
} RCSTA2bits;
volatile near unsigned char       TXSTA2;
volatile near union {
  struct {
    unsigned TX9D:1;
    unsigned TRMT:1;
    unsigned BRGH:1;
    unsigned :1;
    unsigned SYNC:1;
    unsigned TXEN:1;
    unsigned TX9:1;
    unsigned CSRC:1;
  };
  struct {
    unsigned TXD8:1;
    unsigned :5;
    unsigned TX8_9:1;
  };
  struct {
    unsigned :6;
    unsigned NOT_TX8:1;
  };
} TXSTA2bits;
volatile near unsigned char       TXREG2;
volatile near unsigned char       RCREG2;
volatile near unsigned char       SPBRG2;
volatile near unsigned char       CCP5CON;
volatile near union {
  struct {
    unsigned CCP5M0:1;
    unsigned CCP5M1:1;
    unsigned CCP5M2:1;
    unsigned CCP5M3:1;
    unsigned DCCP5Y:1;
    unsigned DCCP5X:1;
  };
  struct {
    unsigned :4;
    unsigned DC5B0:1;
    unsigned DC5B1:1;
  };
} CCP5CONbits;
volatile near unsigned            CCPR5;
volatile near unsigned char       CCPR5L;
volatile near unsigned char       CCPR5H;
volatile near unsigned char       CCP4CON;
volatile near union {
  struct {
    unsigned CCP4M0:1;
    unsigned CCP4M1:1;
    unsigned CCP4M2:1;
    unsigned CCP4M3:1;
    unsigned DCCP4Y:1;
    unsigned DCCP4X:1;
  };
  struct {
    unsigned :4;
    unsigned DC4B0:1;
    unsigned DC4B1:1;
  };
} CCP4CONbits;
volatile near unsigned            CCPR4;
volatile near unsigned char       CCPR4L;
volatile near unsigned char       CCPR4H;
volatile near unsigned char       T4CON;
volatile near struct {
  unsigned T4CKPS0:1;
  unsigned T4CKPS1:1;
  unsigned TMR4ON:1;
  unsigned T4OUTPS0:1;
  unsigned T4OUTPS1:1;
  unsigned T4OUTPS2:1;
  unsigned T4OUTPS3:1;
} T4CONbits;
volatile near unsigned char       PR4;
volatile near unsigned char       TMR4;
volatile near unsigned char       PORTA;
volatile near union {
  struct {
    unsigned RA0:1;
    unsigned RA1:1;
    unsigned RA2:1;
    unsigned RA3:1;
    unsigned RA4:1;     /* Reserved - Do not use */
    unsigned RA5:1;
    unsigned RA6:1;
  };
  struct {
    unsigned AN0:1;
    unsigned AN1:1;
    unsigned AN2:1;
    unsigned AN3:1;
    unsigned T0CKI:1;   /* Reserved - Do not use */
    unsigned AN4:1;
    unsigned OSC2:1;
  };
  struct {
    unsigned :2;
    unsigned VREFM:1;
    unsigned VREFP:1;
    unsigned :1;
    unsigned LVDIN:1;
    unsigned CLKO:1;
  };
} PORTAbits;
volatile near unsigned char       PORTB;
volatile near union {
  struct {
    unsigned RB0:1;     /* Reserved - Do not use */
    unsigned RB1:1;
    unsigned RB2:1;
    unsigned RB3:1;
    unsigned RB4:1;
    unsigned RB5:1;
    unsigned RB6:1;
    unsigned RB7:1;
  };
  struct {
    unsigned INT0:1;    /* Reserved - Do not use */
    unsigned INT1:1;
    unsigned INT2:1;
    unsigned INT3:1;
    unsigned KBI0:1;
    unsigned KBI1:1;
    unsigned KBI2:1;
    unsigned KBI3:1;
  };
  struct {
    unsigned :3;
    unsigned CCP2B:1;
    unsigned :1;
    unsigned PGM:1;
    unsigned PGC:1;
    unsigned PGD:1;
  };
} PORTBbits;
volatile near unsigned char       PORTC;
volatile near union {
  struct {
    unsigned RC0:1;
    unsigned RC1:1;     /* Reserved - Do not use */
    unsigned RC2:1;     /* Reserved - Do not use */
    unsigned RC3:1;     /* Reserved - Do not use */
    unsigned RC4:1;     /* Reserved - Do not use */
    unsigned RC5:1;     /* Reserved - Do not use */
    unsigned RC6:1;
    unsigned RC7:1;
  };
  struct {
    unsigned T1OSO:1;
    unsigned T1OSI:1;   /* Reserved - Do not use */
    unsigned CCP1:1;    /* Reserved - Do not use */
    unsigned SCK:1;     /* Reserved - Do not use */
    unsigned SDI:1;     /* Reserved - Do not use */
    unsigned SDO:1;     /* Reserved - Do not use */
    unsigned TX:1;
    unsigned RX:1;
  };
  struct {
    unsigned T13CKI:1;
    unsigned CCP2C:1;   /* Reserved - Do not use */
    unsigned :1;
    unsigned SCL:1;     /* Reserved - Do not use */
    unsigned SDA:1;     /* Reserved - Do not use */
    unsigned :1;        /* Reserved - Do not use */
    unsigned CK:1;
    unsigned DT:1;
  };
} PORTCbits;
volatile near unsigned char       PORTD;
volatile near union {
  struct {
    unsigned RD0:1;
    unsigned RD1:1;
    unsigned RD2:1;
    unsigned RD3:1;
    unsigned RD4:1;
    unsigned RD5:1;
    unsigned RD6:1;
    unsigned RD7:1;
  };
  struct {
    unsigned PSP0:1;
    unsigned PSP1:1;
    unsigned PSP2:1;
    unsigned PSP3:1;
    unsigned PSP4:1;
    unsigned PSP5:1;
    unsigned PSP6:1;
    unsigned PSP7:1;
  };
  struct {
    unsigned AD0:1;
    unsigned AD1:1;
    unsigned AD2:1;
    unsigned AD3:1;
    unsigned AD4:1;
    unsigned AD5:1;
    unsigned AD6:1;
    unsigned AD7:1;
  };
} PORTDbits;
volatile near unsigned char       PORTE;
volatile near union {
  struct {
    unsigned RE0:1;
    unsigned RE1:1;
    unsigned RE2:1;
    unsigned RE3:1;
    unsigned RE4:1;
    unsigned RE5:1;
    unsigned RE6:1;
    unsigned RE7:1;
  };
  struct {
    unsigned RD:1;
    unsigned WR:1;
    unsigned CS:1;
    unsigned :4;
    unsigned CCP2E:1;
  };
  struct {
    unsigned AD8:1;
    unsigned AD9:1;
    unsigned AD10:1;
    unsigned AD11:1;
    unsigned AD12:1;
    unsigned AD13:1;
    unsigned AD14:1;
    unsigned AD15:1;
  };
} PORTEbits;
volatile near unsigned char       PORTF;
volatile near union {
  struct {
    unsigned RF0:1;
    unsigned RF1:1;
    unsigned RF2:1;
    unsigned RF3:1;
    unsigned RF4:1;
    unsigned RF5:1;
    unsigned RF6:1;
    unsigned RF7:1;         /* Reserved - Do not use */
  };
  struct {
    unsigned AN5:1;
    unsigned AN6:1;
    unsigned AN7:1;
    unsigned AN8:1;
    unsigned AN9:1;
    unsigned AN10:1;
    unsigned AN11:1;
    unsigned SS:1;          /* Reserved - Do not use */
  };
  struct {
    unsigned :1;
    unsigned C2OUTF:1;
    unsigned C1OUTF:1;
    unsigned :2;
    unsigned CVREFF:1;     /* Reserved - Do not use */
  };
} PORTFbits;
volatile near unsigned char       PORTG;
volatile near union {
  struct {
    unsigned RG0:1;
    unsigned RG1:1;
    unsigned RG2:1;
    unsigned RG3:1;
    unsigned RG4:1;
  };
  struct {
    unsigned CCP3:1;
    unsigned TX2:1;
    unsigned RX2:1;
    unsigned CCP4:1;
    unsigned CCP5:1;
  };
  struct {
    unsigned :1;
    unsigned CK2:1;
    unsigned DT2:1;
  };
} PORTGbits;
volatile near unsigned char       PORTH;
volatile near union {
  struct {
    unsigned RH0:1;
    unsigned RH1:1;
    unsigned RH2:1;
    unsigned RH3:1;
    unsigned RH4:1;
    unsigned RH5:1;
    unsigned RH6:1;
    unsigned RH7:1;
  };
  struct {
    unsigned A16:1;
    unsigned A17:1;
    unsigned A18:1;
    unsigned A19:1;
    unsigned AN12:1;
    unsigned AN13:1;
    unsigned AN14:1;
    unsigned AN15:1;
  };
} PORTHbits;
volatile near unsigned char       PORTJ;
volatile near union {
  struct {
    unsigned RJ0:1;
    unsigned RJ1:1;
    unsigned RJ2:1;
    unsigned RJ3:1;
    unsigned RJ4:1;
    unsigned RJ5:1;
    unsigned RJ6:1;
    unsigned RJ7:1;
  };
  struct {
    unsigned ALE:1;
    unsigned OE:1;
    unsigned WRL:1;
    unsigned WRH:1;
    unsigned BA0:1;
    unsigned CE:1;
    unsigned LB:1;
    unsigned UB:1;
  };
} PORTJbits;
volatile near unsigned char       LATA;
volatile near struct {
  unsigned LATA0:1;
  unsigned LATA1:1;
  unsigned LATA2:1;
  unsigned LATA3:1;
  unsigned LATA4:1;     /* Reserved - Do not use */
  unsigned LATA5:1;
  unsigned LATA6:1;
} LATAbits;
volatile near unsigned char       LATB;
volatile near struct {
  unsigned LATB0:1;     /* Reserved - Do not use */
  unsigned LATB1:1;
  unsigned LATB2:1;
  unsigned LATB3:1;
  unsigned LATB4:1;
  unsigned LATB5:1;
  unsigned LATB6:1;
  unsigned LATB7:1;
} LATBbits;
volatile near unsigned char       LATC;
volatile near struct {
  unsigned LATC0:1;
  unsigned LATC1:1;     /* Reserved - Do not use */
  unsigned LATC2:1;     /* Reserved - Do not use */
  unsigned LATC3:1;     /* Reserved - Do not use */
  unsigned LATC4:1;     /* Reserved - Do not use */
  unsigned LATC5:1;     /* Reserved - Do not use */
  unsigned LATC6:1;
  unsigned LATC7:1;
} LATCbits;
volatile near unsigned char       LATD;
volatile near struct {
  unsigned LATD0:1;
  unsigned LATD1:1;
  unsigned LATD2:1;
  unsigned LATD3:1;
  unsigned LATD4:1;
  unsigned LATD5:1;
  unsigned LATD6:1;
  unsigned LATD7:1;
} LATDbits;
volatile near unsigned char       LATE;
volatile near struct {
  unsigned LATE0:1;
  unsigned LATE1:1;
  unsigned LATE2:1;
  unsigned LATE3:1;
  unsigned LATE4:1;
  unsigned LATE5:1;
  unsigned LATE6:1;
  unsigned LATE7:1;
} LATEbits;
volatile near unsigned char       LATF;
volatile near struct {
  unsigned LATF0:1;
  unsigned LATF1:1;
  unsigned LATF2:1;
  unsigned LATF3:1;
  unsigned LATF4:1;
  unsigned LATF5:1;
  unsigned LATF6:1;
  unsigned LATF7:1;     /* Reserved - Do not use */
} LATFbits;
volatile near unsigned char       LATG;
volatile near struct {
  unsigned LATG0:1;
  unsigned LATG1:1;
  unsigned LATG2:1;
  unsigned LATG3:1;
  unsigned LATG4:1;
} LATGbits;
volatile near unsigned char       LATH;
volatile near struct {
  unsigned LATH0:1;
  unsigned LATH1:1;
  unsigned LATH2:1;
  unsigned LATH3:1;
  unsigned LATH4:1;
  unsigned LATH5:1;
  unsigned LATH6:1;
  unsigned LATH7:1;
} LATHbits;
volatile near unsigned char       LATJ;
volatile near struct {
  unsigned LATJ0:1;
  unsigned LATJ1:1;
  unsigned LATJ2:1;
  unsigned LATJ3:1;
  unsigned LATJ4:1;
  unsigned LATJ5:1;
  unsigned LATJ6:1;
  unsigned LATJ7:1;
} LATJbits;
volatile near unsigned char       TRISA;
volatile near struct {
  unsigned TRISA0:1;
  unsigned TRISA1:1;
  unsigned TRISA2:1;
  unsigned TRISA3:1;
  unsigned TRISA4:1;     /* Reserved - Do not use */
  unsigned TRISA5:1;
  unsigned TRISA6:1;
} TRISAbits;
volatile near unsigned char       DDRA;
volatile near struct {
  unsigned RA0:1;
  unsigned RA1:1;
  unsigned RA2:1;
  unsigned RA3:1;
  unsigned RA4:1;     /* Reserved - Do not use */
  unsigned RA5:1;
  unsigned RA6:1;
} DDRAbits;
volatile near unsigned char       DDRB;
volatile near struct {
  unsigned RB0:1;     /* Reserved - Do not use */
  unsigned RB1:1;
  unsigned RB2:1;
  unsigned RB3:1;
  unsigned RB4:1;
  unsigned RB5:1;
  unsigned RB6:1;
  unsigned RB7:1;
} DDRBbits;
volatile near unsigned char       TRISB;
volatile near struct {
  unsigned TRISB0:1;     /* Reserved - Do not use */
  unsigned TRISB1:1;
  unsigned TRISB2:1;
  unsigned TRISB3:1;
  unsigned TRISB4:1;
  unsigned TRISB5:1;
  unsigned TRISB6:1;
  unsigned TRISB7:1;
} TRISBbits;
volatile near unsigned char       DDRC;
volatile near struct {
  unsigned RC0:1;
  unsigned RC1:1;     /* Reserved - Do not use */
  unsigned RC2:1;     /* Reserved - Do not use */
  unsigned RC3:1;     /* Reserved - Do not use */
  unsigned RC4:1;     /* Reserved - Do not use */
  unsigned RC5:1;     /* Reserved - Do not use */
  unsigned RC6:1;
  unsigned RC7:1;
} DDRCbits;
volatile near unsigned char       TRISC;
volatile near struct {
  unsigned TRISC0:1;
  unsigned TRISC1:1;    /* Reserved - Do not use */
  unsigned TRISC2:1;    /* Reserved - Do not use */
  unsigned TRISC3:1;    /* Reserved - Do not use */
  unsigned TRISC4:1;    /* Reserved - Do not use */
  unsigned TRISC5:1;    /* Reserved - Do not use */
  unsigned TRISC6:1;
  unsigned TRISC7:1;
} TRISCbits;
volatile near unsigned char       DDRD;
volatile near struct {
  unsigned RD0:1;
  unsigned RD1:1;
  unsigned RD2:1;
  unsigned RD3:1;
  unsigned RD4:1;
  unsigned RD5:1;
  unsigned RD6:1;
  unsigned RD7:1;
} DDRDbits;
volatile near unsigned char       TRISD;
volatile near struct {
  unsigned TRISD0:1;
  unsigned TRISD1:1;
  unsigned TRISD2:1;
  unsigned TRISD3:1;
  unsigned TRISD4:1;
  unsigned TRISD5:1;
  unsigned TRISD6:1;
  unsigned TRISD7:1;
} TRISDbits;
volatile near unsigned char       DDRE;
volatile near struct {
  unsigned RE0:1;
  unsigned RE1:1;
  unsigned RE2:1;
  unsigned RE3:1;
  unsigned RE4:1;
  unsigned RE5:1;
  unsigned RE6:1;
  unsigned RE7:1;
} DDREbits;
volatile near unsigned char       TRISE;
volatile near struct {
  unsigned TRISE0:1;
  unsigned TRISE1:1;
  unsigned TRISE2:1;
  unsigned TRISE3:1;
  unsigned TRISE4:1;
  unsigned TRISE5:1;
  unsigned TRISE6:1;
  unsigned TRISE7:1;
} TRISEbits;
volatile near unsigned char       TRISF;
volatile near struct {
  unsigned TRISF0:1;
  unsigned TRISF1:1;
  unsigned TRISF2:1;
  unsigned TRISF3:1;
  unsigned TRISF4:1;
  unsigned TRISF5:1;
  unsigned TRISF6:1;
  unsigned TRISF7:1;  /* Reserved - Do not use */
} TRISFbits;
volatile near unsigned char       DDRF;
volatile near struct {
  unsigned RF0:1;
  unsigned RF1:1;
  unsigned RF2:1;
  unsigned RF3:1;
  unsigned RF4:1;
  unsigned RF5:1;
  unsigned RF6:1;
  unsigned RF7:1;     /* Reserved - Do not use */
} DDRFbits;
volatile near unsigned char       DDRG;
volatile near struct {
  unsigned RG0:1;
  unsigned RG1:1;
  unsigned RG2:1;
  unsigned RG3:1;
  unsigned RG4:1;
} DDRGbits;
volatile near unsigned char       TRISG;
volatile near struct {
  unsigned TRISG0:1;
  unsigned TRISG1:1;
  unsigned TRISG2:1;
  unsigned TRISG3:1;
  unsigned TRISG4:1;
} TRISGbits;
volatile near unsigned char       DDRH;
volatile near struct {
  unsigned RH0:1;
  unsigned RH1:1;
  unsigned RH2:1;
  unsigned RH3:1;
  unsigned RH4:1;
  unsigned RH5:1;
  unsigned RH6:1;
  unsigned RH7:1;
} DDRHbits;
volatile near unsigned char       TRISH;
volatile near struct {
  unsigned TRISH0:1;
  unsigned TRISH1:1;
  unsigned TRISH2:1;
  unsigned TRISH3:1;
  unsigned TRISH4:1;
  unsigned TRISH5:1;
  unsigned TRISH6:1;
  unsigned TRISH7:1;
} TRISHbits;
volatile near unsigned char       DDRJ;
volatile near struct {
  unsigned RJ0:1;
  unsigned RJ1:1;
  unsigned RJ2:1;
  unsigned RJ3:1;
  unsigned RJ4:1;
  unsigned RJ5:1;
  unsigned RJ6:1;
  unsigned RJ7:1;
} DDRJbits;
volatile near unsigned char       TRISJ;
volatile near struct {
  unsigned TRISJ0:1;
  unsigned TRISJ1:1;
  unsigned TRISJ2:1;
  unsigned TRISJ3:1;
  unsigned TRISJ4:1;
  unsigned TRISJ5:1;
  unsigned TRISJ6:1;
  unsigned TRISJ7:1;
} TRISJbits;
volatile near unsigned char       MEMCON;     /* Reserved - Do not use */
volatile near struct {
  unsigned WM0:1;
  unsigned WM1:1;
  unsigned :2;
  unsigned WAIT0:1;
  unsigned WAIT1:1;
  unsigned :1;
  unsigned EBDIS:1;
} MEMCONbits;     /* Reserved - Do not use */
volatile near unsigned char       PIE1;
volatile near union {
  struct {
    unsigned TMR1IE:1;
    unsigned TMR2IE:1;
    unsigned CCP1IE:1;
    unsigned SSPIE:1;     /* Reserved - Do not use */
    unsigned TX1IE:1;
    unsigned RC1IE:1;
    unsigned ADIE:1;
    unsigned PSPIE:1;     /* Reserved - Do not use */
  };
  struct {
    unsigned :4;
    unsigned TXIE:1;
    unsigned RCIE:1;
  };
} PIE1bits;
volatile near unsigned char       PIR1;
volatile near union {
  struct {
    unsigned TMR1IF:1;
    unsigned TMR2IF:1;
    unsigned CCP1IF:1;
    unsigned SSPIF:1;     /* Reserved - Do not use */
    unsigned TX1IF:1;
    unsigned RC1IF:1;
    unsigned ADIF:1;
    unsigned PSPIF:1;     /* Reserved - Do not use */
  };
  struct {
    unsigned :4;
    unsigned TXIF:1;
    unsigned RCIF:1;
  };
} PIR1bits;
volatile near unsigned char       IPR1;     /* Reserved - Do not use */
volatile near union {
  struct {
    unsigned TMR1IP:1;
    unsigned TMR2IP:1;
    unsigned CCP1IP:1;
    unsigned SSPIP:1;
    unsigned TX1IP:1;
    unsigned RC1IP:1;
    unsigned ADIP:1;
    unsigned PSPIP:1;
  };
  struct {
    unsigned :4;
    unsigned TXIP:1;
    unsigned RCIP:1;
  };
} IPR1bits;             /* Reserved - Do not use */
volatile near unsigned char       PIE2;
volatile near struct {
  unsigned CCP2IE:1;
  unsigned TMR3IE:1;
  unsigned LVDIE:1;     /* Reserved - Do not use */
  unsigned BCLIE:1;     /* Reserved - Do not use */
  unsigned EEIE:1;
  unsigned :1;
  unsigned CMIE:1;
} PIE2bits;
volatile near unsigned char       PIR2;
volatile near struct {
  unsigned CCP2IF:1;
  unsigned TMR3IF:1;
  unsigned LVDIF:1;     /* Reserved - Do not use */
  unsigned BCLIF:1;     /* Reserved - Do not use */
  unsigned EEIF:1;
  unsigned :1;
  unsigned CMIF:1;
} PIR2bits;
volatile near unsigned char       IPR2;     /* Reserved - Do not use */
volatile near struct {
  unsigned CCP2IP:1;
  unsigned TMR3IP:1;
  unsigned LVDIP:1;
  unsigned BCLIP:1;
  unsigned EEIP:1;
  unsigned :1;
  unsigned CMIP:1;
} IPR2bits;     /* Reserved - Do not use */
volatile near unsigned char       PIE3;
volatile near struct {
  unsigned CCP3IE:1;
  unsigned CCP4IE:1;
  unsigned CCP5IE:1;
  unsigned TMR4IE:1;
  unsigned TX2IE:1;
  unsigned RC2IE:1;
} PIE3bits;
volatile near unsigned char       PIR3;
volatile near struct {
  unsigned CCP3IF:1;
  unsigned CCP4IF:1;
  unsigned CCP5IF:1;
  unsigned TMR4IF:1;
  unsigned TX2IF:1;
  unsigned RC2IF:1;
} PIR3bits;
volatile near unsigned char       IPR3;     /* Reserved - Do not use */
volatile near struct {
  unsigned CCP3IP:1;
  unsigned CCP4IP:1;
  unsigned CCP5IP:1;
  unsigned TMR4IP:1;
  unsigned TX2IP:1;
  unsigned RC2IP:1;
} IPR3bits;     /* Reserved - Do not use */
volatile near unsigned char       EECON1;     /* Use with caution. */
volatile near struct {
  unsigned RD:1;
  unsigned WR:1;
  unsigned WREN:1;
  unsigned WRERR:1;
  unsigned FREE:1;     /* Use with caution. Could result in program corruption. */
  unsigned :1;
  unsigned CFGS:1;
  unsigned EEPGD:1;
} EECON1bits;
volatile near unsigned char       EECON2;
volatile near unsigned char       EEDATA;
volatile near unsigned char       EEADR;
volatile near unsigned char       EEADRH;
volatile near unsigned char       RCSTA1;
volatile near union {
  struct {
    unsigned RX9D:1;
    unsigned OERR:1;
    unsigned FERR:1;
    unsigned ADEN:1;
    unsigned CREN:1;
    unsigned SREN:1;
    unsigned RX9:1;
    unsigned SPEN:1;
  };
  struct {
    unsigned :3;
    unsigned ADDEN:1;
  };
} RCSTA1bits;
volatile near unsigned char       TXSTA1;
volatile near struct {
  unsigned TX9D:1;
  unsigned TRMT:1;
  unsigned BRGH:1;
  unsigned :1;
  unsigned SYNC:1;
  unsigned TXEN:1;
  unsigned TX9:1;
  unsigned CSRC:1;
} TXSTA1bits;
volatile near unsigned char       TXREG1;
volatile near unsigned char       RCREG1;
volatile near unsigned char       SPBRG1;
volatile near unsigned char       PSPCON;     /* Reserved - Do not use */
volatile near struct {
  unsigned :4;
  unsigned PSPMODE:1;
  unsigned IBOV:1;
  unsigned OBF:1;
  unsigned IBF:1;
} PSPCONbits;     /* Reserved - Do not use */
volatile near unsigned char       T3CON;
volatile near union {
  struct {
    unsigned TMR3ON:1;
    unsigned TMR3CS:1;
    unsigned T3SYNC:1;
    unsigned T3CCP1:1;
    unsigned T3CKPS0:1;
    unsigned T3CKPS1:1;
    unsigned T3CCP2:1;
    unsigned RD16:1;
  };
  struct {
    unsigned :2;
    unsigned T3NSYNC:1;
  };
  struct {
    unsigned :2;
    unsigned NOT_T3SYNC:1;
  };
} T3CONbits;
volatile near unsigned            TMR3;
volatile near unsigned char       TMR3L;
volatile near unsigned char       TMR3H;
volatile near unsigned char       CMCON;
volatile near struct {
  unsigned CM0:1;
  unsigned CM1:1;
  unsigned CM2:1;
  unsigned CIS:1;
  unsigned C1INV:1;
  unsigned C2INV:1;
  unsigned C1OUT:1;
  unsigned C2OUT:1;
} CMCONbits;
volatile near unsigned char       CVRCON;
volatile near struct {
  unsigned CVR0:1;
  unsigned CVR1:1;
  unsigned CVR2:1;
  unsigned CVR3:1;
  unsigned CVREF:1;
  unsigned CVRR:1;
  unsigned CVROE:1;
  unsigned CVREN:1;
} CVRCONbits;
volatile near unsigned char       CCP3CON;
volatile near union {
  struct {
    unsigned CCP3M0:1;
    unsigned CCP3M1:1;
    unsigned CCP3M2:1;
    unsigned CCP3M3:1;
    unsigned DCCP3Y:1;
    unsigned DCCP3X:1;
  };
  struct {
    unsigned :4;
    unsigned DC3B0:1;
    unsigned DC3B1:1;
  };
} CCP3CONbits;
volatile near unsigned            CCPR3;
volatile near unsigned char       CCPR3L;
volatile near unsigned char       CCPR3H;
volatile near unsigned char       CCP2CON;
volatile near union {
  struct {
    unsigned CCP2M0:1;
    unsigned CCP2M1:1;
    unsigned CCP2M2:1;
    unsigned CCP2M3:1;
    unsigned DCCP2Y:1;
    unsigned DCCP2X:1;
  };
  struct {
    unsigned :4;
    unsigned CCP2Y:1;
    unsigned CCP2X:1;
  };
  struct {
    unsigned :4;
    unsigned DC2B0:1;
    unsigned DC2B1:1;
  };
} CCP2CONbits;
volatile near unsigned            CCPR2;
volatile near unsigned char       CCPR2L;
volatile near unsigned char       CCPR2H;
volatile near unsigned char       CCP1CON;
volatile near union {
  struct {
    unsigned CCP1M0:1;
    unsigned CCP1M1:1;
    unsigned CCP1M2:1;
    unsigned CCP1M3:1;
    unsigned DCCP1Y:1;
    unsigned DCCP1X:1;
  };
  struct {
    unsigned :4;
    unsigned CCP1Y:1;
    unsigned CCP1X:1;
  };
  struct {
    unsigned :4;
    unsigned DC1B0:1;
    unsigned DC1B1:1;
  };
} CCP1CONbits;
volatile near unsigned char       CCPR1L;
volatile near unsigned            CCPR1;
volatile near unsigned char       CCPR1H;
volatile near unsigned char       ADCON2;
volatile near struct {
  unsigned ADCS0:1;
  unsigned ADCS1:1;
  unsigned ADCS2:1;
  unsigned :4;
  unsigned ADFM:1;
} ADCON2bits;
volatile near unsigned char       ADCON1;
volatile near struct {
  unsigned PCFG0:1;
  unsigned PCFG1:1;
  unsigned PCFG2:1;
  unsigned PCFG3:1;
  unsigned VCFG0:1;
  unsigned VCFG1:1;
} ADCON1bits;
volatile near unsigned char       ADCON0;
volatile near union {
  struct {
    unsigned ADON:1;
    unsigned GO_DONE:1;
    unsigned CHS0:1;
    unsigned CHS1:1;
    unsigned CHS2:1;
    unsigned CHS3:1;
  };
  struct {
    unsigned :1;
    unsigned DONE:1;
  };
  struct {
    unsigned :1;
    unsigned GO:1;
  };
  struct {
    unsigned :1;
    unsigned NOT_DONE:1;
  };
} ADCON0bits;
volatile near unsigned            ADRES;
volatile near unsigned char       ADRESL;
volatile near unsigned char       ADRESH;
volatile near unsigned char       SSPCON2;     /* Reserved - Do not use */
volatile near struct {
  unsigned SEN:1;
  unsigned RSEN:1;
  unsigned PEN:1;
  unsigned RCEN:1;
  unsigned ACKEN:1;
  unsigned ACKDT:1;
  unsigned ACKSTAT:1;
  unsigned GCEN:1;
} SSPCON2bits;     /* Reserved - Do not use */
volatile near unsigned char       SSPCON1;     /* Reserved - Do not use */
                                      /* SSPCON1bits  Reserved */
volatile near unsigned char       SSPSTAT;     /* Reserved - Do not use */
volatile near union {
  struct {
    unsigned BF:1;
    unsigned UA:1;
    unsigned R_W:1;
    unsigned S:1;
    unsigned P:1;
    unsigned D_A:1;
    unsigned CKE:1;
    unsigned SMP:1;
  };
  struct {
    unsigned :2;
    unsigned I2C_READ:1;
    unsigned I2C_START:1;
    unsigned I2C_STOP:1;
    unsigned I2C_DAT:1;
  };
  struct {
    unsigned :2;
    unsigned NOT_W:1;
    unsigned :2;
    unsigned NOT_A:1;
  };
  struct {
    unsigned :2;
    unsigned NOT_WRITE:1;
    unsigned :2;
    unsigned NOT_ADDRESS:1;
  };
  struct {
    unsigned :2;
    unsigned READ_WRITE:1;
    unsigned :2;
    unsigned DATA_ADDRESS:1;
  };
  struct {
    unsigned :2;
    unsigned R:1;
    unsigned :2;
    unsigned D:1;
  };
} SSPSTATbits;     /* Reserved - Do not use */
volatile near unsigned char       SSPADD;     /* Reserved - Do not use */
volatile near unsigned char       SSPBUF;     /* Reserved - Do not use */
volatile near unsigned char       T2CON;
volatile near struct {
  unsigned T2CKPS0:1;
  unsigned T2CKPS1:1;
  unsigned TMR2ON:1;
  unsigned T2OUTPS0:1;
  unsigned T2OUTPS1:1;
  unsigned T2OUTPS2:1;
  unsigned T2OUTPS3:1;
} T2CONbits;
volatile near unsigned char       PR2;
volatile near unsigned char       TMR2;
volatile near unsigned char       T1CON;
volatile near union {
  struct {
    unsigned TMR1ON:1;
    unsigned TMR1CS:1;
    unsigned T1SYNC:1;
    unsigned T1OSCEN:1;
    unsigned T1CKPS0:1;
    unsigned T1CKPS1:1;
    unsigned :1;
    unsigned RD16:1;
  };
  struct {
    unsigned :2;
    unsigned T1INSYNC:1;
  };
  struct {
    unsigned :2;
    unsigned NOT_T1SYNC:1;
  };
} T1CONbits;
volatile near unsigned char       TMR1L;
volatile near unsigned            TMR1;
volatile near unsigned char       TMR1H;
volatile near unsigned char       RCON;     /* Reserved - Do not use */
volatile near union {
  struct {
    unsigned NOT_BOR:1;
    unsigned NOT_POR:1;
    unsigned NOT_PD:1;
    unsigned NOT_TO:1;
    unsigned NOT_RI:1;
    unsigned :2;
    unsigned NOT_IPEN:1;
  };
  struct {
    unsigned BOR:1;
    unsigned POR:1;
    unsigned PD:1;
    unsigned TO:1;
    unsigned RI:1;
    unsigned :2;
    unsigned IPEN:1;
  };
} RCONbits;     /* Reserved - Do not use */
volatile near unsigned char       WDTCON;     /* Reserved - Do not use */
volatile near union {
  struct {
    unsigned SWDTEN:1;
  };
  struct {
    unsigned SWDTE:1;
  };
} WDTCONbits;     /* Reserved - Do not use */
volatile near unsigned char       LVDCON;     /* Reserved - Do not use */
volatile near union {
  struct {
    unsigned LVDL0:1;
    unsigned LVDL1:1;
    unsigned LVDL2:1;
    unsigned LVDL3:1;
    unsigned LVDEN:1;
    unsigned IRVST:1;
  };
  struct {
    unsigned LVV0:1;
    unsigned LVV1:1;
    unsigned LVV2:1;
    unsigned LVV3:1;
    unsigned :1;
    unsigned BGST:1;
  };
} LVDCONbits;     /* Reserved - Do not use */
volatile near unsigned char       OSCCON;     /* Reserved - Do not use */
volatile near struct {
  unsigned SCS:1;
} OSCCONbits;                                        /* Reserved - Do not use */
volatile near unsigned char       T0CON;      /* Reserved - Do not use if you are using ifi_library.lib */
volatile near struct {
  unsigned T0PS0:1;
  unsigned T0PS1:1;
  unsigned T0PS2:1;
  unsigned PSA:1;
  unsigned T0SE:1;
  unsigned T0CS:1;
  unsigned T08BIT:1;
  unsigned TMR0ON:1;
} T0CONbits;                                     /* Reserved - Do not use if you are using ifi_library.lib */
volatile near unsigned            TMR0;   /* Reserved - Do not modify if you are using ifi_library.lib */
volatile near unsigned char       TMR0L;  /* Reserved - Do not modify if you are using ifi_library.lib */
volatile near unsigned char       TMR0H;  /* Reserved - Do not modify if you are using ifi_library.lib */
near unsigned char       STATUS;
near struct {
  unsigned C:1;
  unsigned DC:1;
  unsigned Z:1;
  unsigned OV:1;
  unsigned N:1;
} STATUSbits;
near unsigned            FSR2;  /* Reserved - Do not use */
near unsigned char       FSR2L;  /* Reserved - Do not use */
near unsigned char       FSR2H;  /* Reserved - Do not use */
volatile near unsigned char       PLUSW2;  /* Reserved - Do not use */
volatile near unsigned char       PREINC2;  /* Reserved - Do not use */
volatile near unsigned char       POSTDEC2;  /* Reserved - Do not use */
volatile near unsigned char       POSTINC2;  /* Reserved - Do not use */
near unsigned char       INDF2;  /* Reserved - Do not use */
near unsigned char       BSR;  /* Reserved - Do not use */
near unsigned            FSR1;  /* Reserved - Do not use */
near unsigned char       FSR1L;  /* Reserved - Do not use */
near unsigned char       FSR1H;  /* Reserved - Do not use */
volatile near unsigned char       PLUSW1;  /* Reserved - Do not use */
volatile near unsigned char       PREINC1;  /* Reserved - Do not use */
volatile near unsigned char       POSTDEC1;  /* Reserved - Do not use */
volatile near unsigned char       POSTINC1;  /* Reserved - Do not use */
near unsigned char       INDF1;  /* Reserved - Do not use */
near unsigned char       WREG;  /* Use at your own risk. */
near unsigned char       FSR0L;  /* Use at your own risk. */
near unsigned            FSR0;  /* Use at your own risk. */
near unsigned char       FSR0H;  /* Use at your own risk. */
volatile near unsigned char       PLUSW0;  /* Use at your own risk. */
volatile near unsigned char       PREINC0;  /* Use at your own risk. */
volatile near unsigned char       POSTDEC0;  /* Use at your own risk. */
volatile near unsigned char       POSTINC0;  /* Use at your own risk. */
near unsigned char       INDF0;  /* Use at your own risk. */
volatile near unsigned char       INTCON3;
volatile near union {
  struct {
    unsigned INT1IF:1;
    unsigned INT2IF:1;
    unsigned INT3IF:1;
    unsigned INT1IE:1;
    unsigned INT2IE:1;
    unsigned INT3IE:1;
    unsigned INT1IP:1;     /* Reserved - Do not use */
    unsigned INT2IP:1;     /* Reserved - Do not use */  /* Must be set to 0 (low priority) */
  };
  struct {
    unsigned INT1F:1;
    unsigned INT2F:1;
    unsigned INT3F:1;
    unsigned INT1E:1;
    unsigned INT2E:1;
    unsigned INT3E:1;
    unsigned INT1P:1;     /* Reserved - Do not use */
    unsigned INT2P:1;     /* Reserved - Do not use */
  };
} INTCON3bits;
volatile near unsigned char       INTCON2;
volatile near union {
  struct {
    unsigned RBIP:1;        /* Reserved - Do not use */
    unsigned INT3IP:1;      /* Reserved - Do not use */
    unsigned TMR0IP:1;      /* Reserved - Do not use */
    unsigned INTEDG3:1;
    unsigned INTEDG2:1;
    unsigned INTEDG1:1;
    unsigned INTEDG0:1;     /* Reserved - Do not use */
    unsigned NOT_RBPU:1;    /* Reserved - Do not use */
  };
  struct {
    unsigned :1;
    unsigned INT3P:1;     /* Reserved - Do not use */
    unsigned T0IP:1;      /* Reserved - Do not use */
    unsigned :4;
    unsigned RBPU:1;      /* Reserved - Do not use */
  };
} INTCON2bits;
volatile near unsigned char       INTCON;
volatile near union {
  struct {
    unsigned RBIF:1;
    unsigned INT0IF:1;  /* Reserved - Do not use */
    unsigned TMR0IF:1;  /* Reserved - Do not modify if you are using ifi_library.lib */
    unsigned RBIE:1;
    unsigned INT0IE:1;  /* Reserved - Do not use */
    unsigned TMR0IE:1;  /* Reserved - Do not use if you are using ifi_library.lib */
    unsigned PEIE:1;
    unsigned GIE:1;     /* Reserved - Do not use */
  };
  struct {
    unsigned :1;
    unsigned INT0F:1;   /* Reserved - Do not use */
    unsigned T0IF:1;
    unsigned :1;
    unsigned INT0E:1;   /* Reserved - Do not use */
    unsigned T0IE:1;
    unsigned GIEL:1;    /* Use of this bit could lead to unexpected results */
    unsigned GIEH:1;    /* Reserved - Do not use */
  };
} INTCONbits;
near unsigned char       PRODL;
near unsigned            PROD;
near unsigned char       PRODH;
volatile near unsigned char       TABLAT;
volatile near unsigned char       TBLPTRL;
volatile near UINT24              TBLPTR;
volatile near unsigned char       TBLPTRH;
volatile near unsigned char       TBLPTRU;
volatile near unsigned char       PCL;
volatile near UINT24              PC;
volatile near unsigned char       PCLATH;
volatile near unsigned char       PCLATU;
volatile near unsigned char       STKPTR;
volatile near union {
  struct {
    unsigned STKPTR0:1;
    unsigned STKPTR1:1;
    unsigned STKPTR2:1;
    unsigned STKPTR3:1;
    unsigned STKPTR4:1;
    unsigned :1;
    unsigned STKUNF:1;
    unsigned STKOVF:1;
  };
  struct {
    unsigned :7;
    unsigned STKFUL:1;
  };
} STKPTRbits;
near UINT24 TOS;
near unsigned char       TOSL;
near unsigned char       TOSH;
near unsigned char       TOSU;

