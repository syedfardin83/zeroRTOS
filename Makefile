FAMILY ?= stm32/l0
ARCH ?= armv6-m
MCU ?= stm32l072czyx
OPENCM3_DIR ?= ./libopencm3
OPENCM3_LIB = libopencm3/lib/$(subst /,_,libopencm3_$(FAMILY)).a

OBJ_DIR = build/
C_SRCS = kernel/src/kernel.c kernel/src/tasks.c kernel/src/memory.c arch
KERNEL_OBJS = $(OBJ_DIR)/kernel/kernel.o $(OBJ_DIR)/kernel/tasks.o $(OBJ_DIR)/kernel/memory.o 
ARCH_OBJS = $(OBJ_DIR)/arch/port.o

ARCH_FLAGS  ?= -mcpu=cortex-m0plus -mthumb -DSTM32L0 -DZRTOS_ARCH_ARMV6M
Q			:= @
PREFIX		?= arm-none-eabi
CC 			:= $(PREFIX)-gcc
AR 			:= $(PREFIX)-ar
CFLAGS += -Os -ggdb3
CPPFLAGS += -MD


INCLUDES += -I. -Ikernel/include -Iarch/$(ARCH)/include -Ilibopencm3/include
CFLAGS += $(INCLUDES)

LIB_NAME = lib/$(ARCH)/libzeroRTOS.a

all: $(KERNEL_OBJS) $(ARCH_OBJS) $(LIB_NAME)
	@printf "Built kernel files\n"
	@printf "Built arch files\n"
	@printf "Built $(LIB_NAME)\n"

# Build kernel files
$(OBJ_DIR)/kernel/%.o: kernel/src/%.c
	@mkdir -p $(dir $@)
	@printf " CC $<\n"
	$(Q)$(CC) $(CFLAGS) $(CPPFLAGS) $(ARCH_FLAGS) -o $@ -c $<

# Build arch files
$(OBJ_DIR)/arch/%.o: arch/$(ARCH)/src/%.c
	@mkdir -p $(dir $@)
	@printf " CC $<\n"
	$(Q)$(CC) $(CFLAGS) $(CPPFLAGS) $(ARCH_FLAGS) -o $@ -c $<

# Making the final .a file
$(LIB_NAME): $(KERNEL_OBJS) $(ARCH_OBJS)
	@mkdir -p $(dir $@)
	@printf " AR $@\n"
	$(Q)$(AR) rcs $@ $(KERNEL_OBJS) $(ARCH_OBJS)

# Building libopencm3 static .a library file 

libopencm3: $(OPENCM3_LIB)

$(OPENCM3_LIB):
	@printf "MAKE libopencm3 for $(FAMILY) \n" 
	$(MAKE) -C libopencm3 TARGETS=$(FAMILY)

clean:
	rm -rf build/arch
	rm -rf build/kernel
	rm -rf lib/armv6-m
	rm -rf lib/armv7-m
