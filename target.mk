
# check that LIBE_PATH is set
ifeq ($(LIBE_PATH),)
    $(error LIBE_PATH not set)
endif

# get target from environment if set
ifneq ($(target),)
    TARGET = $(shell echo $(target) | tr '[:lower:]' '[:upper:]')
endif

# check that target is supported
ifeq ($(TARGET),X86)
    # pc
    TARGET_LINUX = 1
    libe_DEFINES += TARGET_LINUX
else ifeq ($(TARGET),RPI)
    # raspberry
    TARGET_LINUX = 1
    libe_DEFINES += TARGET_LINUX
else ifeq ($(TARGET),ESP32)
    # esp32
else ifeq ($(TARGET),AVR)
    # microchip avr
    libe_DEFINES += AES_OPT_8_BIT
else ifeq ($(TARGET),PIC8)
    # microchip pic 8-bit
    libe_DEFINES += AES_OPT_8_BIT
else ifeq ($(TARGET),pic16)
    # microchip pic 16-bit
else ifeq ($(TARGET),pic32)
    # microchip pic 32-bit
else ifeq ($(TARGET),msp430)
    # ti msp430
else
    $(error unsupported target or target not set, either set default target in Makefile using TARGET=<TARGET> or give target=<target> to make)
endif
