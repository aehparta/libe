
# check that LIBE_PATH is set
ifeq ($(LIBE_PATH),)
    $(error LIBE_PATH not set)
endif

# check that target is set
ifeq ($(TARGET),)
    $(error TARGET not set)
endif

ifeq ($(TARGET),esp32)
    # ESP32
    include $(LIBE_PATH)/build-esp32.mk
else
    # Generic targets
    include $(LIBE_PATH)/build-generic.mk
endif
