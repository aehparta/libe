
# check that target is set
ifeq ($(TARGET),)
    $(error TARGET not set)
endif

ifeq ($(TARGET),ESP32)
    include $(LIBE_PATH)/build-esp32.mk
# else ifeq ($(TARGET),PIC8)
#     include $(LIBE_PATH)/build-pic8.mk
else
    # Generic targets
    include $(LIBE_PATH)/build-generic.mk
endif
