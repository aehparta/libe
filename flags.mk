
# check that LIBE_PATH is set
ifeq ($(LIBE_PATH),)
    $(error LIBE_PATH not set)
endif

# add defines to cflags
libe_CFLAGS += $(foreach define,$(libe_DEFINES),-D$(define))

# add libe path to includes
libe_CFLAGS += -I$(LIBE_PATH)
