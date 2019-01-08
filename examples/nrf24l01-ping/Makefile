
# check that LIBE_PATH is set
ifeq ($(LIBE_PATH),)
    $(error LIBE_PATH not set)
endif

# determine target
include $(LIBE_PATH)/target.mk

# enable spi
libe_USE += SPI
ifdef TARGET_X86
    libe_USE += FTDI
endif

# flags and sources
include $(LIBE_PATH)/flags.mk
include $(LIBE_PATH)/source.mk

# our own sources etc
BUILD_BINS = nrf24l01_ping
nrf24l01_ping_SRC = main.c $(libe_SRC)

# compile flags
CFLAGS += -D_GNU_SOURCE $(libe_CFLAGS)
LDFLAGS += $(libe_LDFLAGS)

# build
include $(LIBE_PATH)/build.mk

# programmable targets
include $(LIBE_PATH)/flash/program.mk

-include local.mk
