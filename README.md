# libe
***Hobby project, so work in progress, probably forever***

Generic library targeted mostly for microcontrollers.

[Documentation at readthedocs.](https://libe.readthedocs.io/en/latest/)

## Status

Progress happens whenever I have need/time for this library.
This, in reality, means rarely.

## Features

### GPIO

Generic interface for GPIO which requires only *pin number* instead of *port&pin*-combo used in many smaller devices like *AVR* and *PIC*.
This is achieved by mapping *PORTB* in AVR to pin numbers *8-15*, *PORTC* to *16-23* and so on.
Same for PIC, MSP430 and others that do this kind of thing instead of having continuous
GPIO pin numbers.

GPIO implemented this way is more and less complex thing.
On most platforms, like AVR, GPIO functions are inline and will often optimize as well
as directly written native code.

#### Drivers

* HD44780: Alphanumeric LCD in 4 bit mode

### Timing and delay

Generic *"time elapsed since bootup"* and *sleep/delay* functions.

### SPI

Generic SPI interface. Supports multiple SPI masters and multiple devices under each master.

#### Drivers

* NRF24L01+: RF transceiver IC

### I2C

Generic I2C interface either in bitbang mode or using native hardware.

#### Drivers

* MCP3221: Microchip 12-bit ADC
* HDC1080: Ti temperature and humidity sensor

### Log and debug

Logging and debugging most often through serial port.
Other channels maybe used. Example *ESP32* port uses logging provided by its own development framework.

### NVM (eeprom or such)

Non-volatile storage interface. This could be eeprom, a flash partition or something else.

### PWM

Pulse-width modulation. So far mostly planned and only works in *ESP32*.

### Software AES

Software AES support thanks to [AndrewCarterUK](https://twitter.com/AndrewCarterUK) from [SmarterDM](https://github.com/SmarterDM/micro-aes).

Tested on pretty much all platforms supported by ***libe*** using ***nrf24l01+*** network.


