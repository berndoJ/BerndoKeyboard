# ------------------------------------------------------------------------------
# @file Makefile
# Project: InfiniKeys
# ------------------------------------------------------------------------------
# @author Johannes Berndorfer (berndoJ) [johannes@berndorfer.com]
# @date 25 Jan 2020
# ------------------------------------------------------------------------------
# Copyright (c) 2020 by Johannes Berndorfer (berndoJ) [johannes@berndorfer.com]
# ------------------------------------------------------------------------------
# @brief  Makefile for Infinikeys.
# ------------------------------------------------------------------------------

# --- MAKEFILE SETTINGS ---
LIBNAME       = libinfinikeys
VERSION_MAJOR = 0
VERSION_MINOR = 1
VERSION_REV   = 0

# --- COMPILER COMMANDS ---
GCC_PREFIX = arm-none-eabi-
GCC_CC     = $(GCC_PREFIX)gcc
GCC_AR     = $(GCC_PREFIX)ar

# --- C DEFINES ---
C_DEFS += -DINFINIKEYS_VER_MAJ=$(VERSION_MAJOR)
C_DEFS += -DINFINIKEYS_VER_MIN=$(VERSION_MINOR)
C_DEFS += -DINFINIKEYS_VER_REV=$(VERSION_REV)

# --- COMPILER FLAGS ---
MCU_FLAGS = -mcpu=cortex-m3 -mthumb
OPTIM_FLAGS = -Os
GCC_CC_FLAGS = $(MCU_FLAGS) $(OPTIM_FLAGS) $(C_DEFS) -Wall -fdata-sections -ffunction-sections -MMD -MP -MF"$(@:%.o=%.d)" -Wa,-a,-ad,-ahlms=$(<:.c=.lst)
GCC_ASM_FLAGS = $(MCU_FLAGS) $(OPTIM_FLAGS) -Wall -fdata-sections -ffunction-sections

# --- SOURCE AND BIN DIRECTORIES ---
SRCDIR = ./src
BINDIR = ./bin

# --- SOURCE FILES ---
C_SRC  = infinikeys.c
C_SRC += infinikeys_usb.c
C_SRC += infinikeys_usb_desc.c
C_SRC += infinikeys_cfg.c
C_SRC += infinikeys_kbmatrix.c
C_SRC += infinikeys_swacq.c
C_SRC += infinikeys_debug.c

# --- INCLUDE DIRECTORIES ---
INC = -I ./inc

# ------------------------------------------------------------------------------

OBJ = $(C_SRC:.c=.o)
DEPENDENCIES = $(addprefix $(BINDIR)/, $(C_SRC:.c=.d))

.PRECIOUS: $(SRCDIR)/%.c
.PRECIOUS: $(BINDIR)/%.o

all: __buildstart $(BINDIR)/$(LIBNAME).a __buildend

-include $(DEPENDENCIES)

.PHONY: __buildstart
__buildstart:
	@echo "[$(LIBNAME)] Building $(LIBNAME)..."

.PHONY: __buildend
__buildend:
	@echo "[$(LIBNAME)] Build finished."

.PHONY: rebuild
rebuild: clean all

.PHONY: clean
clean:
	@echo "[$(LIBNAME)] Cleaning up..."
	@rm -rf $(BINDIR)/*
	@echo "[$(LIBNAME)] Cleaned up successfully!"

$(BINDIR)/%.o: $(SRCDIR)/%.c | $(BINDIR) $(SRCDIR)
	@if ! test -d $(dir $@); then mkdir -p $(dir $@); echo "[$(LIBNAME)] Creating subdirectory $(dir $@) for output binaries."; fi
	@echo "[$(LIBNAME)] Compiling $< -> $@"
	@$(GCC_CC) -c $(GCC_CC_FLAGS) $(INC) $< -o $@

$(BINDIR)/%.a: $(addprefix $(BINDIR)/, $(OBJ)) | $(BINDIR)
	@echo "[$(LIBNAME)] Archiving library $@..."
	@if ! test -d $(dir $@); then mkdir -p $(dir $@); fi
	@if test -f $@; then rm $@; fi
	@$(GCC_AR) -rcs $@ $(addprefix $(BINDIR)/, $(OBJ))
	@echo "[$(LIBNAME)] Archived."

$(BINDIR):
	@echo "[$(LIBNAME)] Creating folder $(BINDIR)..."
	@mkdir -p $(BINDIR)

$(SRCDIR):
	@echo "[$(LIBNAME)] Creating folder $(SRCDIR)..."
	@mkdir -p $(SRCDIR)