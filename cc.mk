
# check that LIBE_PATH is set
ifeq ($(LIBE_PATH),)
    $(error LIBE_PATH not set)
endif

# create compilation options

# target extentions for binaries and libraries
TARGET_EXT ?= -$(TARGET)

# target specific stuff
ifeq ($(TARGET),x86)
    # pc
else ifeq ($(TARGET),rpi)
    # raspberry
else ifeq ($(TARGET),esp32)
    # esp32
else ifeq ($(TARGET),avr)
    # microchip avr
    CC_PREFIX     ?= avr-
    MCU           ?= atmega328p
    F_CPU         ?= 8000000L
else ifeq ($(TARGET),pic8)
    # microchip pic 8-bit
    CC_PREFIX     ?= xc8-
    MCU           ?= 18F26K20
    F_CPU         ?= 8000000L
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
    MSP430_BASE_DIR    ?= $(HOME)/ti/msp430-gcc
    MSP430_BIN_DIR     ?= $(MSP430_BASE_DIR)/bin/
    MSP430_INC_DIR     ?= $(MSP430_BASE_DIR)/include/
    CC_PREFIX          ?= $(MSP430_BIN_DIR)msp430-elf-
    MCU                ?= msp430f5529
    F_CPU              ?= 16000000L
else
    $(error libe: unsupported target or target not set)
endif

# tools
CC          ?= $(CC_PREFIX)gcc
OBJCOPY     ?= $(CC_PREFIX)objcopy
OBJDUMP     ?= $(CC_PREFIX)objdump
OBJSIZE     ?= $(CC_PREFIX)size
AR          ?= $(CC_PREFIX)ar
NM          ?= $(CC_PREFIX)nm
REMOVE      ?= rm
AR_FLAGS    ?= rcs
OBJ_EXT     ?= .o
LIB_EXT     ?= .a
BIN_EXT     ?= .elf

# compiler options
ifeq ($(TARGET),avr)
    libe_DEFINES += -DF_CPU=$(F_CPU) -DAES_OPT_8_BIT
    libe_CFLAGS  += -mmcu=$(MCU)
    libe_CFLAGS  += -ffunction-sections -fdata-sections
    libe_LDFLAGS += -mmcu=$(MCU) -Wl,-u,vfprintf -lprintf_flt
    libe_LDFLAGS += -Wl,--gc-sections
else ifeq ($(TARGET),pic8)
    libe_DEFINES += -DF_CPU=$(F_CPU) -DAES_OPT_8_BIT
    libe_CFLAGS  += -mcpu=$(MCU)
    libe_CFLAGS  += -fno-short-float -fno-short-double
    libe_LDFLAGS += -mcpu=$(MCU) -Wl,--gc-sections
    CC       = xc8-cc
    AR       = xc8
    AR_FLAGS = --output=lpp --chip=$(MCU)
    OBJ_EXT  = .p1
    LIB_EXT  = .lpp
    OBJCOPY  = 
    OBJDUMP  =
else ifeq ($(TARGET),pic16)
    libe_DEFINES += -DF_CPU=$(F_CPU)
    libe_CFLAGS  += -mcpu=$(MCU)
    libe_CFLAGS  += -ffunction-sections -fdata-sections -fomit-frame-pointer -no-legacy-libc -mpa
    libe_LDFLAGS += -T p$(MCU).gld
    libe_LDFLAGS += -Wl,--gc-sections
    OBJCOPY  = 
    BIN2HEX  = xc16-bin2hex
else ifeq ($(TARGET),pic32)
    libe_DEFINES += -DF_CPU=$(F_CPU)
    libe_CFLAGS  += -mprocessor=$(MCU)
    libe_CFLAGS  += -ffunction-sections -fdata-sections -no-legacy-libc
    libe_LDFLAGS += -mprocessor=$(MCU)
    libe_LDFLAGS += -Wl,--gc-sections -no-legacy-libc
    libe_LDFLAGS += -Wl,--defsym=_min_heap_size=0,--gc-sections,--no-code-in-dinit,--no-dinit-in-serial-mem,-Map=$(TARGET).map,--cref
else ifeq ($(TARGET),msp430)
    libe_DEFINES += -DF_CPU=$(F_CPU)
    libe_CFLAGS  += -mmcu=$(MCU) -I$(MSP430_INC_DIR)
    libe_CFLAGS  += -ffunction-sections -fdata-sections
    #libe_CFLAGS  += -mlarge -mcode-region=either -mdata-region=either
    libe_LDFLAGS += -mmcu=$(MCU) -L$(MSP430_INC_DIR) -T$(MCU).ld
    libe_LDFLAGS += -Wl,--gc-sections
    #libe_LDFLAGS += -mlarge -mcode-region=either -mdata-region=either
else ifeq ($(TARGET),x86)
    libe_DEFINES += -DUSE_LINUX
    libe_LDFLAGS += -lftdi1 -lrt -lpthread -lpcre
else ifeq ($(TARGET),rpi)
    libe_DEFINES += -DUSE_LINUX
    libe_LDFLAGS += -lrt -lpthread -lpcre
endif

# ifeq ($(TARGET),pic8)
#     libe_CFLAGS     += -std=c99 -O$(OPT)
# else
#     libe_CFLAGS     += -D_GNU_SOURCE -std=gnu99 -g -O$(OPT) -Wall -Wstrict-prototypes -Werror
# endif
libe_CFLAGS += -I$(LIBE_PATH)

# debug
DEBUG ?= 1
ifeq ($(DEBUG),1)
    # debug on
    libe_DEFINES += -DDEBUG
endif
