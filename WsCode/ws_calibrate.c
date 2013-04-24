/*******************************************************************************
* FILE NAME: ws_calibrate.c
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
#else
#include "RC_Simulator.h"
#include "ifi_aliases.h"
#include "ifi_default.h"
#include "stdio.h"
#include "ws_includes.h"
#endif

#include "ws_calibrate.h"
#include "ws_io.h"
#include "ws_general.h"
#include "eeprom.h"


/*******************************************************************************
* FUNCTION NAME: calibrate_pots
* PURPOSE:       store calibration values for crab & arms in EEPROM
* ARGUMENTS:     none
* RETURNS:       none
*******************************************************************************/
void calibrate_pots(void)
{
  static UINT8 eeprom_init_check_flag = FALSE;
  static UINT8 oi_sw_cal_crab_prev = 0;
  static UINT8 oi_sw_cal_b_arm_shoulder_prev = 0;
  static UINT8 oi_sw_cal_b_arm_elbow_prev = 0;
  static UINT8 oi_sw_cal_s_arm_shoulder_prev = 0;
  UINT8 bitmask;

#ifdef CALIBRATION_PRINTF
  printf("calibrate func  ");
#endif

  /* only check once if EEPROM has been initialized */
  if (eeprom_init_check_flag == FALSE)
  {
    /* only initialize EEPROM if the 'known' bits are not correct */
    if ((readEE(ADDR_KNOWN_BYTE1) != CALIBRATE_KNOWN_BYTE_1) ||
        (readEE(ADDR_KNOWN_BYTE2) != CALIBRATE_KNOWN_BYTE_2))
    {
#ifdef CALIBRATION_PRINTF
      printf("initializing EEPROM  ");
#endif
      /* initialize bitmasks to 0 */
      writeEE(ADDR_DATA_BITMASK_CRAB, 0);
      writeEE(ADDR_DATA_BITMASK_ARMS, 0);
      writeEE(ADDR_DATA_BITMASK3, 0);
      writeEE(ADDR_DATA_BITMASK4, 0);

      /* set 'known' bytes */
      writeEE(ADDR_KNOWN_BYTE1, CALIBRATE_KNOWN_BYTE_1);
      writeEE(ADDR_KNOWN_BYTE2, CALIBRATE_KNOWN_BYTE_2);
    }

    eeprom_init_check_flag = TRUE;
  }

  if ((Oi_sw_cal_crab == 1) && (oi_sw_cal_crab_prev == 0))
  {
    bitmask = readEE(ADDR_DATA_BITMASK_CRAB);

#ifdef CALIBRATION_PRINTF
    printf("crab ");
#endif

    if ((Oi_crab_x < (127 + CALIBRATE_ZONE)) &&
        (Oi_crab_x > (127 - CALIBRATE_ZONE)))
    {
      /* Joystick is in middle, calibrate wheels straight forward */
#ifdef CALIBRATION_PRINTF
      printf(" fwd  %d", (int)GET_ANALOG_VALUE_SHIFT(Analog_in_crab_pot));
#endif
      writeEE(ADDR_CRAB_FWD, GET_ANALOG_VALUE_SHIFT(Analog_in_crab_pot));

      bitmask |= CAL_MASK_CRAB_FWD;
      writeEE(ADDR_DATA_BITMASK_CRAB, bitmask);
    }
    else if (Oi_crab_x > (127 + CALIBRATE_ZONE))
    {
      /* Joystick is left, Calibrate Wheels to the left */
#ifdef CALIBRATION_PRINTF
      printf(" left  %d", (int)GET_ANALOG_VALUE_SHIFT(Analog_in_crab_pot));
#endif
      writeEE(ADDR_CRAB_180_LEFT, GET_ANALOG_VALUE_SHIFT(Analog_in_crab_pot));

      bitmask |= CAL_MASK_CRAB_LEFT;
      writeEE(ADDR_DATA_BITMASK_CRAB, bitmask);
    }
    else
    {
      /* Joystick is right, Calibrate Wheels to the right */
#ifdef CALIBRATION_PRINTF
      printf(" right  %d", (int)GET_ANALOG_VALUE_SHIFT(Analog_in_crab_pot));
#endif
      writeEE(ADDR_CRAB_180_RIGHT, GET_ANALOG_VALUE_SHIFT(Analog_in_crab_pot));

      bitmask |= CAL_MASK_CRAB_RIGHT;
      writeEE(ADDR_DATA_BITMASK_CRAB, bitmask);
    }
  }

#if 0
  /* big arm shoulder */
  if ((Oi_sw_cal_b_arm_shoulder == 1) && (oi_sw_cal_b_arm_shoulder_prev == 0))
  {
    bitmask = readEE(ADDR_DATA_BITMASK_ARMS);

#ifdef CALIBRATION_PRINTF
    printf("big shoulder ");
#endif

    if (Oi_big_arm_shoulder > (127 + CALIBRATE_ZONE))
    {
#ifdef CALIBRATION_PRINTF
      printf(" hi %d",
             (int)GET_ANALOG_VALUE_SHIFT(Analog_in_big_arm_shoulder_pot));
#endif
      writeEE(ADDR_B_ARM_SHOULDER_HI,
              GET_ANALOG_VALUE_SHIFT(Analog_in_big_arm_shoulder_pot));

      bitmask |= CAL_MASK_B_ARM_SHOULDER_HI;
      writeEE(ADDR_DATA_BITMASK_ARMS, bitmask);
    }
    else if (Oi_big_arm_shoulder < (127 - CALIBRATE_ZONE))
    {
#ifdef CALIBRATION_PRINTF
      printf(" lo %d",
          (int)GET_ANALOG_VALUE_SHIFT(Analog_in_big_arm_shoulder_pot));
#endif
      writeEE(ADDR_B_ARM_SHOULDER_LO,
          GET_ANALOG_VALUE_SHIFT(Analog_in_big_arm_shoulder_pot));

      bitmask |= CAL_MASK_B_ARM_SHOULDER_LO;
      writeEE(ADDR_DATA_BITMASK_ARMS, bitmask);
    }
  }

  /* big arm elbow */
  if ((Oi_sw_cal_b_arm_elbow == 1) && (oi_sw_cal_b_arm_elbow_prev == 0))
  {
    bitmask = readEE(ADDR_DATA_BITMASK_ARMS);

#ifdef CALIBRATION_PRINTF
    printf("big elbow ");
#endif

    if (Oi_big_arm_elbow < (127 - CALIBRATE_ZONE))
    {
#ifdef CALIBRATION_PRINTF
      printf(" hi %d",
             (int)GET_ANALOG_VALUE_SHIFT(Analog_in_big_arm_elbow_pot));
#endif
      writeEE(ADDR_B_ARM_ELBOW_HI,
              GET_ANALOG_VALUE_SHIFT(Analog_in_big_arm_elbow_pot));

      bitmask |= CAL_MASK_B_ARM_ELBOW_HI;
      writeEE(ADDR_DATA_BITMASK_ARMS, bitmask);
    }
    else if (Oi_big_arm_elbow > (127 + CALIBRATE_ZONE))
    {
#ifdef CALIBRATION_PRINTF
      printf(" lo %d",
          (int)GET_ANALOG_VALUE_SHIFT(Analog_in_big_arm_elbow_pot));
#endif
      writeEE(ADDR_B_ARM_ELBOW_LO,
          GET_ANALOG_VALUE_SHIFT(Analog_in_big_arm_elbow_pot));

      bitmask |= CAL_MASK_B_ARM_ELBOW_LO;
      writeEE(ADDR_DATA_BITMASK_ARMS, bitmask);
    }
  }
#endif

  /* small arm shoulder */
  if ((Oi_sw_cal_s_arm_shoulder == 1) && (oi_sw_cal_s_arm_shoulder_prev == 0))
  {
    bitmask = readEE(ADDR_DATA_BITMASK_ARMS);

#ifdef CALIBRATION_PRINTF
    printf("small shoulder ");
#endif

    if (Oi_small_arm_shoulder > (127 + CALIBRATE_ZONE))
    {
#ifdef CALIBRATION_PRINTF
      printf(" hi %d",
             (int)GET_ANALOG_VALUE_SHIFT(Analog_in_small_arm_shoulder_pot));
#endif
      writeEE(ADDR_S_ARM_SHOULDER_HI,
              GET_ANALOG_VALUE_SHIFT(Analog_in_small_arm_shoulder_pot));

      bitmask |= CAL_MASK_S_ARM_SHOULDER_HI;
      writeEE(ADDR_DATA_BITMASK_ARMS, bitmask);
    }
    else if (Oi_small_arm_shoulder < (127 - CALIBRATE_ZONE))
    {
#ifdef CALIBRATION_PRINTF
      printf(" lo %d",
          (int)GET_ANALOG_VALUE_SHIFT(Analog_in_small_arm_shoulder_pot));
#endif
      writeEE(ADDR_S_ARM_SHOULDER_LO,
          GET_ANALOG_VALUE_SHIFT(Analog_in_small_arm_shoulder_pot));

      bitmask |= CAL_MASK_S_ARM_SHOULDER_LO;
      writeEE(ADDR_DATA_BITMASK_ARMS, bitmask);
    }
  }


  oi_sw_cal_crab_prev = Oi_sw_cal_crab;
  oi_sw_cal_b_arm_shoulder_prev = Oi_sw_cal_b_arm_shoulder;
  oi_sw_cal_b_arm_elbow_prev = Oi_sw_cal_b_arm_elbow;
  oi_sw_cal_s_arm_shoulder_prev = Oi_sw_cal_s_arm_shoulder;

#ifdef CALIBRATION_PRINTF
  printf("\n");
#endif

  return;
}




/*******************************************************************************
* FUNCTION NAME: retrieve_calibration
* PURPOSE:       retrieve calibration values for crab & arms from EEPROM
* ARGUMENTS:     none
* RETURNS:       none
*******************************************************************************/
void retrieve_calibration(CalibrationValsType *calibration_vals)
{
  UINT8 verify1, verify2;
  UINT8 bitmask;

  verify1 = readEE(ADDR_KNOWN_BYTE1);
  verify2 = readEE(ADDR_KNOWN_BYTE2);

  if ((verify1 == CALIBRATE_KNOWN_BYTE_1) &&
      (verify2 == CALIBRATE_KNOWN_BYTE_2))
  {
    bitmask = readEE(ADDR_DATA_BITMASK_CRAB);
    /* crab forward */
    if ((bitmask & CAL_MASK_CRAB_FWD) != 0)
    {
      calibration_vals->crab_fwd = readEE(ADDR_CRAB_FWD);
    }
    else
    {
      calibration_vals->crab_fwd = DEFAULT_CRAB_FWD;
    }

    /* crab left */
    if ((bitmask & CAL_MASK_CRAB_LEFT) != 0)
    {
      calibration_vals->crab_180_left = readEE(ADDR_CRAB_180_LEFT);
    }
    else
    {
      calibration_vals->crab_180_left = DEFAULT_CRAB_LEFT;
    }

    /* crab right */
    if ((bitmask & CAL_MASK_CRAB_RIGHT) != 0)
    {
      calibration_vals->crab_180_right = readEE(ADDR_CRAB_180_RIGHT);
    }
    else
    {
      calibration_vals->crab_180_right = DEFAULT_CRAB_RIGHT;
    }

    bitmask = readEE(ADDR_DATA_BITMASK_ARMS);
#if 0
    /* big arm shoulder hi */
    if((bitmask & CAL_MASK_B_ARM_SHOULDER_HI) != 0)
    {
      calibration_vals->big_arm_shoulder_hi = readEE(ADDR_B_ARM_SHOULDER_HI);
    }
    else
    {
      calibration_vals->big_arm_shoulder_hi = DEFAULT_BIG_ARM_SHOULDER_HI;
    }

    /* big arm shoulder low */
    if((bitmask & CAL_MASK_B_ARM_SHOULDER_LO) != 0)
    {
      calibration_vals->big_arm_shoulder_lo = readEE(ADDR_B_ARM_SHOULDER_LO);
    }
    else
    {
      calibration_vals->big_arm_shoulder_lo = DEFAULT_BIG_ARM_SHOULDER_LO;
    }

    /* big arm elbow hi */
    if((bitmask & CAL_MASK_B_ARM_ELBOW_HI) != 0)
    {
      calibration_vals->big_arm_elbow_hi = readEE(ADDR_B_ARM_ELBOW_HI);
    }
    else
    {
      calibration_vals->big_arm_elbow_hi = DEFAULT_BIG_ARM_ELBOW_HI;
    }

    /* big arm elbow low */
    if((bitmask & CAL_MASK_B_ARM_ELBOW_LO) != 0)
    {
      calibration_vals->big_arm_elbow_lo = readEE(ADDR_B_ARM_ELBOW_LO);
    }
    else
    {
      calibration_vals->big_arm_elbow_lo = DEFAULT_BIG_ARM_ELBOW_LO;
    }
#endif

    /* small arm shoulder hi */
    if((bitmask & CAL_MASK_S_ARM_SHOULDER_HI) != 0)
    {
      calibration_vals->small_arm_shoulder_hi = readEE(ADDR_S_ARM_SHOULDER_HI);
    }
    else
    {
      calibration_vals->small_arm_shoulder_hi = DEFAULT_SMALL_ARM_SHOULDER_HI;
    }

    /* small arm shoulder low */
    if((bitmask & CAL_MASK_S_ARM_SHOULDER_LO) != 0)
    {
      calibration_vals->small_arm_shoulder_lo = readEE(ADDR_S_ARM_SHOULDER_LO);
    }
    else
    {
      calibration_vals->small_arm_shoulder_lo = DEFAULT_SMALL_ARM_SHOULDER_LO;
    }
  }
  else
  {
    calibration_vals->crab_fwd = DEFAULT_CRAB_FWD;
    calibration_vals->crab_180_left = DEFAULT_CRAB_LEFT;
    calibration_vals->crab_180_right = DEFAULT_CRAB_RIGHT;
    calibration_vals->big_arm_shoulder_hi = DEFAULT_BIG_ARM_SHOULDER_HI;
    calibration_vals->big_arm_shoulder_lo = DEFAULT_BIG_ARM_SHOULDER_LO;
    calibration_vals->big_arm_elbow_hi = DEFAULT_BIG_ARM_ELBOW_HI;
    calibration_vals->big_arm_elbow_lo = DEFAULT_BIG_ARM_ELBOW_LO;
    calibration_vals->small_arm_shoulder_hi = DEFAULT_SMALL_ARM_SHOULDER_HI;
    calibration_vals->small_arm_shoulder_lo = DEFAULT_SMALL_ARM_SHOULDER_LO;
  }

  return;
}



/*******************************************************************************
* FUNCTION NAME: display_calibration
* PURPOSE:       display joystick vals & pot values for crab & arms on OI
* ARGUMENTS:     none
* RETURNS:       none
*
* To display pot values on the OI:
*   - enable calibration mode
*   - move the joystick axis that corresponds to the pot (crab stick X,
*     big arm Y, big arm X, small arm X) full forward or left (depending on
*     the axis) and allow it to return to the middle
*   - now the OI displays the pot value
*
* To display joystick values on the OI:
*   - enable calibration mode
*   - enable manual crab mode
*   - move the joystick axis that you want to display full forward or left
*     (depending on the axis) and allow it to return to the middle
*   - now the OI displays the X/Y axis of the stick
*
*******************************************************************************/
void display_calibration(void)
{
  static DISPLAY_TYPE display_type = DISPLAY_NONE;
  UINT8 display_data = 0;

  if (Oi_calibrate > OI_CALIBRATE_JOYSTICKS)
  {
    /* print joystick values when switch is to joystick side */
    if (Oi_drive_x >= (127 + CALIBRATE_ZONE))
    {
      display_type = DISPLAY_DRIVE_X;
    }
    else if (Oi_drive_y >= (127 + CALIBRATE_ZONE))
    {
      display_type = DISPLAY_DRIVE_Y;
    }
    else if (Oi_crab_x >= (127 + CALIBRATE_ZONE))
    {
      display_type = DISPLAY_CRAB_X;
    }
    else if (Oi_crab_y >= (127 + CALIBRATE_ZONE))
    {
      display_type = DISPLAY_CRAB_Y;
    }
    else if (Oi_big_arm_shoulder >= (127 + CALIBRATE_ZONE))
    {
      display_type = DISPLAY_BIG_ARM_SHOULDER;
    }
    else if (Oi_big_arm_elbow >= (127 + CALIBRATE_ZONE))
    {
      display_type = DISPLAY_BIG_ARM_ELBOW;
    }
    else if (Oi_small_arm_shoulder >= (127 + CALIBRATE_ZONE))
    {
      display_type = DISPLAY_SMALL_ARM_SHOULDER;
    }
  }
  else
  {
    /* print pot values when switch is to calibrate pot side */
    if (Oi_crab_x >= (127 + CALIBRATE_ZONE))
    {
      display_type = DISPLAY_CRAB_POT;
    }
    else if (Oi_big_arm_shoulder >= (127 + CALIBRATE_ZONE))
    {
      display_type = DISPLAY_BIG_ARM_SHOULDER_POT;
    }
    else if (Oi_big_arm_elbow >= (127 + CALIBRATE_ZONE))
    {
      display_type = DISPLAY_BIG_ARM_ELBOW_POT;
    }
    else if (Oi_small_arm_shoulder >= (127 + CALIBRATE_ZONE))
    {
      display_type = DISPLAY_SMALL_ARM_SHOULDER_POT;
    }
  }

  switch (display_type)
  {
    case DISPLAY_DRIVE_X:
      display_data = Oi_drive_x;
      break;

    case DISPLAY_DRIVE_Y:
      display_data = Oi_drive_y;
      break;

    case DISPLAY_CRAB_X:
      display_data = Oi_crab_x;
      break;

    case DISPLAY_CRAB_Y:
      display_data = Oi_crab_y;
      break;

    case DISPLAY_BIG_ARM_SHOULDER:
      display_data = Oi_big_arm_shoulder;
      break;

    case DISPLAY_BIG_ARM_ELBOW:
      display_data = Oi_big_arm_elbow;
      break;

    case DISPLAY_SMALL_ARM_SHOULDER:
      display_data = Oi_small_arm_shoulder;
      break;

    case DISPLAY_CRAB_POT:
      display_data = GET_ANALOG_VALUE_SHIFT(Analog_in_crab_pot);
      break;

    case DISPLAY_BIG_ARM_SHOULDER_POT:
      display_data = GET_ANALOG_VALUE_SHIFT(Analog_in_big_arm_shoulder_pot);
      break;

    case DISPLAY_BIG_ARM_ELBOW_POT:
      display_data = GET_ANALOG_VALUE_SHIFT(Analog_in_big_arm_elbow_pot);
      break;

    case DISPLAY_SMALL_ARM_SHOULDER_POT:
      display_data = GET_ANALOG_VALUE_SHIFT(Analog_in_small_arm_shoulder_pot);
      break;

    default:
      break;
  }

  display_oi_data(display_data, DISPLAY_DATA_CALIBRATE);

  return;
}


