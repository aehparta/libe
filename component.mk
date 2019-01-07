
# PROJECT_NAME            = libe
# EXTRA_COMPONENT_DIRS    = libe
# BUILD_DIR_BASE          = $(PROJECT_PATH)/.build.esp32

COMPONENT_OBJS = \
	libe/aes.o \
	libe/broadcast.o \
	libe/nrf.o \
	libe/target/esp32/i2c.o \
	libe/target/esp32/os.o \
	libe/target/esp32/spi.o \
	libe/target/esp32/log.o \
	libe/target/esp32/nvm.o \
	libe/target/esp32/wifi.o \
	libe/target/esp32/pwm.o


COMPONENT_SRCDIRS = libe libe/target/esp32
COMPONENT_ADD_INCLUDEDIRS = .

CFLAGS += -DTARGET_ESP32 -DTARGET=esp32 -DUSE_BROADCAST
