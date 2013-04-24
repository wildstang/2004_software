#include "serial_utils.h"
#include "oi_data.h"

/*******************************************************************************
* FUNCTION NAME: GetOIData
* PURPOSE:       Obtain OI data from the 2003 RC
* ARGUMENTS:     OIData: pointer to struct to store incoming OI data
* RETURNS:       1 if successful, 0 if error
*******************************************************************************/
char GetOIData(oi_data *OIData)
{
    unsigned char temp = 0;
    int i;

    /* Clear out any previous RX error that may be present */
    ClearRXError();
        
    /* Read up to sizeof(oi_data) + 1 bytes from the serial port while
       searching for 0xFF, 0xFF */
    for(i = 0; (i < (sizeof(oi_data) + 1)) && (temp != 0xFF); i++)
    {
        GetByte(&temp);
        if(temp == 0xFF)
        {
            temp = 0;
            GetByte(&temp);
            i++;
        }
    }
    
    if(temp == 0xFF)
    {
        /* Found our 0xFF, 0xFF sync sequence, go ahead and read the OI data */
        if(SerRead(OIData, sizeof(oi_data)) == sizeof(oi_data))
        {
            /* If comp_mode = 0, default all values */
            if(OIData->PB_mode.comp_mode == 1)
            {
                DefaultOIData(OIData);
            }
            
            /* We read in the full structure from the OI */
            return 1;
        }
        /* SerRead did not get all of the data, so send back an error */
        return 0;
    }
}
    
/*******************************************************************************
* FUNCTION NAME: DefaultOIData
* PURPOSE:       Set the oi_data structure to safe default values
* ARGUMENTS:     OIData: pointer to struct to store default OI data
* RETURNS:       void
*******************************************************************************/
void DefaultOIData(oi_data *OIData)
{
    OIData->p1_x = 127;
    OIData->p2_x = 127;
    OIData->p3_x = 127;
    OIData->p4_x = 127;

    OIData->p1_y = 127;
    OIData->p2_y = 127;
    OIData->p3_y = 127;
    OIData->p4_y = 127;
    
    OIData->p1_wheel = 127;
    OIData->p2_wheel = 127;
    OIData->p3_wheel = 127;
    OIData->p4_wheel = 127;

    OIData->p1_aux = 127;
    OIData->p2_aux = 127;
    OIData->p3_aux = 127;
    OIData->p4_aux = 127;

    OIData->swA.allbits = 0;
    OIData->swB.allbits = 0;
}
