
# check that LIBE_PATH is set
ifeq ($(LIBE_PATH),)
    $(error LIBE_PATH not set)
endif

# check user given lower case
ifneq ($(target),)
    TARGET=$(target)
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
else ifeq ($(TARGET),rpi)
    # raspberry
else ifeq ($(TARGET),esp32)
    # esp32
else ifeq ($(TARGET),avr)
    # microchip avr
else ifeq ($(TARGET),pic8)
    # microchip pic 8-bit
else ifeq ($(TARGET),pic16)
    # microchip pic 16-bit
else ifeq ($(TARGET),pic32)
    # microchip pic 32-bit
else ifeq ($(TARGET),msp430)
    # ti msp430
else
    $(error libe: unsupported target or target not set)
endif

# defines
libe_DEFINES += -DTARGET=$(shell echo $(TARGET) | tr '[:lower:]' '[:upper:]') -DTARGET_$(shell echo $(TARGET) | tr '[:lower:]' '[:upper:]')
