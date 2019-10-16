
# check that LIBE_PATH is set
ifeq ($(LIBE_PATH),)
    $(error LIBE_PATH not set)
endif

# unravel use
libe_USE += $(foreach use,$(USE),$(shell echo $(use) | tr '[:lower:]' '[:upper:]'))
libe_USE += $(foreach use,$(USE_$(TARGET)),$(shell echo $(use) | tr '[:lower:]' '[:upper:]'))
libe_USE += $(foreach use,$(USE_$(MCU)),$(shell echo $(use) | tr '[:lower:]' '[:upper:]'))
libe_USE += $(foreach use,$(use),$(shell echo $(use) | tr '[:lower:]' '[:upper:]'))
libe_DEFINES += $(foreach use,$(libe_USE),USE_$(use))

# unravel defines
libe_DEFINES += $(foreach define,$(DEFINES),$(define))
libe_DEFINES += $(foreach define,$(DEFINES_$(TARGET)),$(define))
libe_DEFINES += $(foreach define,$(DEFINES_$(MCU)),$(define))
libe_DEFINES += $(foreach define,$(defines),$(define))

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

# unravel cflags
CFLAGS += $(foreach flag,$(CFLAGS_$(TARGET)),$(flag))
CFLAGS += $(foreach flag,$(CFLAGS_$(MCU)),$(flag))

# unravel ldflags
LDFLAGS += $(foreach flag,$(LDFLAGS_$(TARGET)),$(flag))
LDFLAGS += $(foreach flag,$(LDFLAGS_$(MCU)),$(flag))
