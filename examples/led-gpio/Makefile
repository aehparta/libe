
# default target is x86
TARGET = X86

# enable gpio
USE += GPIO

# set default clock speed to 1 MHz for this example
F_CPU = 1000000

# avr
MCU_AVR = atmega8
F_CPU_AVR = 8000000
USE_AVR += LOG
DEFINES_AVR += LED_GPIO0=GPIOB0 LED_GPIO1=GPIOB1

# pic8
MCU_PIC8 = 16f18345
F_CPU_PIC8 = 16000000
USE_PIC8 += LOG
DEFINES_PIC8 += LED_GPIO0=GPIOC1 LED_GPIO1=GPIOC2
DEFINES_16LF18426 += PIC8_LOG_TXIF=TX1IF

# esp32
USE_ESP32 += LOG
DEFINES_ESP32 += LED_GPIO0=12 LED_GPIO1=13

# rpi
USE_RPI += LOG ERROR
DEFINES_RPI = LED_GPIO0=27 LED_GPIO1=17

# opi
USE_OPI += LOG ERROR
DEFINES_OPI = LED_GPIO0=0 LED_GPIO1=1

# x86
USE_X86 += LOG FTDI
DEFINES_X86 += LED_GPIO0=51 LED_GPIO1=52 FTDI_VID=0 FTDI_PID=0 FTDI_DESC=NULL FTDI_SERIAL=NULL

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
