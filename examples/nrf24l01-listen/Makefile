
# default target is x86
TARGET = X86

# enable spi
USE += SPI LOG GPIO NRF24L01P
USE_X86 += FTDI

DEFINES_PIC8 = SPI_BITBANG_SCLK=GPIOA0 SPI_BITBANG_MOSI=GPIOA1 SPI_BITBANG_MISO=GPIOA2

# init make system
include $(LIBE_PATH)/init.mk

# our own sources etc
BUILD_BINS = nrf24l01_listen
PROGRAM_BIN = nrf24l01_listen
nrf24l01_listen_SRC = main.c $(libe_SRC)

# build
include $(LIBE_PATH)/build.mk

# programmable targets
include $(LIBE_PATH)/flash/program.mk

-include local.mk
