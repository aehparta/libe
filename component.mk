
COMPONENT_SRCDIRS = \
    libe \
    libe/target/esp32 \
    libe/drivers/i2c \
    libe/drivers/gpio

ifneq ($(filter $(CFLAGS),-DUSE_DISPLAY),)
    COMPONENT_SRCDIRS += libe/drivers/display libe/fonts
endif

COMPONENT_ADD_INCLUDEDIRS = .
