#
# common makefile include for most platforms
#

# tools
CC          = $(BIN_PREFIX)gcc
OBJCOPY     = $(BIN_PREFIX)objcopy
OBJDUMP     = $(BIN_PREFIX)objdump
OBJSIZE     = $(BIN_PREFIX)size
AR          = $(BIN_PREFIX)ar
NM          = $(BIN_PREFIX)nm
REMOVE      = rm
AVRDUDE     = avrdude
AR_FLAGS    = rcs
OPT         = s
OBJ_EXT     = .o
LIB_EXT     = .a
BIN_EXT     = .elf

# path of this library
LIBE_PATH ?= ../libe

# target specific stuff
ifeq ($(TARGET),avr)
	CFLAGS  += -DTARGET_AVR -mmcu=$(MCU) -DF_CPU=$(F_CPU) -DAES_OPT_8_BIT
	CFLAGS  += -ffunction-sections -fdata-sections
	LDFLAGS += -mmcu=$(MCU) -Wl,-u,vfprintf -lprintf_flt $(LIBE_PATH)/libe$(TARGET_EXT).a
	LDFLAGS += -Wl,--gc-sections
else ifeq ($(TARGET),pic8)
	CFLAGS  += -DTARGET_PIC8 -mcpu=$(MCU) -DF_CPU=$(F_CPU) -DAES_OPT_8_BIT
	CFLAGS  += -fno-short-float -fno-short-double
	LDFLAGS += -mcpu=$(MCU) $(LIBE_PATH)/libe$(TARGET_EXT).lpp -Wl,--gc-sections
	CC       = xc8-cc
	AR       = xc8
	AR_FLAGS = --output=lpp --chip=$(MCU)
	OBJ_EXT  = .p1
	LIB_EXT  = .lpp
	OPT      = 1
	OBJCOPY  = 
	OBJDUMP  =
else ifeq ($(TARGET),pic16)
	CFLAGS  += -DTARGET_PIC16 -mcpu=$(MCU) -DF_CPU=$(F_CPU)
	CFLAGS  += -ffunction-sections -fdata-sections -fomit-frame-pointer -no-legacy-libc -mpa
	LDFLAGS += -T p$(MCU).gld $(LIBE_PATH)/libe$(TARGET_EXT).a
	LDFLAGS += -Wl,--gc-sections
	OPT      = 1
	OBJCOPY  = 
	BIN2HEX  = xc16-bin2hex
else ifeq ($(TARGET),pic32)
	CFLAGS  += -DTARGET_PIC32 -mprocessor=$(MCU) -DF_CPU=$(F_CPU)
	CFLAGS  += -ffunction-sections -fdata-sections -no-legacy-libc
	LDFLAGS += -mprocessor=$(MCU) $(LIBE_PATH)/libe$(TARGET_EXT).a
	LDFLAGS += -Wl,--gc-sections -no-legacy-libc
	LDFLAGS += -Wl,--defsym=_min_heap_size=0,--gc-sections,--no-code-in-dinit,--no-dinit-in-serial-mem,-Map=$(TARGET).map,--cref
	OPT      = 1
else ifeq ($(TARGET),msp430)
	CFLAGS  += -mmcu=$(MCU) -DTARGET_MSP430 -DF_CPU=$(F_CPU) -I$(MSP430_INC_DIR)
	CFLAGS  += -ffunction-sections -fdata-sections
	#CFLAGS  += -mlarge -mcode-region=either -mdata-region=either
	LDFLAGS += -mmcu=$(MCU) $(LIBE_PATH)/libe$(TARGET_EXT).a -L$(MSP430_INC_DIR) -T$(MCU).ld
	LDFLAGS += -Wl,--gc-sections
	#LDFLAGS += -mlarge -mcode-region=either -mdata-region=either
	OPT      = s
else ifeq ($(TARGET),x86)
	CFLAGS  += -DTARGET_X86 -DUSE_LINUX
	LDFLAGS += $(LIBE_PATH)/libecmd$(TARGET_EXT).a $(LIBE_PATH)/libe$(TARGET_EXT).a -lftdi1 -lrt -lpthread -lpcre
	BIN_EXT  = 
else ifeq ($(TARGET),rpi)
	CFLAGS  += -DTARGET_RPI -DUSE_LINUX
	LDFLAGS += $(LIBE_PATH)/libecmd$(TARGET_EXT).a $(LIBE_PATH)/libe$(TARGET_EXT).a -lrt -lpthread -lpcre
endif

BUILDDIR     = ./.build.$(TARGET)

# check release
ifeq ($(RELEASE),alpha)
	# debug on
	CFLAGS += -DDEBUG
else
	CFLAGS += -DNDEBUG
endif

# customize compilation with specific targets
ifeq ($(TARGET),pic8)
	CFLAGS     += -std=c99 -O$(OPT)
else
	CFLAGS     += -D_GNU_SOURCE -std=gnu99 -g -O$(OPT) -Wall -Wstrict-prototypes -Werror
endif
CFLAGS     += -DTARGET=$(TARGET)
CFLAGS     += -I$(LIBE_PATH) -I.
LDFLAGS    += -O$(OPT)

# terminal colors
LDC_DEFAULT="\033[0m"
LDC_DGRAYB="\033[1;30m"
LDC_REDB="\033[1;31m"
LDC_GREENB="\033[1;32m"
LDC_YELLOWB="\033[1;33m"
LDC_BLUEB="\033[1;34m"
LDC_PURPLEB="\033[1;35m"
LDC_CYANB="\033[1;36m"
LDC_WHITEB="\033[1;37m"

# actions
all: start build
ifneq ($(BUILD_BIN),)
all: $(BUILD_BIN)
endif

start:
ifndef COMPILING_LIB
	@cd $(LIBE_PATH) && make target=$(TARGET) mcu=$(MCU) release=$(RELEASE)
endif
	@echo $(LDC_DGRAYB) "TARGET: $(TARGET)" $(LDC_DEFAULT)
	@echo $(LDC_DGRAYB) "RELEASE: $(RELEASE)" $(LDC_DEFAULT)
ifneq ($(MCU),)
	@echo $(LDC_DGRAYB) "MCU: $(MCU), CLOCK: $(F_CPU)" $(LDC_DEFAULT)
endif

build: clean $(BUILD_LIB) $(BUILD_BIN)

# objects from sources
$(BUILDDIR)/%$(OBJ_EXT): %.c
	@echo $(LDC_CYANB) "CC $<" $(LDC_DEFAULT)
	@mkdir -p `dirname $@`
	@$(CC) $(CFLAGS) -MMD -c $< -o $@
$(BUILDDIR)/%$(OBJ_EXT): %.s
	@echo $(LDC_CYANB) "CC $<" $(LDC_DEFAULT)
	@mkdir -p `dirname $@`
	@$(CC) $(CFLAGS) -MMD -c $< -o $@
$(BUILDDIR)/%$(OBJ_EXT): %
	@echo $(LDC_CYANB) "RAW $<" $(LDC_DEFAULT)
	@mkdir -p `dirname $@`
	@$(OBJCOPY) -I binary -I binary -O elf64-x86-64 -B i386 $< $@

clean:
	@$(REMOVE) -rf .build.*
	@$(REMOVE) -f *.a *.lpp *.hex *.eep *.obj *.cof *.elf *.map *.obj *.a90 *.sym *.lnk *.lss *.bin *.elf

hex:
ifneq ($(OBJCOPY),)
	@echo $(LDC_BLUEB) "CREATE $(PROGRAM_BIN)$(TARGET_EXT).hex" $(LDC_DEFAULT)
	@$(OBJCOPY) -O ihex -R .eeprom $(PROGRAM_BIN)$(TARGET_EXT)$(BIN_EXT) $(PROGRAM_BIN)$(TARGET_EXT).hex
endif
ifneq ($(BIN2HEX),)
	@echo $(LDC_BLUEB) "CREATE $(PROGRAM_BIN)$(TARGET_EXT).hex" $(LDC_DEFAULT)
	@$(BIN2HEX) $(PROGRAM_BIN)$(TARGET_EXT)$(BIN_EXT)
endif

size:
ifneq ($(OBJSIZE),)
	@echo $(LDC_BLUEB) "SIZE $(PROGRAM_BIN)$(TARGET_EXT).elf" $(LDC_DEFAULT)
	@$(OBJSIZE) $(PROGRAM_BIN)$(TARGET_EXT)$(BIN_EXT)
endif

usage:
ifneq ($(NM),)
	@echo $(LDC_BLUEB) "USAGE $(PROGRAM_BIN)$(TARGET_EXT).elf" $(LDC_DEFAULT)
	@$(NM) --radix=dec --print-size --size-sort $(PROGRAM_BIN)$(TARGET_EXT)$(BIN_EXT)
endif

# not real targets
.PHONY: all start build clean avrcheck program fuses programmable_check reset console

# compile binaries, this must be last because of secondary expansion
.SECONDEXPANSION:
$(BUILD_BIN): $$(patsubst %.c,$(BUILDDIR)/%$(OBJ_EXT),$$($$@_SRC)) $$(patsubst %$(OBJ_EXT),$(BUILDDIR)/%$(OBJ_EXT),$$(extra_OBJ)) $$(patsubst %,$(BUILDDIR)/%$(OBJ_EXT),$$($$@_RAW_SRC))
	@echo $(LDC_PURPLEB) "LINK $@$(TARGET_EXT)" $(LDC_DEFAULT)
	@$(CC) $^ -o $@$(TARGET_EXT)$(BIN_EXT) $(LDFLAGS)
ifneq ($(OBJDUMP),)
	@$(OBJDUMP) -S --disassemble $@$(TARGET_EXT)$(BIN_EXT) > $(BUILDDIR)/$@$(TARGET_EXT).asm
endif
$(BUILD_LIB): $$(patsubst %.c,$(BUILDDIR)/%$(OBJ_EXT),$$($$@_SRC))
	@echo $(LDC_BLUEB) "CREATE $@$(TARGET_EXT).a" $(LDC_DEFAULT)
ifeq ($(TARGET),pic8)
	@$(AR) $(AR_FLAGS) -O$@$(TARGET_EXT)$(LIB_EXT) $^
else
	@$(AR) $(AR_FLAGS) $@$(TARGET_EXT)$(LIB_EXT) $^
endif
