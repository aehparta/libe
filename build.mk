
# check that target is set
ifeq ($(TARGET),)
    $(error TARGET not set)
endif

ifeq ($(TARGET),ESP32)
    # ESP32
    include $(LIBE_PATH)/build-esp32.mk
else
    # Generic targets
    include $(LIBE_PATH)/build-generic.mk
endif
