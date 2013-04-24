/*******************************************************************************
* FILE NAME: ws_cc.h
*
* DESCRIPTION:
*  This is the include file which corresponds to ws_cc.c
*
*  It contains some aliases and function prototypes used in those files.
*
* USAGE:
*  If you add your own routines to those files, this is a good place to add
*  your custom macros (aliases), type definitions, and function prototypes.
*******************************************************************************/

#ifndef __ws_cc_h_
#define __ws_cc_h_

#include "ws_autonomous.h"  /* Needed for PositionType */
/******************************* TYPEDEFS *************************************/

typedef enum
{
   CC_SUCCESS=1,
   CC_TIMEOUT
} CcReturnType;

/******************************** MACROS **************************************/

/***************************** DEFINITIONS ************************************/

/****************************** STRUCTURES ************************************/

/************************* FUNCTION PROTOTYPES ********************************/

extern void cc_gyro_calibrate(void);
extern CcReturnType cc_get_heading(UINT8* p_theta);
extern CcReturnType cc_get_position(PositionType *p_curr_pos);
extern void cc_set_position(PositionType *);
extern UINT8 readCcReg(unsigned char* p_data, unsigned char cmd,
                UINT8 resp_size, int timeout_per_char);

extern void print_cc_debug(void);


#endif /* __ws_cc_h_ */

