
TARGET = X86
USE = LOG FTDI SPI GPIO DRIVER_MCP356X

# init
include $(LIBE_PATH)/init.mk

# our own sources etc
BUILD_BINS = testing
PROGRAM_BIN = testing
testing_SRC = main.c $(libe_SRC)

# build
include $(LIBE_PATH)/build.mk

# programmable targets
include $(LIBE_PATH)/flash/program.mk

-include local.mk
