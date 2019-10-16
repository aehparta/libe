
# default target is x86
TARGET = X86

# enable log
USE += LOG

# set default clock speed to 1 MHz for this example
F_CPU=1000000

# target specific settings
MCU_AVR=atmega8
MCU_PIC8=16f87

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
