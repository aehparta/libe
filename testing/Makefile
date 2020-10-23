
TARGET = PIC8
MCU_PIC8 = 16F18345
F_CPU_PIC8 = 16000000
DEFINES_PIC8 += LOG_BAUD=38400 PIC8_LOG_TX_PPS=RA5PPS

USE = LOG LOG_COLORS GPIO

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
