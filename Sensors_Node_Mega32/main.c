
#include "HAL/EF_GPS.h"
#include "HAL/nrf24l01.h"
#include "HAL/EF_Anemometer.h"
#include <util/delay.h>
#include <string.h>
#include "ServiceLayer/EF_Frame.h"
#include "ServiceLayer/Program_Handl.h"


#define POSITION_LENGTH    210
#define DEBUGENABLED       1



extern U8_t g_FramBuild[MAX_FRAM_SIZE];
extern U8_t g_rfChanl;
extern U16_t g_Packet_ID;



U8_t one_time_flag = 0;

U8_t position[POSITION_LENGTH] = {0};

U8_t sensors_read[21] = {0};
U8_t ACK_read[32] = {0};


static UART_cfg_str uart_cfg1 = {9600, 8, ONE_STOP_BIT, NO_PARITY, FALSE, FALSE, TRUE, TRUE};




int main(void)
{

	unsigned int index = 0;
	unsigned char j    = 0;



	unsigned int sucuss_count = 0;
	unsigned int fail_count = 0;

	static U8_t month_index = 1;


	U8_t sendpipe = NRF24L01_DATA_PIPE;
	wrt_nrf_DataStatus nrf_txData_status = nrf_transmit_failed;
	U8_t ack_byte = 0;

	/* initialization */
	EF_void_TimerInit();
	EF_void_UART_Init(&uart_cfg1);

	EF_Gps_init();
	EF_Anemometer_init();
	EF_nrf24l01_init(g_rfChanl);

	/* set addresses */
	uint8_t addrtx0[NRF24L01_ADDRSIZE] = NRF24L01_ADDRP0;
	uint8_t addrtx1[NRF24L01_ADDRSIZE] = NRF24L01_ADDRP1;
	uint8_t addrtx2[NRF24L01_ADDRSIZE] = NRF24L01_ADDRP2;
	uint8_t addrtx3[NRF24L01_ADDRSIZE] = NRF24L01_ADDRP3;
	uint8_t addrtx4[NRF24L01_ADDRSIZE] = NRF24L01_ADDRP4;
	uint8_t addrtx5[NRF24L01_ADDRSIZE] = NRF24L01_ADDRP5;


#if DEBUGENABLED == 1
	nrf24l01_printinfo(EF_void_UART_SendString, EF_BOOLEAN_UART_PutChar);
#endif

	EF_void_TimerCreate(NRF_SEND_ID, NRF_SEND_TIMEOUT);

	while(1)
	{

#if DEBUGENABLED == 1
		char pipebuffer[5];
		EF_void_UART_SendString("\nsending data on, pipe");
		itoa(sendpipe, pipebuffer, 10);
		EF_void_UART_SendString(pipebuffer);
		EF_void_UART_SendString("\n");
#endif

		if(sendpipe == 0)
		{
			nrf24l01_settxaddr(addrtx0);              /* set tx address for pipe 0 */
		}
		else if(sendpipe == 1)
		{
			nrf24l01_settxaddr(addrtx1);             /* set tx address for pipe 1 */
		}
		else if(sendpipe == 2)
		{
			nrf24l01_settxaddr(addrtx2);   		     /* set tx address for pipe 2 */
		}
		else if(sendpipe == 3)
		{
			nrf24l01_settxaddr(addrtx3);             /* set tx address for pipe 3 */
		}
		else if(sendpipe == 4)
		{
			nrf24l01_settxaddr(addrtx4);    		/* set tx address for pipe 4 */
		}
		else if(sendpipe == 5)
		{
			nrf24l01_settxaddr(addrtx5);            /* set tx address for pipe 5 */
		}




		nrf_txData_status = Send_StartPrograming();

		if(nrf_txData_status == nrf_transmit_success)
		{
			for(U8_t i = 0; i < 12; i++)
			{
				Send_ProgramingBlock(month_index);
				month_index++;
			}

			month_index = 1;
			/* TODO check this later*/
			Send_EndPrograming();

		}

		_delay_ms(10000);




//		for(unsigned int k=0; k < 366; k++ )
//		{
////			EF_nrf_Build_Fram((U8_t)SOURCE_ID, (U8_t)DESTINATION_ID,(U8_t)21, sensors_read);
////			nrf_txData_status = EF_nrf24l01_writeData(g_FramBuild,32);
//
//
//			_delay_ms(150);
//			if(nrf_txData_status == nrf_transmit_success )
//			{
//				sucuss_count++;
//			}
//			else
//			{
//				fail_count++;
//			}
//		}
//
//		EF_void_UART_SendString("succes value ");
//		EF_void_UART_Send_Integer(sucuss_count);
//		EF_void_UART_SendString("\n");
//
//		EF_void_UART_SendString("fail value ");
//		EF_void_UART_Send_Integer(fail_count);
//		EF_void_UART_SendString("\n");

//		_delay_ms(5000);

//		/* change the sending channel */
//		if(g_rfChanl == 92)
//		{
//			g_rfChanl = 76;
//
//		}
//		else if(g_rfChanl == 76)
//		{
//			g_rfChanl = 92;
//		}
//
//		EF_nrf24l01_init(g_rfChanl);
//		_delay_ms(70);
//		g_Packet_ID = 0;









	}/* superloop */

	return 0;
}
