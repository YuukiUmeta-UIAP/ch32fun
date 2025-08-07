## Update Status Overview
|PERIPHERAL    |V003|V00x|V10x|V20x|V30x|X035(643\*)|L103|M030| 641|
|:-------------|:--:|:--:|:--:|:--:|:--:|:---------:|:--:|:--:|:--:|
|DPAL Header\* |2.0 | ×  |2.7 | ×  | ×  | √         | ×  | ×  |1.3 |
|ADC           |1.9 | ×  |2.1 | ×  | ×  |1.3        | ×  | ×  | √  |
|AWU           |N/A |N/A |N/A |N/A |N/A | √         |N/A |N/A |N/A |
|BKP           |N/A |N/A |2.1 | ×  | ×  |N/A        | ×  |N/A |N/A |
|CAN           |N/A |N/A |N/A | ×  | ×  |N/A        | ×  |N/A |N/A |
|CRC           |N/A |N/A |2.1 | ×  | ×  |N/A        | ×  |N/A |N/A |
|DAC           |N/A |N/A |N/A |N/A | ×  |N/A        |N/A |N/A |N/A |
|DBGMCU        |1.5 | ×  |2.1 | ×  | ×  | √         | ×  | ×  |1.2 |
|DMA           | √  | ×  | √  | ×  | ×  | √         | ×  | ×  | √  |
|DVP           |N/A |N/A |N/A |N/A | ×  |N/A        |N/A |N/A |N/A |
|ETH           |N/A |N/A |N/A |N/A | ×  |N/A        |N/A |N/A |N/A |
|EXIT          | √  | ×  |2.4 | ×  | ×  | √         | ×  | ×  | √  |
|FLASH         | √  | ×  |2.7 | ×  | ×  |1.4        | ×  | ×  |1.1 |
|FSMC          |N/A |N/A |N/A |N/A | ×  |N/A        |N/A |N/A |N/A |
|GPIO          |2.0 | ×  |2.7 | ×  | ×  |1.6        | ×  | ×  |1.2 |
|I2C           | √  | ×  | √  | ×  | ×  |1.7        | ×  | ×  |1.2 |
|IWDG          | √  | ×  | √  | ×  | ×  | √         | ×  |N/A |N/A |
|LPTIM         |N/A |N/A |N/A |N/A |N/A |N/A        | ×  |N/A |N/A |
|MISC          | √  | ×  |2.4 | ×  | ×  |1.6        | ×  |N/A |1.1 |
|OPA           | √  | ×  |N/A | ×  | ×  |1.3        | ×  | ×  |N/A |
|PWR           |1.9 | ×  |2.6 | ×  | ×  |1.7        | ×  | ×  | √  |
|RCC           |1.8 | ×  |2.7 | ×  | ×  | √         | ×  | ×  |1.1 |
|RNG           |N/A |N/A |N/A |N/A | ×  |N/A        |N/A |N/A |N/A |
|RTC           |N/A |N/A | √  | ×  | ×  |N/A        | ×  |N/A |N/A |
|SPI           |1.9 | ×  |2.7 | ×  | ×  |1.7        | ×  | ×  |N/A |
|TIM           |1.6 | ×  | √  | ×  | ×  | √         | ×  | ×  | √  |
|USART         | √  | ×  |2.4 | ×  | ×  | √         | ×  | ×  | √  |
|USB           |N/A |N/A | √  | ×  | ×  |1.8        | ×  | ×  |N/A |
|USB_HOST      |N/A |N/A | √  |N/A |N/A |N/A        |N/A |N/A |N/A |
|USBPD         |N/A |N/A |N/A |N/A |N/A |1.4        | ×  | ×  |1.2 |
|WWWDG         | √  | ×  | √  | ×  | ×  | √         | ×  | ×  | √  |
|**chxxxhw.h** | √  | ×  | √  | √  | √  | √         | ×  | ×  | √  |
|**minichlink**| √  | ×  | +  | √  | √  | √  ( √   )| √  | ×  | √  |

* n.m:  Last commit message of the header file in ch32xxx/EVT/EXAM/SRC/Peripheral/inc
* √:    Merged in , version unspecified
* ×:    Not merged / Unchecked
* +:    Work in progress
* N/A:  No header file with this suffix in EVT, does not mean that the feature is not supported

\* X035(643): They are the same except electrical characteristics, LEDPWM, remapping, and ADC channel numbers.\
\* DPAL Header: Device Peripheral Access Layer Header File, normally named as ch32xxx.h
