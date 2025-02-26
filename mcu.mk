
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

# detect parse possible variants

# pic8
ifneq (,$(findstring 16F183,$(MCU))$(findstring 16LF183,$(MCU)))
    VARIANT_UART = 16f183xx
    VARIANT_SPI = 16f183xx
    VARIANT_I2C = 16f183xx
endif
ifneq (,$(findstring 16F184,$(MCU))$(findstring 16LF184,$(MCU)))
    VARIANT_UART = 16f184xx
    VARIANT_SPI = 16f184xx
    VARIANT_I2C = 16f184xx
endif
