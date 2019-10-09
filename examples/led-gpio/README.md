# LED GPIO blink example application

## Tested targets

* avr on atmega328
    * PB4: First LED
    * PB5: Second LED
    * PD1: Serial port TX, 9600 8N1

* esp32 with ESP32 DEVKIT v1
    * 12: First LED
    * 13: Second LED

* x86 using FTDI FT4232H
    * DDBUS3: First LED
    * DDBUS4: Second LED

* pic8 on pic16f18345 xpress board
    * RB0: First LED
    * RB1: Second LED
    * RC6: Serial port TX, 9600 8N1

### Emulator tests

* avr on atmega8
    * PB0: First LED
    * PB1: Second LED
    * PD1: Serial port TX, 9600 8N1

* avr on attiny85 without logging
    * PB0: First LED
    * PB1: Second LED

#### pic8

* pic16f84a without logging
    * RB0: First LED
    * RB1: Second LED
* pic16f87
    * RB0: First LED
    * RB1: Second LED
    * RB5: Serial port TX, 9600 8N1
* pic16f882
    * RB0: First LED, **NOT working**, some odd bug, fix!
    * RB1: Second LED
    * RC6: Serial port TX, 9600 8N1
* pic16f946
    * RB0: First LED
    * RB1: Second LED
    * RC6: Serial port TX, 9600 8N1
* pic16f1934
    * RB0: First LED
    * RB1: Second LED
    * RC6: Serial port TX, 9600 8N1
* pic18f13k22
    * RC0: First LED
    * RC1: Second LED
    * RB7: Serial port TX, 9600 8N1
* pic18f1330
    * RB0: First LED
    * RB1: Second LED
    * RA2: Serial port TX, 9600 8N1
* pic18f6622
    * RB0: First LED
    * RB1: Second LED
    * RC6: Serial port TX, 9600 8N1
