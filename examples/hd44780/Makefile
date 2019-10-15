
# check that LIBE_PATH is set
ifeq ($(LIBE_PATH),)
    $(error LIBE_PATH not set)
endif

# default target is x86
TARGET = X86

# enable ftdi when x86
USE_X86 += FTDI

# init make system
include $(LIBE_PATH)/init.mk

# our own sources etc
BUILD_BINS = hd44780
PROGRAM_BIN = hd44780
hd44780_SRC = main.c $(libe_SRC)

# build
include $(LIBE_PATH)/build.mk

# programmable targets
include $(LIBE_PATH)/flash/program.mk
