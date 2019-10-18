
TARGET = X86
USE += I2C ERROR

# init build
include $(LIBE_PATH)/init.mk

# our own sources etc
BUILD_BINS = libe_i2c
PROGRAM_BIN = libe_i2c
libe_i2c_SRC = main.c $(libe_SRC)

# compile flags
CFLAGS += $(libe_CFLAGS) -DCOMPILE_TOOL_I2C
LDFLAGS += $(libe_LDFLAGS)

# build
include $(LIBE_PATH)/build.mk

# programmable targets
include $(LIBE_PATH)/flash/program.mk

-include local.mk
