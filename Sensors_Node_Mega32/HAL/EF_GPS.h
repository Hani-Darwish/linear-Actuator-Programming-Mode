

#include "../Utilities/std_types.h"
#include "../MCAL/EF_UART.h"
#include "../Utilities/EF_SpechialTIMER.h"
#include <stdlib.h>


#define GPS_DBUG            1
#define GPS_UART_NUM        1
#define TIME_MSG_LENGTH     6     /* number of bytes of time within the GPS frame */
#define DATE_MSG_LENGTH     6	  /* number of bytes of date within GPS frame */
#define NO_FREQUNT_COMMA    9     /* number of frequency of comma char to get date behind it */
#define MAX_NUM_DAYS        31
#define MAX_NUM_MONTHS      12
#define MAX_NUM_YEARS       99
#define MAX_NUM_SECONDS     60
#define MAX_NUM_MINUTES     60
#define MAX_NUM_HOURS       24

/* typedef for the data extracted from gps */
typedef struct
{
	U8_t second;
	U8_t minut;
	U8_t hour;
	U8_t day;
	U8_t month;
	U8_t year;
}str_TimeDate_info;


/*typedef for gps time and date return  */

typedef enum
{
	gps_enu_init,
	gps_timeDateRange_correct,
	gps_timeRange_Error,
	gps_dateRange_Error,
	gps_timeOut

}enu_timeDate_ret;

/****************************************************************************
* Function    : void EF_Gps_init(void);
*
* DESCRIPTION : init gps module .
*
* PARAMETERS  : void
*
* Return Value: None.
*
******************************************************************************/
void EF_Gps_init(void);

/****************************************************************************
* Function    : EF_B_Gps_getTimeDate.
*
* DESCRIPTION : used to get the time and date from gps frame RMC.
*
* PARAMETERS  : str_TimeDate_info* timeDate_str --> pointer to struct
* 					to return time and date.
*
* Return Value: None.
*
******************************************************************************/
enu_timeDate_ret EF_U8_Gps_getTimeDate(str_TimeDate_info* timeDate_str);


int char2int (char *array, size_t n);


