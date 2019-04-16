
$(info  $(libe_CFLAGS))
$(info  $(LIBE_PATH))

PROJECT_NAME            = $(PROGRAM_BIN)
EXTRA_COMPONENT_DIRS    = . $(LIBE_PATH)
BUILD_DIR_BASE          = .build.esp32

EXTRA_CFLAGS += $(libe_CFLAGS)
EXTRA_LDFLAGS += $(libe_LDFLAGS)

libe_LDFLAGS +=

include $(IDF_PATH)/make/project.mk
