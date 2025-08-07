#include "ch32fun.h"

#if defined(CH57x) && (MCU_PACKAGE == 0 || MCU_PACKAGE == 2) // ch570/2
#define PIN_LED    PA9
#define PIN_BUTTON PA1
#define BUTTON_PRESSED funDigitalRead( PIN_BUTTON )
#else
#define PIN_LED    PA8
#define PIN_BUTTON PB22
#define BUTTON_PRESSED !funDigitalRead( PIN_BUTTON )
#endif

void blink(int n) {
	for(int i = n-1; i >= 0; i--) {
		funDigitalWrite( PIN_LED, FUN_LOW ); // Turn on LED
		Delay_Ms(33);
		funDigitalWrite( PIN_LED, FUN_HIGH ); // Turn off LED
		if(i) Delay_Ms(33);
	}
}

int main()
{
	SystemInit();

	funGpioInitAll(); // no-op on ch5xx

	funPinMode( PIN_LED,    GPIO_CFGLR_OUT_10Mhz_PP ); // Set PIN_LED to output
	funPinMode( PIN_BUTTON, GPIO_CFGLR_IN_PUPD ); // Set PIN_BUTTON to input
	blink(1);

	while(1)
	{
		if( BUTTON_PRESSED ) {
			blink(5);
			jump_isprom();
		}
	}
}
