
# programming support using ipecmd

# ipecmd base settings
IPECMD ?= /opt/microchip/mplabx/v5.05/mplab_platform/mplab_ipe/ipecmd.sh
ifeq ($(IPECMD_SUDO),1)
    IPECMD_RUN ?= sudo $(IPECMD)
else
    IPECMD_RUN ?= $(IPECMD)
endif

IPECMD_PROGRAMMER ?= PK3
ifneq ($(programmer),)
    IPECMD_PROGRAMMER = $(programmer)
endif

program: $(PROGRAM_BIN)
	$(IPECMD_RUN) -TP$(IPECMD_PROGRAMMER) -P$(MCU) -E -MP -OL -F$^$(TARGET_EXT).hex

