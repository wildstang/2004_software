/*******************************************************************************
* FILE NAME: ws_feedback.h
*
* DESCRIPTION:
*  This is the include file which corresponds to ws_feedback.c
*
*  It contains some aliases and function prototypes used in those files.
*
* USAGE:
*  If you add your own routines to those files, this is a good place to add
*  your custom macros (aliases), type definitions, and function prototypes.
*******************************************************************************/

#ifndef __ws_feedback_h_
#define __ws_feedback_h_

/******************************* TYPEDEFS *************************************/
typedef struct feedback_consts_
{
  UINT8 pos_near_inc;
  UINT8 pos_close_inc;
  UINT8 pos_far_inc;
  UINT8 pos_near_dec;
  UINT8 pos_close_dec;
  UINT8 pos_far_dec;
  UINT8 speed_slow_inc;
  UINT8 speed_fast_inc;
  UINT8 speed_slow_dec;
  UINT8 speed_fast_dec;
} FeedbackConstsType;

/******************************** MACROS **************************************/

/***************************** DEFINITIONS ************************************/

/****************************** STRUCTURES ************************************/

/************************* FUNCTION PROTOTYPES ********************************/
INT8 general_pos_feedback(UINT8 curr_pos, UINT8 desired_pos,
                          static const rom FeedbackConstsType *consts);

#endif /* __ws_feedback_h_ */

