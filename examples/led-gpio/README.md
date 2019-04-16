# LED GPIO blink example application

## Tested targets

* avr on atmega328
    * PB4: First LED
    * PB5: Second LED
    * PD0/PD1: Serial port logging, 38400 8N1

* avr on atmega8 (in emulator)
    * PB4: First LED
    * PB5: Second LED
    * PD0/PD1: Serial port logging, 38400 8N1

* avr on attiny85 without logging (in emulator)
    * PB4: First LED
    * PB5: Second LED, with reset disabled

* pic8 on pic18f2410 (in emulator)
    * RB4: First LED
    * RB5: Second LED
    * RC6/RC7: Serial port logging, 38400 8N1

* pic8 on pic18f26k20 (in emulator)
    * RB4: First LED
    * RB5: Second LED
    * RC6/RC7: Serial port logging, 38400 8N1

* pic8 on pic16f87 without logging (in emulator)
    * RB4: First LED
    * RB5: Second LED

* esp32 with ESP32 DEVKIT v1
    * 12: First LED
    * 13: Second LED
