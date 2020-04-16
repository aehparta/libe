
# default target is x86
TARGET = X86

# enable log and i2c
USE += I2C LOG GPIO

# pic8
MCU_PIC8 = 16F18345
F_CPU_PIC8 = 16000000
# DEFINES_PIC8 += I2C_BITBANG_SCL=0 I2C_BITBANG_SDA=1
DEFINES_PIC8 += LOG_BAUD=115200 PIC8_LOG_TX_PPS=RA5PPS
DEFINES_PIC8 += I2C_HARDCODED_SCL=GPIOC5 I2C_HARDCODED_SDA=GPIOC4

# x86
DEFINES_X86 += I2C_BITBANG_SCL=0 I2C_BITBANG_SDA=1 FTDI_VID=0 FTDI_PID=0 FTDI_DESC=NULL FTDI_SERIAL=NULL

# base init of makesystem
include $(LIBE_PATH)/init.mk

# our own sources etc
BUILD_BINS = i2c_scan
PROGRAM_BIN = i2c_scan
i2c_scan_SRC = main.c $(libe_SRC)

# build
include $(LIBE_PATH)/build.mk

# programmable targets
include $(LIBE_PATH)/flash/program.mk

-include local.mk
