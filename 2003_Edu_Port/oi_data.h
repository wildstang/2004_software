#ifndef OI_DATA_H
#define OI_DATA_H

#include "ifi_default.h" /* For struct bitid */

typedef struct
{
    unsigned int p12_trig:1;
    unsigned int p12_top:1;
    unsigned int p12_aux1:1;
    unsigned int p12_aux2:1;
    unsigned int p34_trig:1;
    unsigned int p34_top:1;
    unsigned int p34_aux1:1;
    unsigned int p34_aux2:1;
} joy_buttons;
        
typedef struct
{
    unsigned int :6;
    unsigned int auton_mode:1;
    unsigned int comp_mode:1;
} PB_mode_type;

/* IF YOU CHANGE THIS STRUCTURE you must also modify OIPassthru.bsx!
   The order of variables in this struct must match the order of variables
   being sent out with the SEROUT command in that file. */
typedef struct
{
    unsigned char p1_x;
    unsigned char p2_x;
    unsigned char p3_x;
    unsigned char p4_x;
    
    union
    {
        bitid bitselect;
        unsigned char allbits;
        joy_buttons p1_p3_sw;
    } swA;
    
    unsigned char p1_y;
    unsigned char p2_y;
    unsigned char p3_y;
    unsigned char p4_y;
    
    union
    {
        bitid bitselect;
        unsigned char allbits;
        joy_buttons p2_p4_sw;
    } swB;
    
    unsigned char p1_wheel;
    unsigned char p2_wheel;
    unsigned char p3_wheel;
    unsigned char p4_wheel;

    PB_mode_type PB_mode;
        
    unsigned char p1_aux;
    unsigned char p2_aux;
    unsigned char p3_aux;
    unsigned char p4_aux;
} oi_data;

char GetOIData(oi_data *OIData);
void DefaultOIData(oi_data *OIData);

#endif /* #ifndef OI_DATA_H */
