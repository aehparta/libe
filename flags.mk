
# check that LIBE_PATH is set
ifeq ($(LIBE_PATH),)
    $(error LIBE_PATH not set)
endif

# add target definition
libe_DEFINES += TARGET=$(TARGET) TARGET_$(TARGET)
# add MCU definitions
ifneq ($(MCU),)
	libe_DEFINES += MCU=$(MCU) MCU_$(MCU)
endif

# unravel uses from environment, Makefile and libe_USE
libe_USE += $(foreach use,$(USE),$(use))
libe_USE += $(foreach use,$(use),$(shell echo $(use) | tr '[:lower:]' '[:upper:]'))
libe_DEFINES += $(foreach use,$(libe_USE),USE_$(use))

# unravel defines from environment
libe_DEFINES += $(foreach define,$(defines),$(shell echo $(define) | tr '[:lower:]' '[:upper:]'))

# debug
ifeq ($(debug),0)
	DEBUG = 0
endif
DEBUG ?= 1
ifeq ($(DEBUG),1)
    # debug on
    libe_DEFINES += DEBUG
endif

# add defines to cflags
libe_CFLAGS += $(foreach define,$(libe_DEFINES),-D$(define))

# add libe path to includes
libe_CFLAGS += -I$(LIBE_PATH)
