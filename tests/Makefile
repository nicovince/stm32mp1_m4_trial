BUILD_DIR ?= build
PROJECT = test_fifo

VPATH += ../common/
CFILES = fifo.c
CFILES += test_fifo.c
CFLAGS += -I../common
CFLAGS += -Wall

OBJS = $(CFILES:%.c=$(BUILD_DIR)/%.o)

# Be silent per default, but 'make V=1' will show all compiler calls.
# If you're insane, V=99 will print out all sorts of things.
V?=0
ifeq ($(V),0)
Q	:= @
NULL	:= 2>/dev/null
endif

all: $(PROJECT)

$(BUILD_DIR):
	$(Q)mkdir $@

$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	@printf "  CC\t$<\n"
	$(Q)$(CC) $(TGT_CFLAGS) $(CFLAGS) $(TGT_CPPFLAGS) $(CPPFLAGS) -o $@ -c $<

$(PROJECT): $(OBJS)
	@printf "  LD\t$@\n"
	$(Q)$(CC) $(LDFLAGS) $(OBJS) $(LDLIBS) -o $@

clean:
	$(Q)rm -Rf $(BUILD_DIR) $(PROJECT)
