# LED GPIO blink example application

## Tested targets

* PIC8 on pic16f18345 xpress board
    * RB4: First LED
    * RB5: Second LED
    * RC6: Serial port TX, 9600 8N1

* ESP32 with ESP32 DEVKIT v1
    * 12: First LED
    * 13: Second LED

* X86 using FTDI FT4232H
    * DDBUS3: First LED
    * DDBUS4: Second LED

### Emulator tests

#### AVR

* atmega8
    * PB0: First LED
    * PB1: Second LED
    * PD1: Serial port TX, 9600 8N1
