
# default target is x86
TARGET = X86

# enable log and i2c
USE += I2C LOG DRIVER_HDC1080 DRIVER_SHT21 DRIVER_SHT31

# set default clock speed to 1 MHz for this example
F_CPU=1000000

# avr defaults
MCU_AVR=atmega8

# pic defaults
MCU_PIC8=16f87
USE_PIC8 += I2C_BITBANG
DEFINES_PIC8 += I2C_BITBANG_SCL=0 I2C_BITBANG_SDA=1

# x86
DEFINES_X86 += I2C_BITBANG_SCL=0 I2C_BITBANG_SDA=1 FTDI_VID=0 FTDI_PID=0 FTDI_DESC=NULL FTDI_SERIAL=NULL

# base init of makesystem
include $(LIBE_PATH)/init.mk

# our own sources etc
BUILD_BINS = i2c_t_and_h
PROGRAM_BIN = i2c_t_and_h
i2c_t_and_h_SRC = main.c $(libe_SRC)

# on avr use extensive printf to print floats
ifeq ($(TARGET),AVR)
    LDFLAGS += -Wl,-u,vfprintf -lprintf_flt
endif

# build
include $(LIBE_PATH)/build.mk

# programmable targets
include $(LIBE_PATH)/flash/program.mk

-include local.mk
