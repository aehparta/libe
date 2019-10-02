
# check that LIBE_PATH is set
ifeq ($(LIBE_PATH),)
    $(error LIBE_PATH not set)
endif

# determine target
include $(LIBE_PATH)/target.mk

# enable i2c
libe_USE += I2C

# flags and sources
include $(LIBE_PATH)/flags.mk
include $(LIBE_PATH)/source.mk

# our own sources etc
BUILD_BINS = libe-i2c
PROGRAM_BIN = libe-i2c
libe-i2c_SRC = main.c $(libe_SRC)

# compile flags
CFLAGS += $(libe_CFLAGS)
LDFLAGS += $(libe_LDFLAGS)

# build
include $(LIBE_PATH)/build.mk

# programmable targets
include $(LIBE_PATH)/flash/program.mk

-include local.mk
