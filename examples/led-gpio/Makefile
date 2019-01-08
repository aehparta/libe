
# check that LIBE_PATH is set
ifeq ($(LIBE_PATH),)
    $(error LIBE_PATH not set)
endif

# init
include $(LIBE_PATH)/init.mk

# our own sources etc
BUILD_BINS = led_gpio
PROGRAM_BIN = led_gpio
led_gpio_SRC = main.c $(libe_SRC)

# compile flags
CFLAGS += $(libe_CFLAGS)
LDFLAGS += $(libe_LDFLAGS)

# build
include $(LIBE_PATH)/build.mk

# programmable targets
include $(LIBE_PATH)/flash/program.mk

-include local.mk
