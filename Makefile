FAMILY ?= stm32/l0
ARCH ?= armv6-m
MCU ?= stm32l072czyx
OPENCM3_DIR ?= ./libopencm3
OPENCM3_LIB = libopencm3/lib/$(subst /,_,libopencm3_$(FAMILY)).a

# Building libopencm3 static .a library file 

libopencm3: $(OPENCM3_LIB)

$(OPENCM3_LIB):
	@printf "MAKE libopencm3 for $(FAMILY)"
	$(MAKE) -C libopencm3 TARGETS=$(FAMILY)


# Build kernel files


OBJ_DIR = build/
C_SRCS = kernel/src/kernel.c kernel/src/tasks.c kernel/src/memory.c
OBJS = $(OBJ_DIR)/kernel/kernel.o $(OBJ_DIR)/kernel/tasks.o $(OBJ_DIR)/kernel/memory.o 

ARCH_FLAGS  ?= -mcpu=cortex-m0plus -mthumb -DSTM32L0 -DZRTOS_ARCH_ARMV6M
Q			:= @
PREFIX		?= arm-none-eabi
CC 			:= $(PREFIX)-gcc
CFLAGS += -Os -ggdb3
CPPFLAGS += -MD


INCLUDES += -Ikernel/include -Iarch/$(ARCH)/include -Ilibopencm3/include
CFLAGS += $(INCLUDES)

all: $(OBJS)

$(OBJ_DIR)/kernel/%.o: kernel/src/%.c
	@mkdir -p $(dir $@)
	@printf " CC $<\n"
	$(Q)$(CC) $(CFLAGS) $(CPPFLAGS) $(ARCH_FLAGS) -o $@ -c $<
