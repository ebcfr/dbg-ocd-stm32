#include <stdio.h>
#include <string.h>
#include "include/board.h"
#include "lib/io.h"

/***************************************************************************
 * UART with DMA transmit and receive test
 *
 ***************************************************************************/

char buf_tx[64]="\r\nThis message comes from STM32F411 :-)\r\n";
char buf_rx[64];

int main()
{
	// enable USART2 clocking
	_RCC->APB1ENR |= 1<<17;
	
	// configure Tx/Rx pins : Tx --> PA2, Rx --> PA3
	io_configure(USART2_GPIO_PORT, USART2_GPIO_PINS, USART2_GPIO_CONFIG, NULL);
	
	// transmission speed : 115200 bauds
    _USART2->BRR = (sysclks.apb1_freq / 115200);
    
	_USART2->GTPR = 0;
	
	// config 8 bit data, 1 stop bit, no parity
	_USART2->CR1 = (1<<3) | (1<<2) | (1<<13);
	_USART2->CR2 = 0;
	_USART2->CR3 = 3<<6; // DMA Transmit and receive
	
	// Config DMA to transmit a string
	_RCC->AHB1ENR |= 1<<21;
	_DMA1->IFCR[1]      = 0x3D<<16;
	_DMA1_Stream6->M0AR = (uint32_t)buf_tx;
	_DMA1_Stream6->PAR  = 0x40004404;
	_DMA1_Stream6->NDTR = strlen(buf_tx);
	_DMA1_Stream6->FCR  = 0;
	_DMA1_Stream6->CR   = (4<<25) | (1<<10) | (1<<6) | (1<<0);
	
	while(_DMA1_Stream6->CR & 1) ;
	
	while(1) {
		// read 10 char data
		_DMA1->IFCR[1]      = 0x3D<<6;
		_DMA1_Stream5->M0AR = (uint32_t)buf_rx;
		_DMA1_Stream5->PAR  = 0x40004404;
		_DMA1_Stream5->NDTR = 10;
		_DMA1_Stream5->FCR  = 0;
		_DMA1_Stream5->CR   = (4<<25) | (1<<10) | (1<<0);
		// with Peripheral Flow Controller
		//_DMA1_Stream5->CR   = (4<<25) | (1<<10) | (1<<0) | (1<<5);
		while(_DMA1_Stream5->CR & 1) ;
		buf_rx[10]=0;
		// echo the data
		_DMA1->IFCR[1]      = 0x3D<<16;
		_DMA1_Stream6->M0AR = (uint32_t)buf_rx;
		_DMA1_Stream6->PAR  = 0x40004404;
		_DMA1_Stream6->NDTR = strlen(buf_rx);
		_DMA1_Stream6->FCR  = 0;
		_DMA1_Stream6->CR   = (4<<25) | (1<<10) | (1<<6) | (1<<0);
		while(_DMA1_Stream6->CR & 1) ; 
	}
	return 1;
}
