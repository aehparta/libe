
# default target is x86
TARGET = X86

# enable spi
USE += SPI LOG GPIO DRIVER_NRF24L01P
USE_X86 += FTDI

DEFINES_PIC8 += SPI_SS=GPIOA4 SPI_MISO=GPIOA2 SPI_MOSI=GPIOA1 SPI_SCLK=GPIOA0
DEFINES_PIC8 += NRF24L01P_CE=GPIOA5
DEFINES_16LF18426 += PIC8_LOG_TXIF=TX1IF

# init make system
include $(LIBE_PATH)/init.mk

# our own sources etc
BUILD_BINS = nrf24l01_ping
PROGRAM_BIN = nrf24l01_ping
nrf24l01_ping_SRC = main.c $(libe_SRC)

# build
include $(LIBE_PATH)/build.mk

# programmable targets
include $(LIBE_PATH)/flash/program.mk

-include local.mk
