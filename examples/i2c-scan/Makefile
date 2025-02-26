
# default target is x86
TARGET = X86

# enable log and i2c
USE += I2C LOG GPIO

# pic8
MCU_PIC8 = 16F18446
F_CPU_PIC8 = 4000000
DEFINES_16LF18446 += PIC8_LOG_TX_PPS_VALUE=0x0f
DEFINES_16LF18446 += PIC8_LOG_TXIF=TX1IF PIC8_LOG_TXREG=TX1REG PIC8_LOG_TXSTA=TX1STA PIC8_LOG_RCSTA=RC1STA PIC8_LOG_SPBRG=SP1BRG

# x86
DEFINES_X86 += I2C_BITBANG_SCL=0 I2C_BITBANG_SDA=1 FTDI_VID=0 FTDI_PID=0 FTDI_DESC=NULL FTDI_SERIAL=NULL

# base init of makesystem
include $(LIBE_PATH)/init.mk

# our own sources etc
BUILD_BINS = i2c_scan
PROGRAM_BIN = i2c_scan
i2c_scan_SRC = main.c $(libe_SRC)

# build
include $(LIBE_PATH)/build.mk

# programmable targets
include $(LIBE_PATH)/flash/program.mk

-include local.mk
