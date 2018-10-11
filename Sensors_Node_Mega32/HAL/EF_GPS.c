
#include "EF_GPS.h"
#include <stdio.h>




U8_t gps_data_Rmcfram[80]  = {0};


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
void EF_Gps_init()
{
//	EF_void_TimerInit();
//	EF_void_UART_Init(&uart_cfg);
	EF_void_TimerCreate(GPS_FRAM_ID, GPS_WAIT_FRAM);
}




/****************************************************************************
* Function    : EF_B_Gps_getTimeDate.
*
* DESCRIPTION : used to get the time and date from gps frame RMC.
*
* PARAMETERS  : str_TimeDate_info* timeDate_str --> pointer to struct
* 					to return time and date.
*
* Return Value: enum of the gps status.
*
******************************************************************************/

enu_timeDate_ret EF_U8_Gps_getTimeDate(str_TimeDate_info* timeDate_str)
{

	enu_timeDate_ret enu_timeDate_status = gps_enu_init;
	U8_t static Data_Rmcfram_index       =  0;
	U8_t time_msg_ary[TIME_MSG_LENGTH]   = {0};
	U8_t date_msg_ary[DATE_MSG_LENGTH]   = {0};
	U8_t Temp_hour[2]                    = {0};
	U8_t Temp_minut[2]  			     = {0};
	U8_t Temp_second[2] 			     = {0};
	U8_t Temp_year[2]                    = {0};
	U8_t Temp_month[2]                   = {0};
	U8_t Temp_day[2]                     = {0};
	U8_t recvd_fram_flag                 = FALSE;
	U8_t recvd_byte      			     = 0;
	U8_t comma_count                     = 0;
	U8_t iteretor        			     = 0;

	EF_void_TimerStart(GPS_FRAM_ID);
	while (EF_BOOLEAN_TimerCheck(GPS_FRAM_ID) == 0 )
	{
		EF_BOOLEAN_UART_GetChar(&recvd_byte);
		if(recvd_byte == '$')
		{
//			EF_void_UART_SendString(1,"found Sign\n");
			EF_BOOLEAN_UART_GetChar(&recvd_byte);
			if (recvd_byte == 'G')
			{
				EF_BOOLEAN_UART_GetChar(&recvd_byte);
//				EF_void_UART_SendString(1,"found G\n");
				if (recvd_byte == 'P')
				{
					EF_BOOLEAN_UART_GetChar(&recvd_byte);
					if (recvd_byte == 'R')
					{
						EF_BOOLEAN_UART_GetChar(&recvd_byte);
						if(recvd_byte == 'M')
						{
							EF_BOOLEAN_UART_GetChar(&recvd_byte);
							if (recvd_byte == 'C')
							{
//								EF_void_UART_SendString(1,"found fram\n");
								Data_Rmcfram_index = 0;
								do
								{
									EF_BOOLEAN_UART_GetChar(&gps_data_Rmcfram[Data_Rmcfram_index]);
									Data_Rmcfram_index += 1;
								}while((gps_data_Rmcfram[Data_Rmcfram_index-1] != 0x0A)&& (gps_data_Rmcfram[Data_Rmcfram_index] != 0x0D));
								/* if RMC fram has found */
								recvd_fram_flag = TRUE;
								break;

							}/*end of check 'C' */
						}
					}
				}
			}
		}
	}/*end of while*/
	EF_void_TimerStop(GPS_FRAM_ID);
	EF_void_TimerReset(GPS_FRAM_ID);
	/* search for time in receiving array */
	for(U8_t time_index=0; time_index < TIME_MSG_LENGTH; time_index++)
	{
		time_msg_ary[time_index] = gps_data_Rmcfram[time_index+1];
		/* if received next comma break */
		if(time_msg_ary[time_index] == ',' )
		{
			break;
		}
	}

	/* search for date in receiving array */
	for(U8_t Date_index = 0; Date_index < Data_Rmcfram_index; Date_index++)
	{
		if( gps_data_Rmcfram[Date_index] == ',' )
		{
			comma_count++;
			if(comma_count == NO_FREQUNT_COMMA )
			{
				comma_count = 0;
				for(iteretor=0; iteretor < DATE_MSG_LENGTH; iteretor++)
				{
					date_msg_ary[iteretor] = gps_data_Rmcfram[Date_index + iteretor + 1];
					/* if received next comma break */
					if(date_msg_ary[iteretor] == ',')
					{
						break;
					}

				}
			}
		}
	}

/* if received frame RMC correct  */
	if(recvd_fram_flag == TRUE)
	{
	    /* Extract the time from time array */
		Temp_hour[0]    = time_msg_ary[0];
		Temp_hour[1]    = time_msg_ary[1];
		Temp_minut[0]   = time_msg_ary[2];
		Temp_minut[1]   = time_msg_ary[3];
		Temp_second[0]  = time_msg_ary[4];
		Temp_second[1]  = time_msg_ary[5];
		/* Extract the date form date array */
		Temp_day[0]     = date_msg_ary[0];
		Temp_day[1]     = date_msg_ary[1];
		Temp_month[0]   = date_msg_ary[2];
		Temp_month[1]   = date_msg_ary[3];
		Temp_year[0]    = date_msg_ary[4];
		Temp_year[1]    = date_msg_ary[5];

		/* check the range of the values */
		timeDate_str->second  = char2int((char*)Temp_second,2);
		timeDate_str->minut   = char2int((char*)Temp_minut,2);
		timeDate_str->hour    = char2int((char*)Temp_hour,2);
		timeDate_str->day     = char2int((char*)Temp_day,2);
		timeDate_str->month   = char2int((char*)Temp_month,2);
	    timeDate_str->year    = char2int((char*)Temp_year,2);
	    /* time zone to Egypt add two hours to the time from gps to
	     * match Egypt's Time  */
	    if(timeDate_str->hour == 22 )
	    {
	    	timeDate_str->hour = 0;    // 12 AM
	    }
	    else if(timeDate_str->hour == 23)
	    {
	    	timeDate_str->hour = 1;
	    }
	    else{
	    	timeDate_str->hour +=2 ;
	    }
		/* check the values range for time  */
		if( ((timeDate_str->second  >= 0) && (timeDate_str->second  < MAX_NUM_SECONDS)) )
		{
			if( ((timeDate_str->minut >= 0) && (timeDate_str->minut < MAX_NUM_MINUTES)) )
			{
				if( ((timeDate_str->hour >= 0) && (timeDate_str->hour < MAX_NUM_HOURS)) )
				{
					enu_timeDate_status   = gps_timeDateRange_correct;

				}else enu_timeDate_status = gps_timeRange_Error;
			}else     enu_timeDate_status = gps_timeRange_Error;
		}else         enu_timeDate_status = gps_timeRange_Error;


		/* check the values range for time */
		if(((timeDate_str->day > 0) && (timeDate_str->day <= MAX_NUM_DAYS)) )
		{
			if( ((timeDate_str->month  > 0) && (timeDate_str->month  <= MAX_NUM_MONTHS)) )
			{
				if( ((timeDate_str->year >= 18) && (timeDate_str->year < MAX_NUM_YEARS)) )
				{
					  enu_timeDate_status = gps_timeDateRange_correct;

				}else enu_timeDate_status = gps_dateRange_Error;
			}else     enu_timeDate_status = gps_dateRange_Error;
		}else         enu_timeDate_status = gps_dateRange_Error;


#if GPS_DBUG == 1

		EF_void_UART_SendString( "\n");
		EF_void_UART_SendString( "hour=");
		EF_void_UART_Send_Integer( timeDate_str->hour);
		EF_void_UART_SendString( " ");
		EF_void_UART_SendString( "min=");
		EF_void_UART_Send_Integer( timeDate_str->minut);
		EF_void_UART_SendString( " ");
		EF_void_UART_SendString( "sec=");
		EF_void_UART_Send_Integer( timeDate_str->second);
		EF_void_UART_SendString( "     ");

		EF_void_UART_SendString( "day=");
		EF_void_UART_Send_Integer( timeDate_str->day);
		EF_void_UART_SendString( " ");
		EF_void_UART_SendString( "month=");
		EF_void_UART_Send_Integer( timeDate_str->month);
		EF_void_UART_SendString( " ");
		EF_void_UART_SendString( "year=");
		EF_void_UART_Send_Integer( timeDate_str->year);
		EF_void_UART_SendString("\n");
#endif
	}
	else
	{
		enu_timeDate_status = gps_timeOut;
	}


return enu_timeDate_status;
}


/* convert character array to integer */
int char2int (char *array, size_t n)
{
	int number = 0;
	int mult = 1;

	n = (int)n < 0 ? -n : n;       /* quick absolute value check  */

	/* for each character in array */
	while (n--)
	{
		/* if not digit or '-', check if number > 0, break or continue */
		if ((array[n] < '0' || array[n] > '9') && array[n] != '-') {
			if (number)
				break;
			else
				continue;
		}

		if (array[n] == '-') {      /* if '-' if number, negate, break */
			if (number) {
				number = -number;
				break;
			}
		}
		else {                      /* convert digit to numeric value   */
			number += (array[n] - '0') * mult;
			mult *= 10;
		}
	}

	return number;
}
