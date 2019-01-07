
# include flash make info using target
ifeq ($(TARGET),avr)
include $(LIBE_PATH)/flash/avrdude.mk
else
endif
