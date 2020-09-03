
# default target is x86
TARGET = X86

# enable spi
USE += SPI LOG GPIO DRIVER_NRF24L01P DRIVER_NRF24L01P_BLE
USE_X86 += FTDI

MCU_PIC8 = pic16f18345
F_CPU_PIC8 = 16000000
USE_PIC8 += SPI_BITBANG
DEFINES_PIC8 += SPI_BITBANG_SCLK=GPIOA0 SPI_BITBANG_MOSI=GPIOA1 SPI_BITBANG_MISO=GPIOA2

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
