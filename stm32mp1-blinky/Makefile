PROJECT = blinky
PROJECT_ST = $(PROJECT)_stld
BUILD_DIR = bin

SHARED_DIR = ../common
CFILES = main.c
CFILES += api.c
AFILES += api-asm.S
AFILES_ST += startup_stm32mp15xx.S

OBJS_ST = $(OBJS)
OBJS_ST += $(AFILES_ST:%.S=$(BUILD_DIR)/%.o)
LDSCRIPT_ST = stm32mp15xx_m4.ld
TGT_LDFLAGS_ST += -T$(LDSCRIPT_ST) -L$(OPENCM3_DIR)/lib -nostartfiles
TGT_LDFLAGS_ST += $(ARCH_FLAGS)
TGT_LDFLAGS_ST += -specs=nosys.specs -specs=nano.specs
TGT_LDFLAGS_ST += -Wl,--gc-sections
# OPTIONAL
TGT_LDFLAGS_ST += -Wl,-Map=$(PROJECT_ST).map
ifeq ($(V),99)
TGT_LDFLAGS_ST += -Wl,--print-gc-sections
endif

all: $(PROJECT_ST).elf $(PROJECT_ST).bin

# TODO - you will need to edit these two lines!
DEVICE=stm32mp157cac3
OOCD_FILE = board/stm32f4discovery.cfg

# You shouldn't have to edit anything below here.
VPATH += $(SHARED_DIR)
INCLUDES += $(patsubst %,-I%, . $(SHARED_DIR))
OPENCM3_DIR=../libopencm3

include $(OPENCM3_DIR)/mk/genlink-config.mk
include ../rules.mk
include $(OPENCM3_DIR)/mk/genlink-rules.mk

$(PROJECT_ST).elf: $(OBJS_ST) $(LIBDEPS)
	@printf "  LD\t$@\n"
	$(Q)$(LD) $(TGT_LDFLAGS_ST) $(LDFLAGS) $(OBJS_ST) $(LDLIBS) -o $@

GENERATED_BINS += $(PROJECT_ST).elf $(PROJECT_ST).bin

