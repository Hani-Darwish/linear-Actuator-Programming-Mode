

#include "../MCAL/EF_ADC.h"




#define WIND_SPEED_ADC_CHANNEL    4
#define ADC_VOLT_REFRANCE         5
#define ADC_MAX_VOLT_LEVEL        1024
#define VOLT2WIND_FACTOR          6



/***********************************************************************************
 * Function    : void EF_Anemometer_init(void);
 *
 * DESCRIPTION : this function used to init the Anemometer module.
 *
 * PARAMETERS  : NONE.

 * Return Value: NONE.
 *****************************************************************************************/

void EF_Anemometer_init(void);


/***********************************************************************************
 * Function    : U8_t EF_U8_Anemometer_GetWindSpeed(void);
 *
 * DESCRIPTION : this function is used to get the wind speep
 *
 * PARAMETERS  : void.
 *
 * Return Value: the measured value.
 *
 * NOTE:        Formula: wind value  = 6 * volt reading
 *****************************************************************************************/
U8_t EF_U8_Anemometer_GetWindSpeed(void);
