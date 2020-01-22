
# default target is x86
TARGET = X86

# enable log and i2c
USE += I2C LOG DISPLAY SDL2

# init
include $(LIBE_PATH)/init.mk

# our own sources etc
BUILD_BINS = display
PROGRAM_BIN = display
display_SRC = main.c $(libe_SRC)

# build
include $(LIBE_PATH)/build.mk

# programmable targets
include $(LIBE_PATH)/flash/program.mk

-include local.mk
