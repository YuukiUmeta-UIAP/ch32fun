#include "ch32fun.h"
#include <stdio.h>

int main()
{
	SystemInit();

	// Enable timers
	RCC->APB2PCENR |= RCC_APB2Periph_TIM1;
	RCC->APB1PCENR |= RCC_APB1Periph_TIM2;

	// Reset TIM1 to init all regs
	RCC->APB2PRSTR |= RCC_APB2Periph_TIM1;
	RCC->APB2PRSTR &= ~RCC_APB2Periph_TIM1;
	
	// Reset TIM2 to init all regs
	RCC->APB1PRSTR |= RCC_APB1Periph_TIM2;
	RCC->APB1PRSTR &= ~RCC_APB1Periph_TIM2;

	// Prescaler 1 tick = 1 ms
	TIM1->PSC = FUNCONF_SYSTEM_CORE_CLOCK / 1000 - 1;
	
	// Reload period 1000 ticks = 1s
	TIM1->ATRLR = 1000 - 1;

	// Reload immediately
	TIM1->SWEVGR |= TIM_UG;

	// Set TIM1 as a master with TRGO configured to trigger on an update event
	TIM1->CTLR2 |= TIM_TRGOSource_Update;
	
	// Select TRIGO of TIM1 as CLK source for TIM2
	TIM2->SMCFGR |= TIM_TS_ITR0
				 |  TIM_SlaveMode_External1
				 |  TIM_SlaveMode_Trigger;
	
	// Prescaler so each tick of this timer is equal
	// to TIM1->PSC * TIM1->ATRLR * TIM2->PSC "SystemTicks"
	//TIM2->PSC = 10 - 1;
	
	// TIM2 refresh period
	TIM2->ATRLR = 0xFFFF;

	// Enable Timers
	TIM1->CTLR1 |= TIM_CEN;
	TIM2->CTLR1 |= TIM_CEN;

	while(1)
	{
		uint16_t ms = TIM1->CNT;
		uint16_t s = TIM2->CNT;
		printf("%d.%03d s\r\n", s, ms);
		Delay_Ms(500);
	}
}

