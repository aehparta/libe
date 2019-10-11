
# check that LIBE_PATH is set
ifeq ($(LIBE_PATH),)
    $(error LIBE_PATH not set)
endif

# default to autodetection
ifeq ($(TARGET),)
    ifeq ($(shell uname -p),x86_64)
        TARGET = x86
    else
    # raspberry
        TARGET = rpi
    endif
endif

# check that target is supported
ifeq ($(TARGET),x86)
    # pc
    TARGET_X86 = 1
    TARGET_LINUX = 1
    libe_DEFINES += TARGET_LINUX
else ifeq ($(TARGET),rpi)
    # raspberry
    TARGET_RPI = 1
    TARGET_LINUX = 1
    libe_DEFINES += TARGET_LINUX
else ifeq ($(TARGET),esp32)
    # esp32
    TARGET_ESP32 = 1
else ifeq ($(TARGET),avr)
    # microchip avr
    TARGET_AVR = 1
    libe_DEFINES += AES_OPT_8_BIT
else ifeq ($(TARGET),pic8)
    # microchip pic 8-bit
    TARGET_PIC8 = 1
    libe_DEFINES += AES_OPT_8_BIT
else ifeq ($(TARGET),pic16)
    # microchip pic 16-bit
    TARGET_PIC16 = 1
else ifeq ($(TARGET),pic32)
    # microchip pic 32-bit
    TARGET_PIC32 = 1
else ifeq ($(TARGET),msp430)
    # ti msp430
    TARGET_MSP430 = 1
else
    $(error libe: unsupported target or target not set)
endif
