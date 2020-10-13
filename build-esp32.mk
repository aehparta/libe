
PROJECT_NAME            = $(PROGRAM_BIN)
EXTRA_COMPONENT_DIRS    += . $(LIBE_PATH)
# BUILD_DIR_BASE          = build.esp32

EXTRA_CFLAGS += $(libe_CFLAGS) -DTARGET_ESP32 -DTARGET=ESP32
EXTRA_LDFLAGS += $(libe_LDFLAGS)

libe_LDFLAGS +=

include $(IDF_PATH)/make/project.mk
