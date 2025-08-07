#include "ch32fun.h"
#include <stdio.h>

//#define RTC_CLOCK_SOURCE_LSI
#define RTC_CLOCK_SOURCE_LSE
#define RTC_CLOCK_FREQ 32768 // in Hz

void RTC_IRQHandler(void) __attribute__((interrupt));
void RTC_IRQHandler(void)
{
	// Clear flag
	RTC->CTLRL &=~ RTC_CTLRL_ALRF;

	printf("Hello, Sailor.\r\n");
}

int main()
{
	SystemInit();

	// Enable access to the RTC and backup registers
	RCC->APB1PCENR |= RCC_PWREN | RCC_BKPEN;
	PWR->CTLR |= PWR_CTLR_DBP;

	// this is needed to reset RTC on external manual reset (nRST pin) and first time flashing
	if ( !(RCC->RSTSCKR & RCC_PORRSTF) && (BKP->DATAR1 != 0xDEAD) || (RCC->RSTSCKR & RCC_PINRSTF) )
	{
		// Clear nRST pin reset flag
		RCC->RSTSCKR |= RCC_RMVF;

		// Reset backup domain. This operation must be performed
		// if you want to change RTC clock source
		RCC->BDCTLR |= RCC_BDRST;
		RCC->BDCTLR &=~RCC_BDRST;
		
		// store flag in backup domain register
		BKP->DATAR1 = 0xDEAD;

		#ifdef RTC_CLOCK_SOURCE_LSI
			// Enable LSI
			RCC->RSTSCKR |= RCC_LSION;
			while ( !(RCC->RSTSCKR & RCC_LSIRDY) );

			// Set clock source for RTC
			RCC->BDCTLR &=~ RCC_RTCSEL;
			RCC->BDCTLR |= RCC_RTCSEL_LSI;
		#endif

		#ifdef RTC_CLOCK_SOURCE_LSE
			// Enable LSE
			RCC->BDCTLR |= RCC_LSEON;
			uint16_t timeout = 0;
			while ( !(RCC->BDCTLR & RCC_LSERDY) ) {
				Delay_Ms(1);
				timeout++;
				if (timeout > 1000) {
					printf("Could not start LSE.\r\n");
					break;
				}
			}
			// Set clock source for RTC
			RCC->BDCTLR &=~ RCC_RTCSEL;
			RCC->BDCTLR |= RCC_RTCSEL_LSE;
		#endif

		// Enable RTC
		RCC->BDCTLR |= RCC_RTCEN;

		// Enter configuration mode
		while ( !(RTC->CTLRL & RTC_FLAG_RTOFF) );
		RTC->CTLRL |= RTC_CTLRL_CNF;

		// Set prescaler so 1 tick = 1 s
		RTC->PSCRH = 0;
		RTC->PSCRL = RTC_CLOCK_FREQ - 1;
		
		// Set counting start value
		RTC->CNTH = 0;
		RTC->CNTL = 0;

		// Set alarm
		RTC->ALRMH = 0;
		RTC->ALRML = 10;

		// Enable alarm interrupt
		while ( !(RTC->CTLRL & RTC_FLAG_RTOFF) );
		RTC->CTLRH |= RTC_CTLRH_ALRIE;
		NVIC_EnableIRQ(RTC_IRQn);

		// Exit configuration mode and actually start RTC
		while ( !(RTC->CTLRL & RTC_FLAG_RTOFF) );
		RTC->CTLRL &=~ RTC_CTLRL_CNF;

	}
	// IDK if it actually needed, but in RM:
	// "after the PB1 is reset or PB1 clock is stopped, the bit should be reset firstly"
	RTC->CTLRL &=~ RTC_CTLRL_RSF;
	
	// Wait for sync so when performing read we get valid counter value
	while ( !(RTC->CTLRL & RTC_CTLRL_RSF) );
	
	while (1)
	{
		uint16_t high1, high2, low;
		uint32_t s;

		high1 = RTC->CNTH;
		low = RTC->CNTL;
		high2 = RTC->CNTH;
		
		// handle rollover
		if (high1 != high2) {
			s = (high2 << 16) | RTC->CNTL;
		} else {
			s = (high1 << 16) | low;
		}

		printf("%ld s\r\n", s);

		Delay_Ms(1000);
	}
	
}