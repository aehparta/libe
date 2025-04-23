
# create compilation options

# target extentions for binaries and libraries
TARGET_EXT ?= -$(TARGET)

# target specific stuff
ifeq ($(TARGET),X86)
    # pc
else ifeq ($(TARGET),RPI)
    # raspberry
else ifeq ($(TARGET),OPI)
    # orange pi
else ifeq ($(TARGET),ESP32)
    # esp32
else ifeq ($(TARGET),AVR)
    # microchip avr
    C_ARCH_PREFIX       ?= avr-
else ifeq ($(TARGET),PIC8)
    # microchip pic 8-bit
    C_ARCH_PREFIX       ?= xc8-
else ifeq ($(TARGET),PIC16)
    # microchip pic 16-bit
    C_ARCH_PREFIX       ?= xc16-
else ifeq ($(TARGET),PIC32)
    # microchip pic 32-bit
    C_ARCH_PREFIX       ?= xc32-
else ifeq ($(TARGET),MSP430)
    # ti msp430
    MSP430_GCC_PATH     ?= /opt/ti/msp430-gcc
    ifeq ($(MSP430_GCC_PATH),)
        C_ARCH_PREFIX   ?= msp430-
    else
        C_ARCH_PREFIX   ?= $(MSP430_GCC_PATH)/bin/msp430-elf-
    endif
else ifneq ($(TARGET),)
    $(error libe: target "$(TARGET)" not configured yet for use)
else
    $(error libe: target not set)
endif

# tools
CC          = $(C_ARCH_PREFIX)gcc
CXX         = $(C_ARCH_PREFIX)g++
OBJCOPY     = $(C_ARCH_PREFIX)objcopy
OBJDUMP     = $(C_ARCH_PREFIX)objdump
OBJSIZE     = $(C_ARCH_PREFIX)size
AR          = $(C_ARCH_PREFIX)ar
NM          = $(C_ARCH_PREFIX)nm
REMOVE      = rm

# compiler options
ifeq ($(TARGET),AVR)
    libe_CFLAGS     += -mmcu=$(shell echo $(MCU) | tr '[:upper:]' '[:lower:]')
    libe_CFLAGS     += -ffunction-sections -fdata-sections
    libe_CXXFLAGS   += -mmcu=$(shell echo $(MCU) | tr '[:upper:]' '[:lower:]')
    libe_CXXFLAGS   += -ffunction-sections -fdata-sections
    libe_LDFLAGS    += -mmcu=$(shell echo $(MCU) | tr '[:upper:]' '[:lower:]')
    libe_LDFLAGS    += -Wl,--gc-sections
    libe_ASFLAGS    += -mmcu=$(shell echo $(MCU) | tr '[:upper:]' '[:lower:]') -x assembler-with-cpp -Wa,-gstabs
else ifeq ($(TARGET),PIC8)
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
else ifeq ($(TARGET),PIC16)
    libe_CFLAGS  += -mcpu=$(MCU)
    libe_CFLAGS  += -ffunction-sections -fdata-sections -fomit-frame-pointer -no-legacy-libc -mpa
    libe_LDFLAGS += -T p$(MCU).gld
    libe_LDFLAGS += -Wl,--gc-sections
    OBJCOPY  = 
    BIN2HEX  = xc16-bin2hex
else ifeq ($(TARGET),PIC32)
    libe_CFLAGS  += -mprocessor=$(MCU)
    libe_CFLAGS  += -ffunction-sections -fdata-sections -no-legacy-libc
    libe_LDFLAGS += -mprocessor=$(MCU)
    libe_LDFLAGS += -Wl,--gc-sections
    libe_LDFLAGS += -Wl,--defsym=_min_heap_size=0,--gc-sections,--no-code-in-dinit,--no-dinit-in-serial-mem,-Map=$(TARGET).map,--cref
    OPTIMIZATION ?= 2
else ifeq ($(TARGET),MSP430)
    libe_CFLAGS  += -mmcu=$(shell echo $(MCU) | tr '[:upper:]' '[:lower:]')
    libe_CFLAGS  += -ffunction-sections -fdata-sections
    libe_LDFLAGS += -mmcu=$(shell echo $(MCU) | tr '[:upper:]' '[:lower:]')
    libe_LDFLAGS += -Wl,--gc-sections
    ifneq ($(MSP430_GCC_PATH),)
        libe_CFLAGS  += -I$(MSP430_GCC_PATH)/include -DMSP430_USING_TI_GCC
        libe_LDFLAGS += -L$(MSP430_GCC_PATH)/include
    endif
    STDC ?= c99
    STDCXX ?= c++99
else ifeq ($(TARGET),X86)
    libe_LDFLAGS += -lrt -lpthread
    ifneq ($(filter $(libe_DEFINES),USE_FTDI),)
        libe_LDFLAGS += -lftdi1
    endif
else ifeq ($(TARGET),RPI)
    libe_LDFLAGS += -lrt -lpthread
else ifeq ($(TARGET),OPI)
    libe_LDFLAGS += -lrt -lpthread
endif

AR_FLAGS     ?= rcs
OBJ_EXT      ?= .o
LIB_EXT      ?= .a
BIN_EXT      ?= .elf
ifeq ($(DEBUG),1)
    OPTIMIZATION = 0
endif
OPTIMIZATION ?= s
STDC ?= c11
STDCXX ?= c++11

# add target definition
libe_CFLAGS     += -DTARGET=$(TARGET) -DTARGET_$(TARGET)
libe_CXXFLAGS   += -DTARGET=$(TARGET) -DTARGET_$(TARGET)
# add MCU definition
ifneq ($(MCU),)
    libe_CFLAGS     += -DMCU=$(MCU) -DMCU_$(MCU)
    libe_CXXFLAGS   += -DMCU=$(MCU) -DMCU_$(MCU)
endif
# add MCU clock frequency
ifneq ($(F_CPU),)
    libe_CFLAGS     += -DF_CPU=$(F_CPU)UL
    libe_CXXFLAGS   += -DF_CPU=$(F_CPU)UL
endif

ifeq ($(TARGET),PIC8)
    libe_CFLAGS += -O$(OPTIMIZATION) -std=c99
else
    libe_CFLAGS += -D_GNU_SOURCE -O$(OPTIMIZATION) -g -Wall -std=$(STDC) -Wstrict-prototypes
    libe_CXXFLAGS += -D_GNU_SOURCE -O$(OPTIMIZATION) -g -Wall -std=$(STDCXX)
#     libe_CFLAGS += -Werror -Wno-unused-variable
endif
