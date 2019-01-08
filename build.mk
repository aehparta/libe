
# check that LIBE_PATH is set
ifeq ($(LIBE_PATH),)
    $(error LIBE_PATH not set)
endif

# check that target is set
ifeq ($(TARGET),)
    $(error TARGET not set)
endif

# include compiler stuff
include $(LIBE_PATH)/cc.mk

# build directory
BUILDDIR ?= .build.$(TARGET)

# add color definitions
include $(LIBE_PATH)/colors.mk

# actions
all: start build

start:
	@echo $(LDC_DGRAYB) "TARGET: $(TARGET)" $(LDC_DEFAULT)
ifneq ($(RELEASE),)
	@echo $(LDC_DGRAYB) "RELEASE: $(RELEASE)" $(LDC_DEFAULT)
endif
ifeq ($(DEBUG),1)
	@echo $(LDC_DGRAYB) "DEBUG: YES" $(LDC_DEFAULT)
else
	@echo $(LDC_DGRAYB) "DEBUG: NO" $(LDC_DEFAULT)
endif
ifneq ($(MCU),)
	@echo $(LDC_DGRAYB) "MCU: $(MCU), CLOCK: $(F_CPU)" $(LDC_DEFAULT)
endif
	@echo $(LDC_DGRAYB) "USE: $(libe_USE)" $(LDC_DEFAULT)

build: clean $(BUILD_LIBS) $(BUILD_BINS)

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
.PHONY: all start build clean program

# compile binaries, this must be last because of secondary expansion
.SECONDEXPANSION:
$(BUILD_BINS): $$(patsubst %.c,$(BUILDDIR)/%$(OBJ_EXT),$$($$@_SRC)) $$(patsubst %$(OBJ_EXT),$(BUILDDIR)/%$(OBJ_EXT),$$(extra_OBJ)) $$(patsubst %,$(BUILDDIR)/%$(OBJ_EXT),$$($$@_RAW_SRC))
	@echo $(LDC_PURPLEB) "LINK $@$(TARGET_EXT)" $(LDC_DEFAULT)
	@$(CC) $^ -o $@$(TARGET_EXT)$(BIN_EXT) $(LDFLAGS)
ifneq ($(OBJDUMP),)
	@$(OBJDUMP) -S --disassemble $@$(TARGET_EXT)$(BIN_EXT) > $(BUILDDIR)/$@$(TARGET_EXT).asm
endif
$(BUILD_LIBS): $$(patsubst %.c,$(BUILDDIR)/%$(OBJ_EXT),$$($$@_SRC))
	@echo $(LDC_BLUEB) "CREATE $@$(TARGET_EXT).a" $(LDC_DEFAULT)
ifeq ($(TARGET),pic8)
	@$(AR) $(AR_FLAGS) -O$@$(TARGET_EXT)$(LIB_EXT) $^
else
	@$(AR) $(AR_FLAGS) $@$(TARGET_EXT)$(LIB_EXT) $^
endif
