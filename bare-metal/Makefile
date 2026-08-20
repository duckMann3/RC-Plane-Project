# Compilers:
GCC=gcc
ARM_GCC=arm-none-eabi-as  # Compile startup file
ASM=arm-none-eabi-gcc     # Compile C code 
LOAD=arm-none-eabi-ld     # Link into .elf file
OBJ=arm-none-eabi-objcopy # Convert .elf file into .bin or .hexo

# Build Directory:
BUILD_DIR := ./build

all: $(BUILD_DIR)/$(TARGET).bin

# 1. TARGET: Compile Startup file:
# 	> Prereq: startup file .s -> startup_stm32f411xe.s
# 	> Recipe: arm-none-eabi-as startup_stm32f411xe.s
comp_startup: startup_stm32f411xe.s
	$(ARM_GCC) -o startup_stm32f411xe.s
# 2. Compile C Code:
# 3. Link Startup & C Code:
# 4. Convert .elf -> .bin or .hex:

clean:
	rm -rf $(BUILD_DIR)