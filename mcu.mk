
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

# detect possible variants and their specific features

# pic8
ifneq (,$(findstring 16F183,$(MCU))$(findstring 16LF183,$(MCU)))
    libe_DEFINES += MCU_VARIANT_16F183XX
else ifneq (,$(findstring 16F184,$(MCU))$(findstring 16LF184,$(MCU)))
    libe_DEFINES += MCU_VARIANT_16F184XX
endif

ifneq (,$(findstring 16F8,$(MCU)))
    VARIANT_I2C = old
else ifeq ($(TARGET),PIC8)
    VARIANT_I2C = generic
endif
