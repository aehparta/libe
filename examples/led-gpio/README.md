# LED GPIO blink example application

## Tested targets

* avr on atmega328
    * PB4: First LED
    * PB5: Second LED
    * PD0/PD1: Serial port logging, 38400 8N1

* esp32 with ESP32 DEVKIT v1
    * 12: First LED
    * 13: Second LED

* x86 using FTDI FT4232H
    * DDBUS3: First LED
    * DDBUS4: Second LED

* pic8 on pic16f18345 xpress board
    * RB4: First LED
    * RB5: Second LED
    * RC6=TX: Serial port logging, 9600 8N1

### Emulator tests

* avr on atmega8
    * PB4: First LED
    * PB5: Second LED
    * PD0/PD1: Serial port logging, 38400 8N1

* avr on attiny85 without logging
    * PB4: First LED
    * PB5: Second LED, with reset disabled

* pic8 on pic18f2410
    * RB4: First LED
    * RB5: Second LED
    * RC6/RC7: Serial port logging, 38400 8N1

* pic8 on pic18f26k20
    * RB4: First LED
    * RB5: Second LED
    * RC6/RC7: Serial port logging, 38400 8N1

* pic8 on pic16f87 without logging
    * RB4: First LED
    * RB5: Second LED
