
# target path helper
T_PATH = $(LIBE_PATH)/libe/target/$(shell echo $(TARGET) | tr '[:upper:]' '[:lower:]')

# libe sources
libe_SRC = \
    $(T_PATH)/os.c \
    $(LIBE_PATH)/libe/error.c

# linux generic
ifdef TARGET_LINUX
    libe_SRC += $(LIBE_PATH)/libe/target/linux/os.c
    ifneq ($(filter $(libe_DEFINES),USE_CFG),)
        libe_SRC += $(LIBE_PATH)/libe/target/linux/cfg.c
    endif
endif

# add gpio
ifneq ($(filter $(libe_DEFINES),USE_GPIO),)
    ifeq ($(TARGET),RPI)
        libe_SRC += $(LIBE_PATH)/libe/target/linux/gpio.c
    else ifeq ($(TARGET),OPI)
        libe_SRC += $(LIBE_PATH)/libe/target/linux/gpio.c
    else
        libe_SRC += $(T_PATH)/gpio.c
    endif
    # gpio drivers
    libe_SRC += $(LIBE_PATH)/libe/drivers/gpio/hd44780.c
    libe_SRC += $(LIBE_PATH)/libe/drivers/gpio/rot_enc.c
endif

# add logging
ifneq ($(filter $(libe_DEFINES),USE_LOG),)
    ifdef TARGET_LINUX
        libe_SRC += $(LIBE_PATH)/libe/target/linux/log.c
    else
        libe_SRC += $(T_PATH)/log.c
    endif
endif

# add spi and drivers for chips
ifneq ($(filter $(libe_DEFINES),USE_SPI),)
    # include native driver(s)
    ifneq (,$(wildcard $(T_PATH)/spi.c))
        libe_SRC += $(T_PATH)/spi.c
    endif
    # include linux specific drivers
    ifdef TARGET_LINUX
        libe_SRC += $(LIBE_PATH)/libe/target/linux/spidev.c
        ifneq ($(filter $(libe_DEFINES),USE_FTDI),)
            libe_SRC += $(LIBE_PATH)/libe/target/linux/spiftdi.c
        endif
    endif
    # include bitbang driver
    ifneq ($(filter $(libe_DEFINES),USE_SPI_BITBANG),)
        libe_SRC += $(LIBE_PATH)/libe/drivers/spi/bitbang.c
    endif

    # include device drivers
    libe_SRC += \
        $(LIBE_PATH)/libe/drivers/spi/nrf24l01p.c \
        $(LIBE_PATH)/libe/drivers/spi/nrf24l01p_ble.c
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
        $(LIBE_PATH)/libe/drivers/i2c/hdc1080.c \
        $(LIBE_PATH)/libe/drivers/i2c/sht21.c \
        $(LIBE_PATH)/libe/drivers/i2c/sht31.c \
        $(LIBE_PATH)/libe/drivers/i2c/fan5702.c \
        $(LIBE_PATH)/libe/drivers/i2c/cap1293.c \
        $(LIBE_PATH)/libe/drivers/i2c/act4751.c \
        $(LIBE_PATH)/libe/drivers/i2c/mcp3221.c \
        $(LIBE_PATH)/libe/drivers/i2c/mcp4725.c
    ifneq ($(filter $(libe_DEFINES),USE_I2C_BITBANG),)
        libe_SRC += $(LIBE_PATH)/libe/drivers/i2c/bitbang.c
    else ifdef TARGET_LINUX
        libe_SRC += $(LIBE_PATH)/libe/target/linux/i2c.c
    else
        libe_SRC += $(T_PATH)/i2c.c
    endif
endif

# add internal adc drivers
ifneq ($(filter $(libe_DEFINES),USE_ADC),)
    libe_SRC += $(T_PATH)/adci.c
endif

# add non-volatile memory (eeprom or similar)
ifneq ($(filter $(libe_DEFINES),USE_NVM),)
    libe_SRC += $(T_PATH)/nvm.c
endif

# add pwm
ifneq ($(filter $(libe_DEFINES),USE_PWM),)
    libe_SRC += $(T_PATH)/pwm.c
endif

# add software aes
ifneq ($(filter $(libe_DEFINES),USE_AES),)
    libe_SRC += $(LIBE_PATH)/libe/crypt/aes.c
endif

# add software rc5
ifneq ($(filter $(libe_DEFINES),USE_RC5),)
    libe_SRC += $(LIBE_PATH)/libe/crypt/rc5.c
endif

# add software xtea
ifneq ($(filter $(libe_DEFINES),USE_XTEA),)
    libe_SRC += $(LIBE_PATH)/libe/crypt/xtea.c
endif

# add software xxtea
ifneq ($(filter $(libe_DEFINES),USE_XXTEA),)
    libe_SRC += $(LIBE_PATH)/libe/crypt/xxtea.c
endif

# add broadcast
ifneq ($(filter $(libe_DEFINES),USE_BROADCAST),)
    libe_SRC += $(LIBE_PATH)/libe/broadcast.c
endif

# add ftdi
ifneq ($(filter $(libe_DEFINES),USE_FTDI),)
    libe_SRC += $(LIBE_PATH)/libe/target/linux/ftdi.c
endif

# add display drivers and draw functions
ifneq ($(filter $(libe_DEFINES),USE_DISPLAY),)
    libe_SRC += \
        $(LIBE_PATH)/libe/draw.c \
        $(LIBE_PATH)/libe/draw_string.c \
        $(LIBE_PATH)/libe/drivers/display/ssd1306.c
    ifneq ($(filter $(libe_DEFINES),USE_SDL2),)
        libe_SRC += $(LIBE_PATH)/libe/drivers/display/sdl2.c
    endif
    libe_SRC += $(foreach font,$(FONTS),$(LIBE_PATH)/libe/fonts/$(font).c)
    libe_SRC += $(foreach font,$(FONTS_$(TARGET)),$(LIBE_PATH)/libe/fonts/$(font).c)
    libe_SRC += $(foreach font,$(FONTS_$(MCU)),$(LIBE_PATH)/libe/fonts/$(font).c)
    libe_SRC += $(foreach font,$(fonts),$(LIBE_PATH)/libe/fonts/$(font).c)
endif
