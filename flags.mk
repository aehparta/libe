
# check target specific mcu
ifneq ($(MCU_$(TARGET)),)
    MCU = $(shell echo $(MCU_$(TARGET)) | tr '[:lower:]' '[:upper:]')
endif

# get mcu from environment if set
ifneq ($(mcu),)
    MCU = $(shell echo $(mcu) | tr '[:lower:]' '[:upper:]')
endif

# check target and mcu specific clock frequency
ifneq ($(F_CPU_$(TARGET)),)
    F_CPU = $(F_CPU_$(TARGET))
endif
ifneq ($(F_CPU_$(MCU)),)
    F_CPU = $(F_CPU_$(MCU))
endif

# get clock from environment if set
ifneq ($(f_cpu),)
    F_CPU = $(f_cpu)
endif

# unravel use
libe_USE += $(foreach use,$(USE),$(shell echo $(use) | tr '[:lower:]' '[:upper:]'))
libe_USE += $(foreach use,$(USE_$(TARGET)),$(shell echo $(use) | tr '[:lower:]' '[:upper:]'))
libe_USE += $(foreach use,$(USE_$(MCU)),$(shell echo $(use) | tr '[:lower:]' '[:upper:]'))
libe_USE += $(foreach use,$(use),$(shell echo $(use) | tr '[:lower:]' '[:upper:]'))
libe_DEFINES += $(foreach use,$(libe_USE),USE_$(use))

# unravel defines
libe_DEFINES += $(foreach define,$(DEFINES),$(define))
ifneq ($(DEFINES_$(TARGET)),)
	libe_DEFINES += $(foreach define,$(DEFINES_$(TARGET)),$(define))
endif
ifneq ($(DEFINES_$(MCU)),)
	libe_DEFINES += $(foreach define,$(DEFINES_$(MCU)),$(define))
endif
ifneq ($(defines),)
	libe_DEFINES += $(foreach define,$(defines),$(define))
endif

# debug
ifeq ($(debug),0)
	DEBUG = 0
endif
ifeq ($(debug),1)
	DEBUG = 1
endif
DEBUG ?= 0
ifeq ($(DEBUG),1)
    # debug on
    libe_DEFINES += DEBUG
endif

# add defines to cflags and cxxflags
ifneq ($(libe_DEFINES),)
	libe_CFLAGS     += $(foreach define,$(libe_DEFINES),-D$(define))
	libe_CXXFLAGS   += $(foreach define,$(libe_DEFINES),-D$(define))
endif

# add libe path to includes
libe_CFLAGS     += -I$(LIBE_PATH)
libe_CXXFLAGS   += -I$(LIBE_PATH)

# unravel cflags
ifneq ($(CFLAGS_$(TARGET)),)
	CFLAGS += $(foreach flag,$(CFLAGS_$(TARGET)),$(flag))
endif
ifneq ($(CFLAGS_$(MCU)),)
	CFLAGS += $(foreach flag,$(CFLAGS_$(MCU)),$(flag))
endif
ifneq ($(cflags),)
	CFLAGS += $(foreach flag,$(cflags),$(flag))
endif

# unravel cxxflags
ifneq ($(CXXFLAGS_$(TARGET)),)
	CXXFLAGS += $(foreach flag,$(CXXFLAGS_$(TARGET)),$(flag))
endif
ifneq ($(CXXFLAGS_$(MCU)),)
	CXXFLAGS += $(foreach flag,$(CXXFLAGS_$(MCU)),$(flag))
endif
ifneq ($(cxxflags),)
	CXXFLAGS += $(foreach flag,$(cxxflags),$(flag))
endif

# unravel ldflags
ifneq ($(LDFLAGS_$(TARGET)),)
	LDFLAGS += $(foreach flag,$(LDFLAGS_$(TARGET)),$(flag))
endif
ifneq ($(LDFLAGS_$(MCU)),)
	LDFLAGS += $(foreach flag,$(LDFLAGS_$(MCU)),$(flag))
endif
ifneq ($(ldflags),)
	LDFLAGS += $(foreach flag,$(ldflags),$(flag))
endif

# add libraries
ifneq ($(filter $(libe_DEFINES),USE_DRIVER_SDL2),)
	LDFLAGS += -lSDL2
endif
