
# default target is x86
TARGET = X86

# enable log and display
USE += LOG DISPLAY
# USE_X86 += SDL2
USE_AVR += I2C

USE_ESP32 += I2C

# init
include $(LIBE_PATH)/init.mk

# our own sources etc
BUILD_BINS = display
PROGRAM_BIN = display
display_SRC = main.c $(libe_SRC)
display_SRC += $(LIBE_PATH)/libe/fonts/fontfixed6x8.c $(LIBE_PATH)/libe/fonts/fontcm30.c

# build
include $(LIBE_PATH)/build.mk

# programmable targets
include $(LIBE_PATH)/flash/program.mk

-include local.mk
