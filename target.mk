
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
    libe_DEFINES += USE_LINUX
else ifeq ($(TARGET),rpi)
    # raspberry
    libe_DEFINES += USE_LINUX
else ifeq ($(TARGET),esp32)
    # esp32
else ifeq ($(TARGET),avr)
    # microchip avr
    libe_DEFINES += AES_OPT_8_BIT
else ifeq ($(TARGET),pic8)
    # microchip pic 8-bit
    libe_DEFINES += AES_OPT_8_BIT
else ifeq ($(TARGET),pic16)
    # microchip pic 16-bit
else ifeq ($(TARGET),pic32)
    # microchip pic 32-bit
else ifeq ($(TARGET),msp430)
    # ti msp430
else
    $(error libe: unsupported target or target not set)
endif

# add target definition
libe_DEFINES += TARGET=$(shell echo $(TARGET) | tr '[:lower:]' '[:upper:]') TARGET_$(shell echo $(TARGET) | tr '[:lower:]' '[:upper:]')

# unravel uses from environment
libe_DEFINES += $(foreach use,$(USE),USE_$(use))

# unravel defines from environment
libe_DEFINES += $(foreach define,$(DEFINES),$(define))

# debug
DEBUG ?= 1
ifeq ($(DEBUG),1)
    # debug on
    libe_DEFINES += DEBUG
endif
