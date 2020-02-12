
TARGET = ESP32
USE += WIFI GPIO LOG

# init
include $(LIBE_PATH)/init.mk

# our own sources etc
BUILD_BINS = wifi
PROGRAM_BIN = wifi
wifi_SRC = main.c $(libe_SRC)

# build
include $(LIBE_PATH)/build.mk

# programmable targets
include $(LIBE_PATH)/flash/program.mk

-include local.mk
