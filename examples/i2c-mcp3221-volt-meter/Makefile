
# check that LIBE_PATH is set
ifeq ($(LIBE_PATH),)
    $(error LIBE_PATH not set)
endif

# enable log and i2c
libe_USE += I2C LOG

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
