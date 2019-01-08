
# check that LIBE_PATH is set
ifeq ($(LIBE_PATH),)
    $(error LIBE_PATH not set)
endif

# libe sources
libe_SRC = \
    $(LIBE_PATH)/libe/target/$(TARGET)/os.c \
    $(LIBE_PATH)/libe/target/$(TARGET)/nvm.c \
    $(LIBE_PATH)/libe/aes.c

# target specific stuff
ifeq ($(TARGET),x86)
    libe_SRC += $(LIBE_PATH)/libe/target/linux/cfg.c
else ifeq ($(TARGET),rpi)
    libe_SRC += $(LIBE_PATH)/libe/target/linux/cfg.c
endif

# add ftdi only if said so
ifneq ($(filter $(libe_DEFINES),USE_FTDI),)
    libe_SRC += $(LIBE_PATH)/libe/target/x86/ftdi.c
endif

# add broadcast only if said so
ifneq ($(filter $(libe_DEFINES),USE_BROADCAST),)
    libe_SRC += $(LIBE_PATH)/libe/broadcast.c
endif

# add pwm only if said so
ifneq ($(filter $(libe_DEFINES),USE_PWM),)
    libe_SRC += $(LIBE_PATH)/libe/target/$(TARGET)/pwm.c
endif

# add logging stuff  if not disabled specifically
ifeq ($(filter $(libe_DEFINES),USE_NOT_LOG),)
    libe_SRC += $(LIBE_PATH)/libe/target/$(TARGET)/log.c
endif

# add spi stuff  if not disabled specifically
ifeq ($(filter $(libe_DEFINES),USE_NOT_SPI),)
    libe_SRC += \
        $(LIBE_PATH)/libe/target/$(TARGET)/spi.c \
        $(LIBE_PATH)/libe/nrf.c
endif

# add i2c stuff if not disabled specifically
ifeq ($(filter $(libe_DEFINES),USE_NOT_I2C),)
    libe_SRC += \
        $(LIBE_PATH)/libe/target/$(TARGET)/i2c.c \
        $(LIBE_PATH)/libe/i2c.c
endif
