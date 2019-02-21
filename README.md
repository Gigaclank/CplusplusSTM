[![GitHub issues](https://img.shields.io/github/issues/Gigaclank/STM-Drivers.svg?style=popout)](https://github.com/Gigaclank/STM-Drivers/issues)
[![GitHub forks](https://img.shields.io/github/forks/Gigaclank/STM-Drivers.svg?style=popout)](https://github.com/Gigaclank/STM-Drivers/network)
[![GitHub stars](https://img.shields.io/github/stars/Gigaclank/STM-Drivers.svg?style=popout)](https://github.com/Gigaclank/STM-Drivers/stargazers)
[![GitHub license](https://img.shields.io/github/license/Gigaclank/STM-Drivers.svg?style=popout)](https://github.com/Gigaclank/STM-Drivers/blob/master/LICENSE.txt)
[![Github all releases](https://img.shields.io/github/downloads/Gigaclank/STM-Drivers/total.svg)](https://github.com/Gigaclank/STM-Drivers)
---
# README #
Welcome to STM32-Drivers - a C++ variation on the standard peripheral drivers. 

All drivers have been built using IAR.
---
## Implemented ##
* GPIO
* Timers
* RTC
* IWDG
* UART
* BKP
* SPI
* CRC
* ADC

---
### TODO ###
* Implement DAC Drivers
* Implement I2C Drivers
* Implement Window Watchdog Drivers
* Implement EEPROM Drivers
* Implement PWR Drivers
* Implement SDIO Drivers
* Implement CEC Drivers
* Implement CAN Drivers
* Implement DMA Drivers

#### Examples ####
* adc_main.cpp    - Starts up an ADC and prints out the value of the adc.
* bkp_main.cpp    - Starts up the bkp peripheral and writes a value if not aready written.
* crc_main.cpp    - Starts up the crc peripheral and calculates the crc32 of a buffer
* gpio_main.cpp   - Implements GPIO with interrupt and toggles led pins.
* iwdg_main.cpp   - Starts up the iwdg for 2 seconds timeout and reloads the iwdg in the main loop.
* rtc_main.cpp    - Starts up the rtc and attaches an interrupt handler to the rtc tick.
* spi_main.cpp    - Starts up the spi peripheral and writes/reads.
* timer_main.cpp  - Cycles through all 14 timers and delays for 1 count of the timer count
* timer_main_pwm.cpp  - Starts up timers and outputs different pwm values on the the pwm channel pins.

---
<p align="center" z-index = "-1">
  <img src="https://avatars2.githubusercontent.com/u/12459794?s=200&v=4"/>
</p>