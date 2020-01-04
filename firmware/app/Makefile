# ------------------------------------------------------------------------------
# @file Makefile
# Project / Module: Main Application
# ------------------------------------------------------------------------------
# @author Johannes Berndorfer (berndoJ) [johannes@berndorfer.com]
# @date 02 Jan 2020
# ------------------------------------------------------------------------------
# Copyright (c) 2020 by Johannes Berndorfer (berndoJ) [johannes@berndorfer.com]
# ------------------------------------------------------------------------------
# @brief  This makefile is responsible for building the main application and 
#         linking all other app-modules and libraries to it, as well as
#         generating the binaries.
# ------------------------------------------------------------------------------

MODULE_NAME    = app
MODULE_VER_MAJ = 1
MODULE_VER_MIN = 0
MODULE_VER_REV = 0
TARGET_STM32   = STM32F103xE
PROJECT_ROOT   = ..
LIB_FOLDER     = $(PROJECT_ROOT)/lib
LINKER_FOLDER  = $(PROJECT_ROOT)/linker
BIN_FOLDER     = ./bin
SOURCE_FOLDER  = ./src

LINKER_SCRIPT_FILENAME = STM32F103XE_FLASH.ld

# --- LINKED LIBRARIES ---
LLIB_STM32F1HAL = $(LIB_FOLDER)/libstm32f1hal
LLIB_PCA9555    = $(LIB_FOLDER)/libpca9555

LLIB_DIRS =  -L $(LLIB_STM32F1HAL)/bin
LLIB_DIRS += -L $(LLIB_PCA9555)/bin

LLIBS  = -lpca9555
LLIBS += -lstm32f1hal
LLIBS += -lc
LLIBS += -lm
LLIBS += -lnosys

# --- SOURCES ---
C_SOURCES  = main.c
C_SOURCES += mcu_init.c
C_SOURCES += isr.c
C_SOURCES += error.c
C_SOURCES += sys_clock.c
C_SOURCES += sys_gpio.c
C_SOURCES += sys_init.c
C_SOURCES += sys_pca9555.c
C_SOURCES += sys_i2c.c

ASM_SOURCES = startup_stm32f103xe.s

# --- C-DEFINES ---
C_DEFS  = -DUSE_HAL_DRIVER
C_DEFS += -D$(TARGET_STM32)
C_DEFS += -DMODULE_APP_VER_MAJ=$(MODULE_VER_MAJ)
C_DEFS += -DMODULE_APP_VER_MIN=$(MODULE_VER_MIN)
C_DEFS += -DMODULE_APP_VER_REV=$(MODULE_VER_REV)

# --- INCLUDE FOLDERS ---
INC_DIRS  = -I ./inc
INC_DIRS += -I $(LLIB_STM32F1HAL)/inc
INC_DIRS += -I $(LLIB_STM32F1HAL)/config
INC_DIRS += -I $(LLIB_STM32F1HAL)/cmsis/inc
INC_DIRS += -I $(LLIB_PCA9555)/inc

# --- TOOLCHAIN COMMANDS ---
GCC_PREFIX = arm-none-eabi-
GCC_CC     = $(GCC_PREFIX)gcc
GCC_LD     = $(GCC_PREFIX)gcc
GCC_ASM    = $(GCC_PREFIX)gcc -x assembler-with-cpp

# --- COMPILER SETTINGS ---
OPTIM_FLAGS = -Os
MCU_FLAGS = -mcpu=cortex-m3 -mthumb
GCC_CC_FLAGS = $(MCU_FLAGS) $(OPTIM_FLAGS) $(C_DEFS) -Wall -fdata-sections -ffunction-sections -MMD -MP -MF"$(@:%.o=%.d)" -Wa,-a,-ad,-ahlms=$(<:.c=.lst)
GCC_ASM_FLAGS = $(MCU_FLAGS) $(OPTIM_FLAGS) -Wall -fdata-sections -ffunction-sections

# --- LINKER SETTINGS ---
LINKER_SCRIPT = $(LINKER_FOLDER)/$(LINKER_SCRIPT_FILENAME)
LINKER_FLAGS  = -T$(LINKER_SCRIPT) $(MCU_FLAGS) -specs=nano.specs -Wl,--gc-sections

# ------------------------------------------------------------------------------

OBJ = $(C_SOURCES:.c=.o) $(ASM_SOURCES:.s=.o)
DEPENDENCIES = $(addprefix $(BIN_FOLDER)/, $(C_SOURCES:.c=.d))

.PRECIOUS: $(SOURCE_FOLDER)/%.c
.PRECIOUS: $(SOURCE_FOLDER)/%.s
.PRECIOUS: $(BIN_FOLDER)/%.o

# --- MAIN TARGET ---
all: __buildstart $(BIN_FOLDER)/app.elf __buildend

# --- DEPENDENCIES ---
-include $(DEPENDENCIES)

# --- CLEAN AND REBUILD TARGETS ---

.PHONY: clean
clean:
	@echo "[$(MODULE_NAME)] Cleaning up..."
	@rm -rf $(BIN_FOLDER)/*
	@echo "[$(MODULE_NAME)] Cleaned up successfully!"

.PHONY: rebuild
rebuild: clean all

# --- BUILD INFO TARGETS ---
.PHONY: __buildstart
__buildstart:
	@echo "[$(MODULE_NAME)] Building $(MODULE_NAME)..."

.PHONY: __buildend
__buildend:
	@echo "[$(MODULE_NAME)] Build finished."

# --- COMPILER TARGETS ---
$(BIN_FOLDER)/%.o: $(SOURCE_FOLDER)/%.c | $(BIN_FOLDER) $(SOURCE_FOLDER)
	@if ! test -d $(dir $@); then mkdir -p $(dir $@); echo "[$(MODULE_NAME)] Creating subdirectory $(dir $@) for output binaries."; fi
	@echo "[$(MODULE_NAME)] Compiling $< -> $@"
	@$(GCC_CC) -c $(GCC_CC_FLAGS) $(INC_DIRS) $< -o $@

$(BIN_FOLDER)/%.o: $(SOURCE_FOLDER)/%.s | $(BIN_FOLDER) $(SOURCE_FOLDER)
	@if ! test -d $(dir $@); then mkdir -p $(dir $@); echo "[$(MODULE_NAME)] Creating subdirectory $(dir $@) for output binaries."; fi
	@echo "[$(MODULE_NAME)] Compiling $< -> $@"
	@$(GCC_ASM) -c $(GCC_ASM_FLAGS) $< -o $@

# --- LINKER TARGETS ---
$(BIN_FOLDER)/app.elf: $(addprefix $(BIN_FOLDER)/, $(OBJ))
	@echo "[$(MODULE_NAME)] Linking project..."
	@$(GCC_LD) $(LINKER_FLAGS) $(LLIB_DIRS) $(addprefix $(BIN_FOLDER)/, $(OBJ)) $(LLIBS) -static -Wl,-Map=$(BIN_FOLDER)/app.map,--cref -o $@
	@echo "[$(MODULE_NAME)] Project linked successfully."
	@echo "[$(MODULE_NAME)] Created ELF format flash image."

$(BIN_FOLDER):
	@mkdir -p $(BIN_FOLDER)
