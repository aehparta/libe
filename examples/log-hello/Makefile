
# default board is x86
BOARD ?= x86

# default libe path, when in examples
LIBE_PATH ?= ../..

# init
include $(LIBE_PATH)/init.mk

# our own sources etc
BUILD_BINS = log_hello
PROGRAM_BIN = log_hello
log_hello_SRC = main.c $(libe_SRC)

# build
include $(LIBE_PATH)/build.mk

# programmable targets
include $(LIBE_PATH)/flash/program.mk

-include local.mk
