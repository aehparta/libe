
# check that LIBE_PATH is set
ifeq ($(LIBE_PATH),)
    $(error LIBE_PATH not set)
endif

# init
include $(LIBE_PATH)/init.mk

# our own sources etc
BUILD_BINS = nrf24l01_listen
PROGRAM_BIN = nrf24l01_listen
nrf24l01_listen_SRC = main.c $(libe_SRC)

# compile flags
CFLAGS += -D_GNU_SOURCE $(libe_CFLAGS)
LDFLAGS += $(libe_LDFLAGS)

# build
include $(LIBE_PATH)/build.mk

# programmable targets
include $(LIBE_PATH)/flash/program.mk

-include local.mk
