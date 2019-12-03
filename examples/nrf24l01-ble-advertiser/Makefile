
# default target is x86
TARGET = X86

# enable spi
USE += SPI LOG GPIO
USE_X86 += FTDI

# init make system
include $(LIBE_PATH)/init.mk

# our own sources etc
BUILD_BINS = nrf24l01_ble_adv
PROGRAM_BIN = nrf24l01_ble_adv
nrf24l01_ble_adv_SRC = main.c $(libe_SRC)

# build
include $(LIBE_PATH)/build.mk

# programmable targets
include $(LIBE_PATH)/flash/program.mk

-include local.mk
