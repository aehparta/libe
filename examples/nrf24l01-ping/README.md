# NRF24L01+ ping example application

This example need precise timing, so it was not tested on smaller/slower platforms.

## Tested targets

* x86 with FTDI FT232H in MPSSE-SPI mode
    * ADBUS0: SCLK
    * ADBUS1: MOSI
    * ADBUS2: MISO
    * ADBUS3: SS
    * ADBUS4: CE (NRF radio enable)
* Raspberry Pi 3 Model B
    * SPI: /dev/spidev0.0
    * GPIO25: CE (NRF radio enable)
* ESP32
    * GPIO14: SCLK
    * GPIO13: MOSI
    * GPIO12: MISO
    * GPIO26: SS
    * GPIO27: CE (NRF radio enable)
