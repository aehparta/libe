
# include flash make info using target
ifeq ($(TARGET),AVR)
include $(LIBE_PATH)/flash/avrdude.mk
else ifeq ($(TARGET),PIC8)
include $(LIBE_PATH)/flash/ipecmd.mk
else
endif
