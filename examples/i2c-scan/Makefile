
# default target is x86
TARGET = X86

# enable log and i2c
USE += I2C LOG

DEFINES_PIC8 += I2C_BITBANG_SCL=0 I2C_BITBANG_SDA=1

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
