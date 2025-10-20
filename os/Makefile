
BUILD_DIR = ./build
BIN_DIR   = ./bin
SRC_DIR   = ./src



# Toolchain
CC = i686-elf-gcc
AS = nasm
LD = i686-elf-ld
OBJCOPY = i686-elf-objcopy

# Compiler flags (kept from your original)
FLAGS = -g -ffreestanding -nostdlib -nostartfiles -nodefaultlibs -Wall -O0 -Iinc -Ilibary

LIB_SRCS := $(shell find libary -type f -name "*.c") #update; includes all c files ine every folder under libary
LIB_OBJS := $(patsubst libary/%.c, $(BUILD_DIR)/libary/%.o, $(LIB_SRCS))

DRI_SRCS := $(shell find drivers -type f -name "*.c")
DRI_OBJS := $(patsubst drivers/%.c, $(BUILD_DIR)/drivers/%.o,$(DRI_SRCS))

SRC_C := $(wildcard $(SRC_DIR)/**/*.c)
OBJS_C := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC_C))



OBJS = $(BUILD_DIR)/kernel.asm.o \
       $(BUILD_DIR)/kernel.o \
       $(BUILD_DIR)/isr.o \
       $(LIB_OBJS) \
       $(DRI_OBJS)
	

.PHONY: all clean run gdb-instructions

all: $(BIN_DIR)/davidos.img $(BUILD_DIR)/completeKernel.o

$(BUILD_DIR)/libary/%.o: libary/%.c
	@mkdir -p $(dir $@)
	$(CC) $(FLAGS) -std=gnu99 -c $< -o $@
	
$(BUILD_DIR)/drivers/%.o: drivers/%.c
	@mkdir -p $(dir $@)
	$(CC) $(FLAGS) -std=gnu99 -c $< -o $@


# ensure directories exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Bootloader (raw binary)
$(BIN_DIR)/boot.bin: $(SRC_DIR)/boot.asm | $(BIN_DIR)
	$(AS) -f bin $< -o $@

# Assemble kernel asm and ISR as elf32 objects
$(BUILD_DIR)/kernel.asm.o: $(SRC_DIR)/kernel.asm | $(BUILD_DIR)
	$(AS) -f elf32 -g $< -o $@

$(BUILD_DIR)/isr.o: $(SRC_DIR)/isr.asm | $(BUILD_DIR)
	$(AS) -f elf32 -g $< -o $@

# Compile C kernel
$(BUILD_DIR)/kernel.o: $(SRC_DIR)/kernel.c | $(BUILD_DIR)
	$(CC) $(FLAGS) -std=gnu99 -c $< -o $@


# Link flat binary kernel using your linker.ld (script expects to produce binary)
# Output: bin/kernel.bin (flat binary)
$(BIN_DIR)/kernel.bin: $(OBJS) $(SRC_DIR)/linker.ld | $(BIN_DIR)
	$(LD) -T $(SRC_DIR)/linker.ld -o $@ $(OBJS)

# Produce relocatable object containing all kernel symbols for GDB
# Output: build/completeKernel.o
$(BUILD_DIR)/completeKernel.o: $(OBJS) | $(BUILD_DIR)
	$(LD) -r -o $@ $(OBJS)








# New: 
$(BIN_DIR)/davidos.img: $(BIN_DIR)/boot.bin $(BIN_DIR)/kernel.bin
	dd if=/dev/zero of=$@ bs=512 count=65536
	dd if=$(BIN_DIR)/boot.bin of=$@ bs=512 seek=0 conv=notrunc
	dd if=$(BIN_DIR)/kernel.bin of=$@ bs=512 seek=1 conv=notrunc

#old:
#$(BIN_DIR)/davidos.img: $(BIN_DIR)/boot.bin $(BIN_DIR)/kernel.bin
#	dd if=$(BIN_DIR)/boot.bin of=$@ bs=512 conv=notrunc
#	dd if=$(BIN_DIR)/kernel.bin of=$@ bs=512 seek=1 conv=notrunc
#	dd if=/dev/zero bs=512 count=8 >> $@

clean:
	rm -rf $(BIN_DIR) $(BUILD_DIR)