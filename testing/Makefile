
# check that LIBE_PATH is set
ifeq ($(LIBE_PATH),)
    $(error LIBE_PATH not set)
endif

TARGET = X86

MCU_AVR = atmega128
F_CPU_ATMEGA328 = 1000

USE = I2C
USE_X86 = I2C_BITBANG FTDI GPIO
USE_ATMEGA8 = LOG

CFLAGS_ATMEGA8 = -Dxxx=yyy
LDFLAGS_ATMEGA8 = -lm

# enable gpio
# libe_USE += GPIO

# enable spi
# libe_USE += SPI LOG
# ifdef TARGET_X86
#     libe_USE += FTDI
# endif

# init
include $(LIBE_PATH)/init.mk

# our own sources etc
BUILD_BINS = testing
PROGRAM_BIN = testing
testing_SRC = main.c $(libe_SRC)

# other
ifeq ($(TARGET), avr)
	CFLAGS += -DI2C_BITBANG_SCL=28 -DI2C_BITBANG_SDA=27
endif
ifeq ($(TARGET), pic8)
	CFLAGS += -DI2C_BITBANG_SCL=0 -DI2C_BITBANG_SDA=1
endif

# build
include $(LIBE_PATH)/build.mk

# programmable targets
include $(LIBE_PATH)/flash/program.mk

-include local.mk
