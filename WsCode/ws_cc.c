/*******************************************************************************
* FILE NAME: ws_cc.c
*
* DESCRIPTION:
*
* USAGE:
*
*******************************************************************************/

#ifndef CYGWIN_IFI_SIMULATOR
#include "ifi_aliases.h"
#include "ifi_default.h"
#include "ifi_utilities.h"
#include "ws_includes.h"
#include "user_routines.h"
#include "printf_lib.h"
#include "delays.h"       /*defined locally*/
#include "ws_io.h"
#include "ws_serial2.h"
#include "ws_sync_serial.h"
#else /* CYGWIN_IFI_SIMULATOR include */
#include "RC_Simulator.h"
#include "stdio.h"
#include "ws_includes.h"
#endif

#include "ws_cc.h"
#include "ws_autonomous.h"

/* Use these defines to dump debug info from the CC in input_data */
/*#define CC_DEBUG_RAYX*/
/*#define CC_DEBUG_RAYY*/
/*#define CC_DEBUG_WHEEL_POS*/
/*#define CC_DEBUG_QUAD_ERRS*/

/*Cmd byte sent to CC to request X,Y,theta */
#define  CC_CMD_REQ_POSITION  180
/*Cmd byte sent to CC to request X(16bit),Y(16bit),theta(byte) */
#define  CC_CMD_REQ_16BIT_POSITION  182  /* 16 bit */
/*Cmd byte sent to CC to request X(16bit),Y(16bit),theta(byte) 
  of the center of the robot (not the sense wheel) */
#define  CC_CMD_REQ_16BIT_CENTER_POSITION  184  /* 16 bit */
/*Cmd byte sent to CC to request just X postion*/
#define  CC_CMD_REQ_X_POS     85
/*Cmd byte sent to CC to request just Y postion*/
#define  CC_CMD_REQ_Y_POS     170
/*Cmd byte sent to CC to request just theta postion*/
#define  CC_CMD_REQ_THETA     240
/*Cmd byte sent to CC to request gyro calibration*/
#define  RECALIBRATE_GYRO     210
/*Cmd byte sent to CC to set X,Y,theta on the CC if it reset*/
#define  CC_CMD_SET_POSITION  120
/*Cmd byte sent to CC to set X,Y,theta on the CC if it reset(16bit x,y)*/
#define  CC_CMD_SET_16BIT_POSITION  122
/*Cmd byte sent to CC to set X,Y,theta on the CC if it reset(16bit x,y)
  of the center of the robot (not the sense wheel) */
#define  CC_CMD_SET_16BIT_CENTER_POSITION  124
/*Cmd byte sent to CC to set our current waypoint */
#define  CC_CMD_SET_WAYPOINT  130
/*Cmd byte sent to CC to get our current waypoint*/
#define  CC_CMD_REQ_WAYPOINT  140
/*Cmd byte sent to CC to get our program number and side*/
#define  CC_CMD_REQ_PROG_SIDE 200
#define  CC_CMD_REQ_RAW_X     100  /* Request raw 32-bit X value for debug */
#define  CC_CMD_REQ_RAW_Y     175  /* Request raw 32-bit Y value for debug */
#define  CC_CMD_REQ_WHEEL_BRAD 60  /* Request brads of the crab wheel */
#define  CC_CMD_REQ_WHEEL_POT  63  /* Request pot of the crab wheel */
#define  CC_CMD_REQ_WHEEL_HDG  70  /* Request wheel heading */
#define  CC_CMD_REQ_QUAD_ERRS  66  /* Quad decode errors */

#define  CC_LOOP_CNT_TIMEOUT  1000 /* Num of loops before giving up on a resp
                                      from the CC */

/* Size of the response to various commands */
#define CC_RESP_X_POS_SIZE      1  /* XPos is 1 byte */
#define CC_RESP_Y_POS_SIZE      1  /* YPos is 1 byte */
#define CC_RESP_THETA_POS_SIZE  1  /* Theta is 1 byte */
#define CC_RESP_QUAD_ERRS_SIZE  1  /* Quad Decode Errors is 1 byte */
#define CC_RESP_WHEEL_BRAD_SIZE 1  /* Wheel Brads is 1 byte */
#define CC_RESP_WHEEL_POT_SIZE  1  /* Wheel pot is 1 byte */
#define CC_RESP_WHEEL_HDG_SIZE  1  /* Wheel hdg is 1 byte */
#define CC_RESP_RAWY_SIZE       4  /* Raw Y is 4 bytes */
#define CC_RESP_RAWX_SIZE       4  /* Raw X is 4 bytes */
#define CC_RESP_16BIT_POS       5  /* 2bytesX, 2bytesY, 1byteTheta */

/*
#define FAKE_AUTON_WITH_MOUSE */   /* Cause input data to just use the mouse instead of CC */



/*******************************************************************************
* FUNCTION NAME: cc_set_position
* PURPOSE:       Tell the CC the current position of the robot.
* CALLED FROM:   
* ARGUMENTS:     
* RETURNS:       
*******************************************************************************/
void cc_set_position(PositionType *p_curr_pos)
{
    /* send starting postition to CC */
    SendSerialByte2(CC_CMD_SET_16BIT_CENTER_POSITION);
    Delay10TCY();

    SendSerialByte2(((p_curr_pos->x >> 8) & 0xFF));  /* 16bit X, MSB first */
    Delay10TCY();
    SendSerialByte2((p_curr_pos->x & 0xFF));
    Delay10TCY();
    SendSerialByte2(((p_curr_pos->y >> 8) & 0xFF));  /* 16bit Y, MSB first */
    Delay10TCY();
    SendSerialByte2((p_curr_pos->y & 0xFF));
    Delay10TCY();
    SendSerialByte2(p_curr_pos->theta);              /* 8bit theta */

    return;
}

/*******************************************************************************
* FUNCTION NAME: cc_gyro_calibrate
* PURPOSE:       Cause the CC to recalibrate the gyro to get the current
*                center position of the gyro.  Make sure the robot isn't moving
*                before calling the function.
* CALLED FROM:   
* ARGUMENTS:     
* RETURNS:       
*******************************************************************************/
void cc_gyro_calibrate(void)
{
    /* recalibrate gyro (send twice as a double check) */
    SendSerialByte2(RECALIBRATE_GYRO);  /* 8bit theta */
    SendSerialByte2(RECALIBRATE_GYRO);  /* 8bit theta */

    return;
}

/*******************************************************************************
* FUNCTION NAME: cc_get_heading
* PURPOSE:       Read the robot heading (compass value) from the CC.
* CALLED FROM:   
* ARGUMENTS:     
* RETURNS:       
*******************************************************************************/
CcReturnType cc_get_heading(UINT8* p_theta)
{
  CcReturnType   ret_val=CC_SUCCESS;

  if(readCcReg(p_theta, CC_CMD_REQ_THETA,
                CC_RESP_THETA_POS_SIZE, CC_LOOP_CNT_TIMEOUT) < CC_RESP_THETA_POS_SIZE  )
  {
     ret_val=CC_TIMEOUT;
  }

  return(ret_val);
}

/*******************************************************************************
* FUNCTION NAME: cc_get_position
* PURPOSE:       Read the X,Y,Theta of the robot from the CC 
* CALLED FROM:   
* ARGUMENTS:     
* RETURNS:       
*******************************************************************************/
#define BITS_OF_MOUSE_TO_SHIFT 5
#define DEFAULT_MOUSE_X 128
#define DEFAULT_MOUSE_Y 128
#define DEFAULT_MOUSE_THETA 0
CcReturnType cc_get_position(PositionType *p_curr_pos)
{
  CcReturnType cc_ret_val=CC_SUCCESS;

  unsigned char pos_data[CC_RESP_16BIT_POS];

  /* Clear any extra chars out from the input buffer */
  ClearRxBuffer2();

  /* request x position data */ 
  /*printf("Send1 ");*/
  if(readCcReg(&(pos_data[0]), CC_CMD_REQ_16BIT_CENTER_POSITION,
                CC_RESP_16BIT_POS, CC_LOOP_CNT_TIMEOUT) < CC_RESP_16BIT_POS)
  {
     /* No response from the CC...tell the auton code not to run */
     cc_ret_val= CC_TIMEOUT;
     printf("CC TO1 ");
  }
  else
  {
     /* pos_data is |Xhigh|Xlow|Yhigh|Ylow|Theta|, break out the data */
     /*printf("pos_data=%X%X%X%X%X ",(int)pos_data[0],(int)pos_data[1],
             (int)pos_data[2], (int)pos_data[3],(int)pos_data[4]);*/

     /* X is byte 0 and 1 */
     p_curr_pos->x = pos_data[1];
     p_curr_pos->x = p_curr_pos->x + ((int)pos_data[0] << 8);

     /* Y is byte 2 and 3 */
     p_curr_pos->y = pos_data[3];
     p_curr_pos->y = p_curr_pos->y + ((int)pos_data[2] << 8);

     /* Theta is byte 4 */
     p_curr_pos->theta = pos_data[4];

     /* Print out various CC debug information */
     print_cc_debug();
  }
  /*printf(" RXErr=%d ",(int)s_rx1_errors);*/
  printf("X=%d Y=%d T=%d",
         (int)p_curr_pos->x, (int)p_curr_pos->y,
         (int)p_curr_pos->theta);

  return(cc_ret_val);
}

/*******************************************************************************
* FUNCTION NAME: readCcReg
* PURPOSE:       Read a register on the CC.
* CALLED FROM:   
* ARGUMENTS:     
*            p_data    - Pointer to the buffer where the contents of the reg will be put
*            cmd       - Which specific register is being read
*            resp_size - Size of the register being read
*            timeout   - Timeout to wait for each char of the register being read
* RETURNS:       
*            Number of bytes read from the register
*******************************************************************************/
UINT8 readCcReg(unsigned char* p_data, unsigned char cmd,
                UINT8 resp_size, int timeout_per_char)
{
  UINT8  byte_recvd;    /* Was a byte recvd in this loop */
  UINT8  bytes_recvd=0; /* How many bytes were recvd total */
  UINT8  i;
  int    loop_count;

  /* Send the command */
  SendSerialByte2(cmd);

  /* Now recv all the expected response data, break out after all the
   * expected bytes are recvd or until we don't recv a byte.
   */
  byte_recvd=1;   /* Init to 1 so the first loop works */
  for(i=0;(i < resp_size) && (byte_recvd == 1);i++)
  {
     loop_count=timeout_per_char;
     byte_recvd = 0;

     do
     {
        /* GetRxByte puts data recieved into curr_pos->theta  */
        byte_recvd =  GetRxByte2(&p_data[i]);
        loop_count--;
     }while(byte_recvd == 0 && loop_count > 0);

     /* Add the byte we recvd to the total count */
     bytes_recvd += byte_recvd;
  }

  return(bytes_recvd);
}

/*******************************************************************************
* FUNCTION NAME: print_cc_debug
* PURPOSE:       Print out various debug information from the CC.  The info
*                printed is controled by various #defines
* CALLED FROM:   
* ARGUMENTS:     
* RETURNS:       
*******************************************************************************/
void print_cc_debug(void)
{
/* TEMP DEBUG, GET THE QUAD DECODE ERRORS */
#ifdef CC_DEBUG_QUAD_ERRS
{
        unsigned char quad_errs;
        if(readCcReg(&(quad_errs), CC_CMD_REQ_QUAD_ERRS,
                     CC_RESP_QUAD_ERRS_SIZE, CC_LOOP_CNT_TIMEOUT)
                                                  < CC_RESP_QUAD_ERRS_SIZE)
        {
           /* No response from the CC...tell the auton code not to run */
           printf("CC TOQERR ");
        }
        printf("QERR=%d ", (int)quad_errs);
}
#endif

/* TEMP DEBUG, GET THE WHEEL BRAD VALUE */
#ifdef CC_DEBUG_WHEEL_POS
{
        unsigned char brad, pot, hdg;
        if(readCcReg(&(brad), CC_CMD_REQ_WHEEL_BRAD,
                     CC_RESP_WHEEL_BRAD_SIZE, CC_LOOP_CNT_TIMEOUT)
                                                  < CC_RESP_WHEEL_BRAD_SIZE)
        {
           /* No response from the CC...tell the auton code not to run */
           printf("CC TOBRAD ");
        }

        if(readCcReg(&(pot), CC_CMD_REQ_WHEEL_POT,
                     CC_RESP_WHEEL_POT_SIZE, CC_LOOP_CNT_TIMEOUT)
                                                  < CC_RESP_WHEEL_POT_SIZE)
        {
           /* No response from the CC...tell the auton code not to run */
           printf("CC TOPOT ");
        }

        if(readCcReg(&(hdg), CC_CMD_REQ_WHEEL_HDG,
                     CC_RESP_WHEEL_HDG_SIZE, CC_LOOP_CNT_TIMEOUT)
                                                  < CC_RESP_WHEEL_HDG_SIZE)
        {
           /* No response from the CC...tell the auton code not to run */
           printf("CC TOHDG ");
        }
        printf("BRAD=%d(%d) HDG=%d ",(int)brad, (int)pot, (int)hdg);
}
#endif

/* TEMP DEBUG, GET THE FULL Y DATA */
#ifdef CC_DEBUG_RAYY
{
        unsigned char rawy[CC_RESP_RAWY_SIZE];
        if(readCcReg(&(rawy[0]), CC_CMD_REQ_RAW_Y,
                     CC_RESP_RAWY_SIZE, CC_LOOP_CNT_TIMEOUT)
                                                 < CC_RESP_RAWY_SIZE)
        {
           /* No response from the CC...tell the auton code not to run */
           printf("CC TOYRAW ");
        }
        printf("RAWY=%2X%2X%2X%2X ",(int)rawy[0], (int)rawy[1], (int)rawy[2],
               (int)rawy[3]);
}
#endif

/* TEMP DEBUG, GET THE FULL X DATA */
#ifdef CC_DEBUG_RAYX
{
        unsigned char rawx[CC_RESP_RAWX_SIZE];
        if(readCcReg(&(rawx[0]), CC_CMD_REQ_RAW_X,
                     CC_RESP_RAWX_SIZE, CC_LOOP_CNT_TIMEOUT)
                                                  < CC_RESP_RAWX_SIZE)
        {
           /* No response from the CC...tell the auton code not to run */
           printf("CC TOXRAW ");
        }
        printf("RAWX=%2X%2X%2X%2X ",(int)rawx[0], (int)rawx[1], (int)rawx[2],
               (int)rawx[3]);
}
#endif

  return;
}
