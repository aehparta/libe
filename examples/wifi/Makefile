
# check that LIBE_PATH is set
ifeq ($(LIBE_PATH),)
    $(error LIBE_PATH not set)
endif

libe_USE += WIFI

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
