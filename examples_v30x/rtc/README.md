# RTC
This example demonstrates how to configure the RTC and set an alarm interrupt.

## Hardware
### Supported devices
Should apply to the entire range of v30x, v20x, v10x chips.
For parts without a VBAT pin, it can still be used for "volatile" time tracking.

### Battery connection
Connect the battery through a low-dropout diode to the VBAT pin to prevent
current injection into the battery when VBAT is less than VDD - 0.6V. 
If VBAT pin is left floating, RTC can still be used for "volatile" time tracking.