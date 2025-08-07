// ADC test on ch570...
// 
// Methodology:
//  Connect a 0.1uF capcaitor between PA7 and GND
//  Connect a 5.1k resistor between PA7 and the voltage you want to measure.
//  WARNING: this will inject a little bit of noise on your signal.
//
//  Increasing resistance will improve reading quality, and reduce ripple, but take longer.
//  Increasing capacitance will reduce ripple, take more power and take longer.
//
// NOTE: The measurement is EXTREMELY sensitive to the capacitance and you should calibrate.  Capacitance changes over temperature.
// NOTE: You can only measure voltages from just over the reference or higher. Practical limits are in the range of 20ish volts.

#include "ch32fun.h"
#include <stdio.h>

#define CAPACITANCE 0.000000088
#define RESISTANCE  5100.0
#define VREF 0.2
#define FIXEDPOINT_SCALE 1000

// This can be PA7 or PA4, PA4 is less tested than PA7.  Recommend PA7 at this time.

#define ADCPIN PA7

// VERF = 100mV and PA7 - CMP_VERF
// Timer Capture comes from Input (not that it matters) + Enable.
//
// Rising edge generates interrupt. (Disable interrupt for now)
// After a lot of testing 100mV seems best.

#if ADCPIN == PA7
#define CMP_CONFIG ( 0x3 << 4 /*200mV*/ ) | RB_CMP_EN | RB_CMP_CAP | (0b11<<2 /*Which input*/) | (0b00<<10 /*High level trigger*/)
#elif ADCPIN == PA4
#define CMP_CONFIG ( 0x3 << 4 /*200mV*/ ) | RB_CMP_EN | RB_CMP_CAP | (0b01<<2 /*Which input*/) | (0b00<<10 /*High level trigger*/)
#else
#error ADCPIN must be PA7 or PA4
#endif

volatile uint32_t lastfifo = 0;

void TMR1_IRQHandler(void) __attribute__((interrupt))  __attribute__((section(".srodata")));
void TMR1_IRQHandler(void)
{
	R8_TMR_INT_FLAG = 2;
	lastfifo = R32_TMR_FIFO;
	funPinMode( ADCPIN, GPIO_ModeOut_PP_20mA );
}


// The timing on the setup has to be tight.
void EventRelease(void) __attribute__((section(".srodata"))) __attribute__((noinline));
void EventRelease(void)
{
	R8_TMR_CTRL_MOD = 0b00000010; // Reset Timer
	R8_TMR_CTRL_MOD = 0b11000101; // Capture mode rising edge
	funPinMode( ADCPIN, GPIO_ModeIN_Floating );
}

void SetupADC(void)
{
	R32_CMP_CTRL = CMP_CONFIG;

	R8_TMR_CTRL_MOD = 0b00000010; // All clear
	R32_TMR_CNT_END = 0x03FFFFFF; // Maximum possible counter size.
	R8_TMR_CTRL_MOD = 0b11000101; // Capture mode rising edge
	R8_TMR_INTER_EN = 0b10; // Capture event.

	NVIC_EnableIRQ(TMR1_IRQn);
	__enable_irq();

	funPinMode( ADCPIN, GPIO_ModeOut_PP_20mA );
}

int main()
{
	SystemInit();

	funGpioInitAll(); // no-op on ch5xx

	SetupADC();

	while(1)
	{
		Delay_Ms(2);
		EventRelease();
		Delay_Ms(10);

		// tau = R * C
		// Vcomp = Vmeas(1 - 2.718 ^ (-t/tau))
		// Vmeas = Vcomp / (1 - 2.718 ^ (-t/tau))
		//
		// For example:
		//t = ticks / 60000000
		//tau = 5.1k * 0.1uF
		//Vcomp = 0.1V
		//Vmeas = Vcomp / (1 - e ^ (-t/tau))
		//
		//Vcomp = 0.1
		//t = 533 / 60000000
		//tau = 0.0051
		//-t/tau = -0.00174183
		//e^that = 0.998259686

		//A complete computation is = 0.1/(1-EXP(-ticks/60000000/(5100*0.0000001)))
		// Which oddly works out to almost exactly: = 3060/ticks+0.05
		// 3060=60000000*(5100*0.0000001)*0.1
		//
		// 3060000/ticks + 50 in millivolts.
		#define COEFFICIENT (const uint32_t)(FUNCONF_SYSTEM_CORE_CLOCK*(RESISTANCE*CAPACITANCE)*VREF*FIXEDPOINT_SCALE+0.5)
		int r = lastfifo - 2; // 2 cycles back.
		int vtot = COEFFICIENT/r + ((const uint32_t)(VREF*FIXEDPOINT_SCALE));
		printf( "%d %d\n", r, vtot );
	}
}


