BINARY = firmware

OBJS += kernel.o tasks.o memory.o

## My includes:
INCLUDES += -I../inc
CFLAGS += $(INCLUDES)

FAMILY ?= stm32/l0
ARCH ?= ARMV6M
MCU ?= stm32l072czyx
OPENCM3_DIR ?= ./libopencm3
OPENCM3_LIB = libopencm3/lib/$(subst /,_,libopencm3_$(FAMILY)).a

# Building libopencm3 static .a library file 

libopencm3: $(OPENCM3_LIB)

$(OPENCM3_LIB):
	@printf "MAKE libopencm3 for $(FAMILY)"
	$(MAKE) -C libopencm3 TARGETS=$(FAMILY)

