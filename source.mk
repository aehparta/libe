
# check that LIBE_PATH is set
ifeq ($(LIBE_PATH),)
    $(error LIBE_PATH not set)
endif

# libe sources, only os is mandatory
libe_SRC = $(LIBE_PATH)/libe/target/$(TARGET)/os.c

# target specific stuff
ifdef TARGET_LINUX
ifneq ($(filter $(libe_DEFINES),USE_CFG),)
    libe_SRC += $(LIBE_PATH)/libe/target/linux/cfg.c
endif
endif

# add logging
ifneq ($(filter $(libe_DEFINES),USE_LOG),)
    libe_SRC += $(LIBE_PATH)/libe/target/$(TARGET)/log.c
endif

# add spi and drivers for chips
ifneq ($(filter $(libe_DEFINES),USE_SPI),)
    libe_SRC += \
        $(LIBE_PATH)/libe/target/$(TARGET)/spi.c \
        $(LIBE_PATH)/libe/nrf.c
endif

# add i2c and drivers for chips
ifneq ($(filter $(libe_DEFINES),USE_I2C),)
    libe_SRC += \
        $(LIBE_PATH)/libe/target/$(TARGET)/i2c.c \
        $(LIBE_PATH)/libe/i2c.c
endif

# add non-volatile memory (eeprom or similar)
ifneq ($(filter $(libe_DEFINES),USE_NVM),)
    libe_SRC += $(LIBE_PATH)/libe/target/$(TARGET)/nvm.c
endif

# add pwm
ifneq ($(filter $(libe_DEFINES),USE_PWM),)
    libe_SRC += $(LIBE_PATH)/libe/target/$(TARGET)/pwm.c
endif

# add software aes
ifneq ($(filter $(libe_DEFINES),USE_AES),)
    libe_SRC += $(LIBE_PATH)/libe/aes.c
endif

# add ftdi
ifneq ($(filter $(libe_DEFINES),USE_FTDI),)
    libe_SRC += $(LIBE_PATH)/libe/target/x86/ftdi.c
endif

# add broadcast
ifneq ($(filter $(libe_DEFINES),USE_BROADCAST),)
    libe_SRC += $(LIBE_PATH)/libe/broadcast.c
endif
