
LIBE_PATH := $(realpath $(LIBE_PATH))

# check that board is set
ifeq ($(BOARD),)
    $(error BOARD not set)
endif

# generate config.h and config.mk
ifneq (,$(wildcard config.yaml))
    gen_config_output := $(shell python3 $(LIBE_PATH)/tools/gen_config.py $(BOARD) --yaml config.yaml --output-dir .)
    include config.mk
endif

# detect target and do other init stuff
include $(LIBE_PATH)/target.mk
include $(LIBE_PATH)/mcu.mk
include $(LIBE_PATH)/flags.mk
include $(LIBE_PATH)/source.mk
