
# default target is x86
TARGET = X86

# enable log and i2c
USE = I2C LOG LOG_COLORS DRIVER_I2C_AK1595
DEFINES_X86 = I2C_CONTEXT=NULL I2C_FREQUENCY=0 I2C_SCL=0 I2C_SDA=0

# base init of makesystem
include $(LIBE_PATH)/init.mk

# our own sources etc
BUILD_BINS = ak1595
PROGRAM_BIN = ak1595
ak1595_SRC = main.c $(libe_SRC)

# build
include $(LIBE_PATH)/build.mk

# programmable targets
include $(LIBE_PATH)/flash/program.mk

-include local.mk
