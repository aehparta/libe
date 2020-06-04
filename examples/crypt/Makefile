
TARGET = X86
USE += LOG LOG_COLORS AES RC5 XTEA XXTEA
DEBUG = 1


# init
include $(LIBE_PATH)/init.mk

# our own sources etc
BUILD_BINS = crypt
crypt_SRC = main.c $(libe_SRC)
LDFLAGS += -lmcrypt -lssl -lcrypto

# build
include $(LIBE_PATH)/build.mk
