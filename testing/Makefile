
TARGET = PIC8
MCU_PIC8 = 16LF1939
F_CPU_PIC8 = 1000000

USE = GPIO RC5 AES

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
