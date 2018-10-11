
#include "EF_Anemometer.h"







/***********************************************************************************
 * Function    : void EF_Anemometer_init(void);
 *
 * DESCRIPTION : this function used to init the Anemometer module.
 *
 * PARAMETERS  : NONE.

 * Return Value: NONE.
 *****************************************************************************************/

void EF_Anemometer_init(void)
{
	EF_void_ADC_init();
}



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

U8_t EF_U8_Anemometer_GetWindSpeed(void)
{

	U16_t  adc_read   = 0;
    float  adc_inVolt = 0;
	U16_t  wind_speed = 0;

	adc_read = EF_u16_ADC_read(WIND_SPEED_ADC_CHANNEL);
	adc_inVolt =  ((float)adc_read * ADC_VOLT_REFRANCE) / ADC_MAX_VOLT_LEVEL;

	/* Convert the reading volt to wind speed according to data sheet
	 * Formula: V = 6*U */
	wind_speed = adc_inVolt * VOLT2WIND_FACTOR;

return wind_speed;
}
