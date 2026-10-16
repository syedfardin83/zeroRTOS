BINARY = firmware

OBJS += kernel.o tasks.o memory.o

## My includes:
INCLUDES += -I../inc
CFLAGS += $(INCLUDES)

DEVICE = stm32l072czyx
OPENCM3_DIR = ../libopencm3

CFLAGS += -Os -ggdb3
CPPFLAGS += -MD
LDFLAGS += -static -nostartfiles
LDFLAGS += -Wl,-Map=$(BINARY).map
LDLIBS += -Wl,--start-group -lc -lgcc -lnosys -Wl,--end-group

.PHONY: clean all

all: $(BINARY).elf $(BINARY).hex $(BINARY).bin

clean:
	$(Q)$(RM) -rf $(BINARY).* *.o *.d

include $(OPENCM3_DIR)/mk/genlink-rules.mk
include $(OPENCM3_DIR)/mk/gcc-rules.mk

################################################################################
# OpenOCD specific variables

OOCD		?= openocd
OOCD_INTERFACE	?= stlink-v2-1
OOCD_TARGET	?= stm32l0

################################################################################
# Black Magic Probe specific variables
# Set the BMP_PORT to a serial port and then BMP is used for flashing
BMP_PORT	?=

################################################################################
# texane/stlink specific variables
#STLINK_PORT	?= :4242