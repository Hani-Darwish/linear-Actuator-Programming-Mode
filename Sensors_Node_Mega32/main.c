
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

	U8_t data_recvd_pipe = 0;
//	U8_t data_readyFlag
	U8_t blocks_error[12] = {0};
	U8_t blocks_error_number = 0;
	U8_t bufferin[32] = {0};

	str_received_data recv_data;

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



		/* programin the first time   */
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
			nrf_txData_status = Send_EndPrograming();

		}


		/* Check the block fault in programming  */
		if(nrf_txData_status == nrf_transmit_success)
		{
			// change mode to rx
			nrf24l01_setRXMode();
			do
			{
				if(EF_nrf24l01_readready(&data_recvd_pipe))
				{
					EF_void_UART_SendString("Programming report frame received \n");

					/* read buffer */
					EF_nrf24l01_readData(bufferin,32);
					Receive_ErrorFrame(bufferin, blocks_error, &blocks_error_number);

				}/* end of read data */
				/* wait until receiving any data TODO check time out to be unstuck*/
			}while(data_recvd_pipe == 0);

			data_recvd_pipe = 0;
		}

		nrf24l01_setTXMode();
		nrf24l01_settxaddr(addrtx2);
		/* Reprogramming the fault blocks */
		EF_void_UART_SendString(" fualt number ");
		EF_void_UART_Send_Integer(blocks_error_number);
		EF_void_UART_SendString("\n");
		if(!(blocks_error_number == 0))
		{
			EF_void_UART_SendString(" Reprogramming the fault blocks  ");

			nrf_txData_status = Send_StartPrograming();
			if(nrf_txData_status == nrf_transmit_success)
			{

				for(U8_t i = 0; i < blocks_error_number; i++)
				{
					Send_ProgramingBlock(blocks_error[i]);
					//				month_index++;
				}

				month_index = 1;
				/* TODO check this later*/
				nrf_txData_status = Send_EndPrograming();

			}

		}



		_delay_ms(10000);

	}/* superloop */

	return 0;
}
