/*******************************************************************************
* FILE NAME: compiler.h
*
* DESCRIPTION:
*  This file contains compiler definitions.
*
*******************************************************************************/

#ifndef __compiler_h_
#define __compiler_h_

/******************************* TYPEDEFS *************************************/
#ifdef CYGWIN_IFI_SIMULATOR
/* Simulator defines */
typedef unsigned char   INT8;
typedef unsigned char   UINT8;
typedef signed char     SINT8;
typedef unsigned short  UINT16;
typedef signed short    INT16;
/* 'short long' does not exist with gcc, so just use an in */
typedef signed int      SINT24;
typedef unsigned int    UINT24;
typedef unsigned int    UINT32;
typedef signed int      SINT32;
#else
/* Target defines */
typedef unsigned char       UINT8;
typedef char                INT8;
typedef signed short long   SINT24;
typedef unsigned short long UINT24;
typedef unsigned int        UINT16;
typedef int                 INT16;
#endif


/******************************** MACROS **************************************/

/***************************** DEFINITIONS ************************************/

/****************************** STRUCTURES ************************************/

/************************* FUNCTION PROTOTYPES ********************************/

#endif /* __compiler_h_ */

