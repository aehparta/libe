
# default target is x86
TARGET = X86

# enable log and i2c
USE += I2C LOG

DEFINES_PIC8 += I2C_BITBANG_SCL=0 I2C_BITBANG_SDA=1
DEFINES_ESP32 += I2C_BITBANG_SCL=23 I2C_BITBANG_SDA=18

# init
include $(LIBE_PATH)/init.mk

# our own sources etc
BUILD_BINS = i2c_mcp3221
PROGRAM_BIN = i2c_mcp3221
i2c_mcp3221_SRC = main.c $(libe_SRC)

# build
include $(LIBE_PATH)/build.mk

# programmable targets
include $(LIBE_PATH)/flash/program.mk

-include local.mk
