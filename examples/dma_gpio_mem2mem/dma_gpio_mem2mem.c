// DMA GPIO Output Example - this example shows
// how you can output 8 pins all simultaneously
// with a planned bit pattern at 8MSamples/s. This
// mechanism is called "mem2mem" and lets the DMA
// run as fast as possible and is not affected
// by the latency of a timer trigger.
// Accessing SRAM with the CPU during this process
// will cause jitter, so the below example
// updates the buffer after the DMA transfer finishes and
// restarts the transfer.
//
// It outputs a pattern of repeating 01010101 and
// 000000 alternating "frames".
//

#include "ch32fun.h"
#include <stdio.h>

volatile uint32_t count;

#define MBSAMPS 1024
uint8_t memory_buffer[MBSAMPS];

void DMA1_Channel2_IRQHandler( void ) __attribute__((interrupt)) __attribute__((section(".srodata")));

void DMA1_Channel2_IRQHandler( void )
{
	int i;
	static int frameno;
	volatile int intfr = DMA1->INTFR;
	do
	{
		DMA1->INTFCR = DMA1_IT_GL2;

		// Gets called at end of transfer
		if( intfr & DMA1_IT_TC2 )
		{
			uint32_t fv = (frameno&1)?0:0xaa55aa55;
			for( i = 0; i < MBSAMPS/4; i++ )
			{
				((uint32_t *)memory_buffer)[i] = fv;
			}
			frameno++;
		}

		intfr = DMA1->INTFR;
	} while( intfr );

	// Restart DMA
	DMA1_Channel2->CNTR = MBSAMPS;
	DMA1_Channel2->CFGR |= DMA_CFGR1_EN;
}

int main()
{
	SystemInit();

	// Reset all the peripherals we care about.
	RCC->APB2PRSTR = 0xffffffff;
	RCC->APB2PRSTR = 0;

	// Enable DMA
	RCC->AHBPCENR = RCC_AHBPeriph_SRAM | RCC_AHBPeriph_DMA1;

	// Enable GPIO
	RCC->APB2PCENR = RCC_APB2Periph_GPIOC;

	// GPIO C All output.
	// This is where our bitstream will be outputted to.
	GPIOC->CFGLR =
		(GPIO_Speed_30MHz | GPIO_CNF_OUT_PP)<<(4*0) |
		(GPIO_Speed_30MHz | GPIO_CNF_OUT_PP)<<(4*1) |
		(GPIO_Speed_30MHz | GPIO_CNF_OUT_PP)<<(4*2) |
		(GPIO_Speed_30MHz | GPIO_CNF_OUT_PP)<<(4*3) |
		(GPIO_Speed_30MHz | GPIO_CNF_OUT_PP)<<(4*4) |
		(GPIO_Speed_30MHz | GPIO_CNF_OUT_PP)<<(4*5) |
		(GPIO_Speed_30MHz | GPIO_CNF_OUT_PP)<<(4*6) |
		(GPIO_Speed_30MHz | GPIO_CNF_OUT_PP)<<(4*7);

	// The idea here is that this copies byte-at-a-time from the buffer
	// to GPIO
	DMA1_Channel2->CNTR = sizeof(memory_buffer) / sizeof(memory_buffer[0]);

	// Some limitations of mem2mem:
	//  * Unintuitively, transfer goes from PADDR to MADDR
	//  * Circular mode is not supported
	DMA1_Channel2->MADDR = (uint32_t)&GPIOC->OUTDR;
	DMA1_Channel2->PADDR = (uint32_t)memory_buffer;
	DMA1_Channel2->CFGR =
	    DMA_CFGR1_MEM2MEM |
		DMA_CFGR1_PL |                       // Very High priority.
		0 |                                  // 8-bit memory
		0 |                                  // 8-bit peripheral
		DMA_CFGR1_PINC |                     // Increase peripheral
		DMA_CFGR1_TCIE;                      // Interrupt on transfer complete

	NVIC_EnableIRQ( DMA1_Channel2_IRQn );
	DMA1_Channel2->CFGR |= DMA_CFGR1_EN;

	// Just debug stuff.
	Delay_Ms( 250 );
	printf( "Setup OK\n" );

	while(1)
	{
	}
}

