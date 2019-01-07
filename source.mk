
# check that LIBE_PATH is set
ifeq ($(LIBE_PATH),)
    $(error LIBE_PATH not set)
endif

# libe sources
libe_SRC = \
	$(LIBE_PATH)/libe/nrf.c \
	$(LIBE_PATH)/libe/aes.c \
	$(LIBE_PATH)/libe/comm.c \
	$(LIBE_PATH)/libe/i2c.c \
	$(LIBE_PATH)/libe/target/$(TARGET)/os.c \
	$(LIBE_PATH)/libe/target/$(TARGET)/spi.c \
	$(LIBE_PATH)/libe/target/$(TARGET)/i2c.c \
	$(LIBE_PATH)/libe/target/$(TARGET)/log.c \
	$(LIBE_PATH)/libe/target/$(TARGET)/nvm.c \
	$(LIBE_PATH)/libe/target/$(TARGET)/pwm.c

# target specific stuff
ifeq ($(TARGET),X86)
	libe_SRC += $(LIBE_PATH)/libe/target/x86/ftdi.c $(LIBE_PATH)/libe/broadcast.c $(LIBE_PATH)/libe/target/linux/cfg.c
else ifeq ($(TARGET),RPI)
	libe_SRC += $(LIBE_PATH)/libe/broadcast.c $(LIBE_PATH)/libe/target/linux/cfg.c
endif
