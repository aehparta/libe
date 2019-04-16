
PROJECT_NAME            = $(PROGRAM_BIN)
# EXTRA_COMPONENT_DIRS    = . $(LIBE_PATH)
BUILD_DIR_BASE          = $(PROJECT_PATH)/.build.esp32

EXTRA_CFLAGS += $(libe_CFLAGS)
EXTRA_LDFLAGS += $(libe_LDFLAGS)

libe_LDFLAGS +=

include $(IDF_PATH)/make/project.mk
