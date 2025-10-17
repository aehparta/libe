
LIBE_PATH ?= ../..

# default target is x86
TARGET = X86

# enable log and display
USE += LOG DISPLAY
# USE_X86 += FTDI SPI GPIO DRIVER_PCD8544
USE_X86 += FTDI SPI GPIO DRIVER_SSD1681
# USE_X86 += I2C DRIVER_SSD1306
USE_AVR += I2C DRIVER_SSD1306
USE_ESP32 += I2C DRIVER_SSD1306

OPTIMIZATION = 0

# init
include $(LIBE_PATH)/init.mk

# our own sources etc
BUILD_BINS = display
PROGRAM_BIN = display
FONTS += fontfixed6x8 fontcm30
display_SRC = main.c $(libe_SRC)

# build
include $(LIBE_PATH)/build.mk

# programmable targets
include $(LIBE_PATH)/flash/program.mk

-include local.mk
