# ------------------------------------------------------------------------------
# @file Makefile
# Master Makefile for this Project.
# ------------------------------------------------------------------------------
# @author Johannes Berndorfer (berndoJ) [johannes@berndorfer.com]
# @date 02 Jan 2020
# ------------------------------------------------------------------------------
# Copyright (c) 2020 by Johannes Berndorfer (berndoJ) [johannes@berndorfer.com]
# ------------------------------------------------------------------------------
# @brief  Main Makefile for this project. This makefile invokes all sub-
#         makefiles in the correct order to ensure the correct build sequence.
#         This project uses the recursive-make build architecture.
# ------------------------------------------------------------------------------
 
PROJECTNAME = BerndoKeyboard-Firmware
BINNAME     = $(PROJECTNAME)

FLASH_FOLDER = flash

# --- COMMAND DEFS ---
CPY        = cp
GCC_OBJCPY = arm-none-eabi-objcopy
GCC_SIZE   = arm-none-eabi-size

# --- MODULES AND LIBRARIES ---

LIB_STM32F1HAL = lib/libstm32f1hal
LIB_PCA9555    = lib/libpca9555
LIB_NEOPIXEL32 = lib/libneopixel32
MAIN_APP       = app

SUBMODULES = $(MAIN_APP) $(LIB_STM32F1HAL) $(LIB_PCA9555) $(LIB_NEOPIXEL32)

.PHONY: $(SUBMODULES)

.PHONY: all
all: $(MAIN_APP) $(FLASH_FOLDER)/$(BINNAME).elf $(FLASH_FOLDER)/$(BINNAME).hex $(FLASH_FOLDER)/$(BINNAME).bin bin_size

.PHONY: clean
clean:
	@for d in $(SUBMODULES); \
	do \
		$(MAKE) clean --directory=$$d --no-print-directory; \
	done
	@echo "[$(PROJECTNAME)] Cleaning up..."
	@rm -rf $(FLASH_FOLDER)/*
	@echo "[$(PROJECTNAME)] Cleaned up successfully!"

.PHONY: rebuild
rebuild: clean all

.PHONY: doc
doc:
	@doxygen Doxyfile

# --- RECURSIVE TARGET ---

$(SUBMODULES):
	@$(MAKE) --directory=$@ --no-print-directory

# --- DEPENDENCIES ---

$(MAIN_APP): $(LIB_STM32F1HAL) $(LIB_PCA9555) $(LIB_NEOPIXEL32)

# --- FILE MANAGEMENT ---

APP_BIN_FOLDER = $(MAIN_APP)/bin
APP_ELF = $(APP_BIN_FOLDER)/app.elf

$(FLASH_FOLDER)/$(BINNAME).elf: $(MAIN_APP) | $(FLASH_FOLDER) $(APP_BIN_FOLDER)
	@$(CPY) $(APP_ELF) $@
	@echo "[$(PROJECTNAME)] Copied ELF app binary to flash folder."

$(FLASH_FOLDER)/$(BINNAME).hex: $(FLASH_FOLDER)/$(BINNAME).elf
	@$(GCC_OBJCPY) -O ihex $< $@
	@echo "[$(PROJECTNAME)] Converted ELF app binary to HEX format."

$(FLASH_FOLDER)/$(BINNAME).bin: $(FLASH_FOLDER)/$(BINNAME).elf
	@$(GCC_OBJCPY) -O binary -S $< $@
	@echo "[$(PROJECTNAME)] Converted ELF app binary to BIN format."

.PHONY: bin_size
bin_size: $(FLASH_FOLDER)/$(BINNAME).elf
	@echo "[$(PROJECTNAME)] Binary size table:"
	@$(GCC_SIZE) $(FLASH_FOLDER)/$(BINNAME).elf

$(FLASH_FOLDER):
	@echo "[$(PROJECTNAME)] Creating folder $(FLASH_FOLDER)..."
	@mkdir -p $(FLASH_FOLDER)