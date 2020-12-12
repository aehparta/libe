
# programming support using ipecmd

# ipecmd base settings
IPECMD ?= /opt/microchip/mplabx/v5.05/mplab_platform/mplab_ipe/ipecmd.sh
IPECMD_PROGRAMMER ?= PK3
ifneq ($(programmer),)
    IPECMD_PROGRAMMER = $(programmer)
endif

program: $(PROGRAM_BIN)
	sudo $(IPECMD) -TP$(IPECMD_PROGRAMMER) -P$(MCU) -E -MP -OL -F$^$(TARGET_EXT).hex

