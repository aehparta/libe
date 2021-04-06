
# programming support using avrdude

# avrdude base settings
ifeq ($(AVRDUDE_SUDO),1)
    AVRDUDE ?= sudo avrdude
else
    AVRDUDE ?= avrdude
endif
AVRDUDE_PROGRAMMER ?= avrisp2
ifneq ($(programmer),)
    AVRDUDE_PROGRAMMER = $(programmer)
endif
AVRDUDE_PORT ?= usb
AVRDUDE_FLAGS = -p $(MCU) -P $(AVRDUDE_PORT) -c $(AVRDUDE_PROGRAMMER)

program: $(PROGRAM_BIN)
	$(AVRDUDE) $(AVRDUDE_FLAGS) -U flash:w:$^$(TARGET_EXT)$(BIN_EXT)

