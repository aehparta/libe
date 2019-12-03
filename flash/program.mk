
# include flash make info using target
ifeq ($(TARGET),AVR)
include $(LIBE_PATH)/flash/avrdude.mk
else
endif
