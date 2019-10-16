
# default target is x86
TARGET = X86

# enable gpio
USE += GPIO
# enable log and ftdi always on x86
USE_X86 += LOG FTDI

# set default clock speed to 1 MHz for this example
F_CPU=1000000

# target specific settings
MCU_AVR=atmega8
MCU_PIC8=16f84a

# init build system
include $(LIBE_PATH)/init.mk

# our own sources etc
BUILD_BINS = led_gpio
PROGRAM_BIN = led_gpio
led_gpio_SRC = main.c $(libe_SRC)

# build
include $(LIBE_PATH)/build.mk

# programmable targets
include $(LIBE_PATH)/flash/program.mk
