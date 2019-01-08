# libe

Generic library targeted mostly for microcontrollers.

## Features

### GPIO

Generic interface for GPIO which requires only *pin number* instead of *port&pin*-combo used in many smaller devices like *AVR* and *PIC*.
This is achieved by mapping *PORTB* in AVR to pin numbers *8-15*, *PORTC* to *16-23* and so on.
Same for PIC, MSP430 and others that do this kind of thing instead of having continuous
GPIO pin numbers.

### Timing and delay

Generic *"time elapsed since bootup"* and *sleep/delay* functions.

### SPI

Generic SPI interfacing. Supports multiple SPI masters and multiple devices under each master.

#### Drivers

* nrf24l01+

### I2C

Generic I2C interfacing either in bitbang mode or using native hardware.

#### Drivers

***Coming but not yet available (already written though, need a bit of porting to new libe)***

* CAP1293: Microchip touch sensor
* FAN5702: Fairchild LED driver
* HDC1080: Ti temperature and humidity sensor
* SHT21: Sensirion temperature and humidity sensor
* MCP39F521: Microchip power monitor

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


