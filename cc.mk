
# check that LIBE_PATH is set
ifeq ($(LIBE_PATH),)
    $(error LIBE_PATH not set)
endif

# get mcu from environment if set
ifneq ($(mcu),)
    MCU = $(shell echo $(mcu) | tr '[:lower:]' '[:upper:]')
endif

# get clock from environment if set
ifneq ($(f_cpu),)
    F_CPU = $(f_cpu)
endif

# create compilation options

# target extentions for binaries and libraries
TARGET_EXT ?= -$(TARGET)

# target specific stuff
ifeq ($(TARGET),X86)
    # pc
else ifeq ($(TARGET),RPI)
    # raspberry
else ifeq ($(TARGET),ESP32)
    # esp32
    MCU           ?= ESP32
else ifeq ($(TARGET),AVR)
    # microchip avr
    CC_PREFIX     ?= avr-
    MCU           ?= ATMEGA8
    F_CPU         ?= 8000000L
else ifeq ($(TARGET),PIC8)
    # microchip pic 8-bit
    CC_PREFIX     ?= xc8-
    MCU           ?= 16F18345
    F_CPU         ?= 16000000L
else ifeq ($(TARGET),pic16)
    # microchip pic 16-bit
    CC_PREFIX     ?= xc16-
    MCU           ?= 24F16KA102
    F_CPU         ?= 16000000L
else ifeq ($(TARGET),pic32)
    # microchip pic 32-bit
    CC_PREFIX     ?= xc32-
    MCU           ?= 32MM0256GPM028
    F_CPU         ?= 16000000L
else ifeq ($(TARGET),msp430)
    # ti msp430
#     ifeq ($(MSP430_BASE_DIR),)
#         $(error MSP430_BASE_DIR is not set, install example CCS and point this to msp430 base under it)
#     endif
#     ifeq ($(MSP430_TOOLS_DIR),)
#         $(error MSP430_TOOLS_DIR is not set, install example CCS and point this to msp430 tools under it)
#     endif
    MSP430_INC_DIR     ?= $(HOME)/ti/ccs920/ccs/ccs_base/msp430/include_gcc/
    CC_PREFIX          ?= msp430-elf-
    MCU                ?= msp430f1121
    F_CPU              ?= 4000000L
else
    $(error libe: unsupported target or target not set)
endif

# tools
CC          = $(CC_PREFIX)gcc
OBJCOPY     = $(CC_PREFIX)objcopy
OBJDUMP     = $(CC_PREFIX)objdump
OBJSIZE     = $(CC_PREFIX)size
AR          = $(CC_PREFIX)ar
NM          = $(CC_PREFIX)nm
REMOVE      = rm

# compiler options
ifeq ($(TARGET),AVR)
    libe_CFLAGS  += -DF_CPU=$(F_CPU)
    libe_CFLAGS  += -mmcu=$(MCU)
    libe_CFLAGS  += -ffunction-sections -fdata-sections
    libe_LDFLAGS += -mmcu=$(MCU)
    libe_LDFLAGS += -Wl,--gc-sections
else ifeq ($(TARGET),PIC8)
    libe_CFLAGS  += -DF_CPU=$(F_CPU)
    libe_CFLAGS  += -mcpu=$(MCU)
    libe_CFLAGS  += -fno-short-float -fno-short-double
    libe_LDFLAGS += -mcpu=$(MCU) -Wl,--gc-sections
    CC       = xc8-cc
    AR       = xc8
    AR_FLAGS ?= --output=lpp --chip=$(MCU)
    OBJ_EXT  ?= .p1
    LIB_EXT  ?= .lpp
    OBJCOPY  = 
    OBJDUMP  =
    OPTIMIZATION ?= 2
else ifeq ($(TARGET),pic16)
    libe_CFLAGS  += -DF_CPU=$(F_CPU)
    libe_CFLAGS  += -mcpu=$(MCU)
    libe_CFLAGS  += -ffunction-sections -fdata-sections -fomit-frame-pointer -no-legacy-libc -mpa
    libe_LDFLAGS += -T p$(MCU).gld
    libe_LDFLAGS += -Wl,--gc-sections
    OBJCOPY  = 
    BIN2HEX  = xc16-bin2hex
    OPTIMIZATION ?= 2
else ifeq ($(TARGET),pic32)
    libe_CFLAGS  += -DF_CPU=$(F_CPU)
    libe_CFLAGS  += -mprocessor=$(MCU)
    libe_CFLAGS  += -ffunction-sections -fdata-sections -no-legacy-libc
    libe_LDFLAGS += -mprocessor=$(MCU)
    libe_LDFLAGS += -Wl,--gc-sections -no-legacy-libc
    libe_LDFLAGS += -Wl,--defsym=_min_heap_size=0,--gc-sections,--no-code-in-dinit,--no-dinit-in-serial-mem,-Map=$(TARGET).map,--cref
    OPTIMIZATION ?= 2
else ifeq ($(TARGET),msp430)
    libe_CFLAGS  += -DF_CPU=$(F_CPU)
    libe_CFLAGS  += -mmcu=$(MCU)
    libe_CFLAGS  += -I$(MSP430_INC_DIR)
    libe_CFLAGS  += -ffunction-sections -fdata-sections
    #libe_CFLAGS  += -mlarge -mcode-region=either -mdata-region=either
    libe_LDFLAGS += -mmcu=$(MCU)
    libe_LDFLAGS += -L$(MSP430_INC_DIR)
    libe_LDFLAGS += -T$(MCU).ld
    libe_LDFLAGS += -Wl,--gc-sections -mhwmult=none
    #libe_LDFLAGS += -mlarge -mcode-region=either -mdata-region=either
else ifeq ($(TARGET),X86)
    libe_LDFLAGS += -lftdi1 -lrt -lpthread
else ifeq ($(TARGET),RPI)
    libe_LDFLAGS += -lrt -lpthread
endif

AR_FLAGS     ?= rcs
OBJ_EXT      ?= .o
LIB_EXT      ?= .a
BIN_EXT      ?= .elf
OPTIMIZATION ?= s

ifeq ($(TARGET),PIC8)
    libe_CFLAGS += -O$(OPTIMIZATION) -std=c99
else
    libe_CFLAGS += -D_GNU_SOURCE -O$(OPTIMIZATION) -std=c11 -g -Wall -Wstrict-prototypes
#     libe_CFLAGS += -Werror -Wno-unused-variable
endif
