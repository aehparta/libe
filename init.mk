
# default to autodetection
ifndef target
    ifeq ($(shell uname -p),x86_64)
        TARGET        = x86
        TARGET_EXT    = 
        BIN_PREFIX    = 
    else
    # raspberry
        TARGET        = rpi
        TARGET_EXT    = -$(TARGET)
        BIN_PREFIX    = 
    endif
endif
ifeq ($(target),x86)
    TARGET        = x86
    TARGET_EXT    = 
    BIN_PREFIX    = 
endif
# microchip avr
ifeq ($(target),avr)
    TARGET        = avr
    TARGET_EXT    = -$(TARGET)
    BIN_PREFIX    = avr-
    MCU           = atmega328p
    F_CPU         = 8000000L
    ifdef mcu
        MCU       = $(mcu)
    endif
endif
# microchip pic 8-bit
ifeq ($(target),pic8)
    TARGET        = pic8
    TARGET_EXT    = -$(TARGET)
    BIN_PREFIX    = xc8-
    MCU           = 18F26K20
    F_CPU         = 8000000L
    ifdef mcu
        MCU       = $(mcu)
    endif
endif
# microchip pic 16-bit
ifeq ($(target),pic16)
    TARGET        = pic16
    TARGET_EXT    = -$(TARGET)
    BIN_PREFIX    = xc16-
    MCU           = 24F16KA102
    F_CPU         = 16000000L
    ifdef mcu
        MCU       = $(mcu)
    endif
endif
# microchip pic 32-bit
ifeq ($(target),pic32)
    TARGET        = pic32
    TARGET_EXT    = -$(TARGET)
    BIN_PREFIX    = xc32-
    MCU           = 32MM0256GPM028
    F_CPU         = 16000000L
    ifdef mcu
        MCU       = $(mcu)
    endif
endif
# ti msp430
ifeq ($(target),msp430)
    MSP430_BASE_DIR    ?= $(HOME)/ti/msp430-gcc
    MSP430_BIN_DIR     ?= $(MSP430_BASE_DIR)/bin/
    MSP430_INC_DIR     ?= $(MSP430_BASE_DIR)/include/
    TARGET        = msp430
    TARGET_EXT    = -$(TARGET)
    BIN_PREFIX    = $(MSP430_BIN_DIR)msp430-elf-
    MCU           = msp430f5529
    F_CPU         = 16000000L
    ifdef mcu
        MCU       = $(mcu)
    endif
endif
# raspberry
ifeq ($(target),rpi)
    TARGET        = rpi
    TARGET_EXT    = -$(TARGET)
    BIN_PREFIX    = 
endif
# esp32
ifeq ($(target),esp32)
    TARGET        = esp32
    TARGET_EXT    = -$(TARGET)
    BIN_PREFIX    = 
endif

# check release
ifeq ($(release),)
    # default to alpha
    RELEASE       = alpha
else ifeq ($(release),alpha)
    RELEASE       = alpha
else ifeq ($(release),beta)
    RELEASE       = beta
else ifeq ($(release),stable)
    RELEASE       = stable
else
    $(error invalid release: $(release))
endif

# only define base FLAGS if not esp32
ifneq ($(target),esp32)
ifneq ($(target),pic8)
    LDFLAGS      = -lm
else
    LDFLAGS      =
endif
    CFLAGS       =
endif
