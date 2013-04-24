/*******************************************************************************
* FILE NAME: ws_crab_feedback.h
*
* DESCRIPTION:
*  This is the include file which corresponds to ws_crab_feedback.c
*
*  It contains some aliases and function prototypes used in those files.
*
* USAGE:
*  If you add your own routines to those files, this is a good place to add
*  your custom macros (aliases), type definitions, and function prototypes.
*******************************************************************************/

#ifndef __ws_crab_feedback_h_
#define __ws_crab_feedback_h_

/******************************* TYPEDEFS *************************************/

/******************************** MACROS **************************************/

/***************************** DEFINITIONS ************************************/

/****************************** STRUCTURES ************************************/

/************************* FUNCTION PROTOTYPES ********************************/
extern void crab_system(CrabTargetsType *, MotorValsType *,
                        CalibrationValsType *);
extern UINT8 crab_scaling(UINT8, UINT8, UINT8, UINT8);
extern UINT8 crab_feedback(UINT8, UINT8);


#endif /* __ws_crab_feedback_h_ */

