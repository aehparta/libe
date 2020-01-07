
TARGET = AVR
# MCU_PIC8 = pic16f18345
# F_CPU_PIC8 = 16000000
MCU_AVR = atmega328p
F_CPU_AVR = 8000000

USE = I2C GPIO LOG
# DEFINES_PIC8 = SPI_BITBANG_SCLK=GPIOA0 SPI_BITBANG_MOSI=GPIOA1 SPI_BITBANG_MISO=GPIOA2

# init
include $(LIBE_PATH)/init.mk

# our own sources etc
BUILD_BINS = testing
PROGRAM_BIN = testing
testing_SRC = main.c $(libe_SRC)

DEFINES_PIC8 += SPI_SS=0

# build
include $(LIBE_PATH)/build.mk

# programmable targets
include $(LIBE_PATH)/flash/program.mk

-include local.mk
