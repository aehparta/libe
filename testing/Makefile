
# check that LIBE_PATH is set
ifeq ($(LIBE_PATH),)
    $(error LIBE_PATH not set)
endif

# determine target
include $(LIBE_PATH)/target.mk

# enable spi
# libe_USE += SPI LOG
# ifdef TARGET_X86
#     libe_USE += FTDI
# endif

# flags and sources
include $(LIBE_PATH)/flags.mk
include $(LIBE_PATH)/source.mk

# our own sources etc
BUILD_BINS = testing
PROGRAM_BIN = testing
testing_SRC = main.c $(libe_SRC)

# other
CFLAGS += -DI2C_BITBANG_SCL=28 -DI2C_BITBANG_SDA=27

# build
include $(LIBE_PATH)/build.mk

# programmable targets
include $(LIBE_PATH)/flash/program.mk

-include local.mk
