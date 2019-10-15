
# default target is x86
TARGET = X86

# enable spi
USE += SPI LOG
USE_X86 += FTDI

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
