
# check that LIBE_PATH is set
ifeq ($(LIBE_PATH),)
    $(error LIBE_PATH not set)
endif

# add target definition
libe_DEFINES += TARGET=$(shell echo $(TARGET) | tr '[:lower:]' '[:upper:]') TARGET_$(shell echo $(TARGET) | tr '[:lower:]' '[:upper:]')
# add MCU definitions
libe_DEFINES += MCU=$(shell echo $(MCU) | tr '[:lower:]' '[:upper:]') MCU_$(shell echo $(MCU) | tr '[:lower:]' '[:upper:]')

# unravel uses from environment and libe_USE
libe_USE += $(foreach use,$(use),$(use))
libe_DEFINES += $(foreach use,$(libe_USE),USE_$(use))

# unravel defines from environment
libe_DEFINES += $(foreach define,$(defines),$(define))

# debug
DEBUG ?= 1
ifeq ($(DEBUG),1)
    # debug on
    libe_DEFINES += DEBUG
endif

# add defines to cflags
libe_CFLAGS += $(foreach define,$(libe_DEFINES),-D$(define))

# add libe path to includes
libe_CFLAGS += -I$(LIBE_PATH)
