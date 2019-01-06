
# this is defined only when compiling library itself
COMPILING_LIB = yes

# detect target and do other init stuff
include init.mk

# command line helper lib
libecmd_SRC = libe/cmd-common.c
# libe itself
libe_SRC = \
	libe/nrf.c \
	libe/aes.c \
	libe/comm.c \
	libe/target/$(TARGET)/os.c \
	libe/target/$(TARGET)/spi.c \
	libe/i2c.c \
	libe/target/$(TARGET)/i2c.c \
	libe/target/$(TARGET)/log.c \
	libe/target/$(TARGET)/nvm.c \
	libe/target/$(TARGET)/pwm.c

# what parts to build
#BUILD_BIN       = 
BUILD_LIB       = libe

# target specific stuff
ifeq ($(TARGET),x86)
	BUILD_LIB  += libecmd
	libe_SRC += libe/target/x86/ftdi.c libe/broadcast.c libe/target/linux/cfg.c
	CFLAGS += -DUSE_BROADCAST -DUSE_FTDI
else ifeq ($(TARGET),rpi)
	BUILD_LIB  += libecmd
	libe_SRC += libe/broadcast.c libe/target/linux/cfg.c
	CFLAGS += -DUSE_BROADCAST
endif

CFLAGS += -DUSE_NRF

# do actual build
-include local.mk
ifeq ($(target),esp32)
	include esp32.mk
else
	include common.mk
endif
