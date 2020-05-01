PROJECT = openamp
BUILD_DIR = bin

SHARED_DIR = ../common
CFILES += main.c
CFILES += clock.c
CFILES += usart.c
CFILES += fifo.c
CFILES += led.c

LDLIBS += -lmetal -lopen_amp
LIBOPENAMP_DIR=../OpenAMP/open-amp/build/output/usr/local/lib
LIBMETAL_DIR=../OpenAMP/libmetal/build/output/usr/local/lib
TGT_LDFLAGS += -L$(LIBMETAL_DIR) -L$(LIBOPENAMP_DIR)

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
