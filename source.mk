
# check that LIBE_PATH is set
ifeq ($(LIBE_PATH),)
    $(error LIBE_PATH not set)
endif

# libe sources
libe_SRC = \
    $(LIBE_PATH)/libe/target/$(TARGET)/os.c \
    $(LIBE_PATH)/libe/error.c

# linux generic
ifdef TARGET_LINUX
    libe_SRC += $(LIBE_PATH)/libe/target/linux/os.c
    ifneq ($(filter $(libe_DEFINES),USE_CFG),)
        libe_SRC += $(LIBE_PATH)/libe/target/linux/cfg.c
    endif
endif

# add logging
ifneq ($(filter $(libe_DEFINES),USE_LOG),)
    ifdef TARGET_LINUX
        libe_SRC += $(LIBE_PATH)/libe/target/linux/log.c
    else
        libe_SRC += $(LIBE_PATH)/libe/target/$(TARGET)/log.c
    endif
endif

# gpio drivers
ifneq ($(filter $(libe_DEFINES),USE_HD44780),)
    libe_SRC += $(LIBE_PATH)/libe/drivers/gpio/hd44780.c
endif

# add spi and drivers for chips
ifneq ($(filter $(libe_DEFINES),USE_SPI),)
    libe_SRC += \
        $(LIBE_PATH)/libe/target/$(TARGET)/spi.c \
        $(LIBE_PATH)/libe/drivers/spi/nrf.c
    ifneq ($(filter $(libe_DEFINES),USE_WIZNET),)
        ifeq ($(wildcard ioLibrary_Driver),)
            $(info Fetching WIZNET ioLibrary_Driver sources)
            $(shell git clone https://github.com/Wiznet/ioLibrary_Driver.git)
        endif
        libe_SRC += ioLibrary_Driver/Ethernet/wizchip_conf.c
        libe_SRC += ioLibrary_Driver/Ethernet/socket.c
        libe_SRC += ioLibrary_Driver/Ethernet/W5500/w5500.c
        libe_SRC += ioLibrary_Driver/Internet/DHCP/dhcp.c
        libe_SRC += ioLibrary_Driver/Internet/DNS/dns.c
        libe_CFLAGS += -D_WIZCHIP_=W5500 -D_WIZCHIP_IO_MODE_=0x0200
        libe_CFLAGS += -IioLibrary_Driver/Ethernet
#         libe_CFLAGS += -Wno-error=misleading-indentation
        libe_CFLAGS += -Wno-error=pointer-to-int-cast
    endif
endif

# add i2c and drivers for chips
ifneq ($(filter $(libe_DEFINES),USE_I2C),)
    libe_SRC += \
        $(LIBE_PATH)/libe/i2c.c \
        $(LIBE_PATH)/libe/drivers/i2c/mcp3221.c \
        $(LIBE_PATH)/libe/drivers/i2c/hdc1080.c \
        $(LIBE_PATH)/libe/drivers/i2c/sht21.c \
        $(LIBE_PATH)/libe/drivers/i2c/sht31.c \
        $(LIBE_PATH)/libe/drivers/i2c/fan5702.c \
        $(LIBE_PATH)/libe/drivers/i2c/cap1293.c
    ifneq ($(filter $(libe_DEFINES),USE_I2C_BITBANG),)
        libe_SRC += $(LIBE_PATH)/libe/drivers/i2c/bitbang.c
    else ifdef TARGET_LINUX
        libe_SRC += $(LIBE_PATH)/libe/target/linux/i2c.c
    else
        libe_SRC += $(LIBE_PATH)/libe/target/$(TARGET)/i2c.c
    endif
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

# add broadcast
ifneq ($(filter $(libe_DEFINES),USE_BROADCAST),)
    libe_SRC += $(LIBE_PATH)/libe/broadcast.c
endif

# add ftdi
ifneq ($(filter $(libe_DEFINES),USE_FTDI),)
    libe_SRC += $(LIBE_PATH)/libe/target/linux/ftdi.c
endif
