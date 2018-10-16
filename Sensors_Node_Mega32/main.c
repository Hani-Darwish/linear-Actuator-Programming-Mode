
#include "HAL/EF_GPS.h"
#include "HAL/nrf24l01.h"
#include "HAL/EF_Anemometer.h"
#include <util/delay.h>
#include <string.h>
#include "ServiceLayer/EF_Frame.h"
#include "ServiceLayer/Program_Handl.h"


#define DEBUGENABLED       1
#define RF_CHANL_1		   92
#define RF_CHANL_2		   57


//
//extern U8_t g_FramBuild[MAX_FRAM_SIZE];
//extern U16_t g_Packet_ID;

U8_t g_rfChanl = RF_CHANL_1;
static UART_cfg_str uart_cfg1 = {9600, 8, ONE_STOP_BIT, NO_PARITY, FALSE, FALSE, TRUE, TRUE};


int main(void)
{

	U8_t data_recvd_pipe = 0;
	U8_t blocks_error[12] = {0};
	U8_t blocks_error_number = 12;
	U8_t bufferin[32] = {0};

	U8_t sendpipe = NRF24L01_DATA_PIPE;
	wrt_nrf_DataStatus nrf_txData_status = nrf_transmit_failed;

	/* initialization */
	EF_void_TimerInit();
	EF_void_UART_Init(&uart_cfg1);

	EF_nrf24l01_init(g_rfChanl);

	/* set addresses */
//	uint8_t addrtx0[NRF24L01_ADDRSIZE] = NRF24L01_ADDRP0;
//	uint8_t addrtx1[NRF24L01_ADDRSIZE] = NRF24L01_ADDRP1;
	uint8_t addrtx2[NRF24L01_ADDRSIZE] = NRF24L01_ADDRP2;
//	uint8_t addrtx3[NRF24L01_ADDRSIZE] = NRF24L01_ADDRP3;
//	uint8_t addrtx4[NRF24L01_ADDRSIZE] = NRF24L01_ADDRP4;
//	uint8_t addrtx5[NRF24L01_ADDRSIZE] = NRF24L01_ADDRP5;


#if DEBUGENABLED == 1
	nrf24l01_printinfo(EF_void_UART_SendString, EF_BOOLEAN_UART_PutChar);
#endif

	EF_void_TimerCreate(NRF_SEND_ID, NRF_SEND_TIMEOUT);


	for(U8_t index = 0; index <12; index++)
	{
		blocks_error[index] = index+1;

	}

#if DEBUGENABLED == 1
		char pipebuffer[5];
		EF_void_UART_SendString("\nsending data on, pipe");
		itoa(sendpipe, pipebuffer, 10);
		EF_void_UART_SendString(pipebuffer);
		EF_void_UART_SendString("\n");
#endif


	while(1)
	{


		 /* set tx address for pipe 2 */
		nrf24l01_settxaddr(addrtx2);

		/* Programming the first time   */
		nrf_txData_status = Send_StartPrograming();
		if(nrf_txData_status == nrf_transmit_success)
		{
//			for(U8_t index = 0; index < blocks_error_number; index++)
			for(U8_t index = 0; index < 12; index++)
			{
//				Send_ProgramingBlock(blocks_error[index]);
				Send_ProgramingBlock(index+1);
			}
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

		_delay_ms(4000);

		/* change the channel   */
		if(g_rfChanl == RF_CHANL_1)
		{
			g_rfChanl = RF_CHANL_2;
		}
		else if(g_rfChanl == RF_CHANL_2)
		{
			g_rfChanl = RF_CHANL_1;
		}
		/*reinit channel */
		EF_nrf24l01_init(g_rfChanl);
		nrf24l01_setTXMode();






	}/* superloop */

	return 0;
}
