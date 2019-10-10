
# check that LIBE_PATH is set
ifeq ($(LIBE_PATH),)
    $(error LIBE_PATH not set)
endif

# enable log and i2c
libe_USE += I2C LOG

# base init of makesystem
include $(LIBE_PATH)/init.mk

# our own sources etc
BUILD_BINS = i2c_scan
PROGRAM_BIN = i2c_scan
i2c_scan_SRC = main.c $(libe_SRC)

# include config or define things for pic8 xc8 compiler manually
ifeq ($(TARGET),pic8)
CFLAGS += -DI2C_BITBANG_SCL=0
CFLAGS += -DI2C_BITBANG_SDA=1
else
CFLAGS += -include ../config.h
endif

# build
include $(LIBE_PATH)/build.mk

# programmable targets
include $(LIBE_PATH)/flash/program.mk

-include local.mk
