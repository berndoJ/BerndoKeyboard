# ------------------------------------------------------------------------------
# @file Makefile
# Project / Module: libstm32f1hal
# ------------------------------------------------------------------------------
# @author Johannes Berndorfer (berndoJ) [johannes@berndorfer.com]
# @date 02 Jan 2020
# ------------------------------------------------------------------------------
# Copyright (c) 2020 by Johannes Berndorfer (berndoJ) [johannes@berndorfer.com]
# ------------------------------------------------------------------------------
# @brief  Makefile for the STM32F1HAL library.
#
# HOW TO CONFIGURE:
#  - Set the correct values in the 'MAKEFILE SETTINGS' section of this makefile.
#  - Check the library configuration header file ./config/stm32f1xx_hal_conf.h
#    and correct all configuration values according to the target platfrom.
# ------------------------------------------------------------------------------

# --- MAKEFILE SETTINGS ---
LIBNAME   = libstm32f1hal
VERSION_MAJOR = 1
VERSION_MINOR = 8
VERSION_REV   = 0
TARGET_STM32  = STM32F103xE

# --- COMPILER COMMANDS ---
GCC_PREFIX = arm-none-eabi-
GCC_CC     = $(GCC_PREFIX)gcc
GCC_AR     = $(GCC_PREFIX)ar

# --- C DEFINES ---
C_DEFS  = -DUSE_HAL_DRIVER
C_DEFS += -D$(TARGET_STM32)
C_DEFS += -DLIBSTM32F1HAL_VER_MAJ=$(VERSION_MAJOR)
C_DEFS += -DLIBSTM32F1HAL_VER_MIN=$(VERSION_MINOR)
C_DEFS += -DLIBSTM32F1HAL_VER_REV=$(VERSION_REV)

# --- COMPILER FLAGS ---
MCU_FLAGS = -mcpu=cortex-m3 -mthumb
OPTIM_FLAGS = -Os
GCC_CC_FLAGS = $(MCU_FLAGS) $(OPTIM_FLAGS) $(C_DEFS) -Wall -fdata-sections -ffunction-sections -MMD -MP -MF"$(@:%.o=%.d)" -Wa,-a,-ad,-ahlms=$(<:.c=.lst)
GCC_ASM_FLAGS = $(MCU_FLAGS) $(OPTIM_FLAGS) -Wall -fdata-sections -ffunction-sections

# --- SOURCE AND BIN DIRECTORIES ---
SRCDIR = ./src
BINDIR = ./bin

# --- SOURCE FILES ---
C_SRC  = Legacy/stm32f1xx_hal_can.c
C_SRC += stm32f1xx_hal.c
C_SRC += stm32f1xx_hal_adc.c
C_SRC += stm32f1xx_hal_adc_ex.c
C_SRC += stm32f1xx_hal_can.c
C_SRC += stm32f1xx_hal_cec.c
C_SRC += stm32f1xx_hal_cortex.c
C_SRC += stm32f1xx_hal_crc.c
C_SRC += stm32f1xx_hal_dac.c
C_SRC += stm32f1xx_hal_dac_ex.c
C_SRC += stm32f1xx_hal_dma.c
C_SRC += stm32f1xx_hal_eth.c
C_SRC += stm32f1xx_hal_exti.c
C_SRC += stm32f1xx_hal_flash.c
C_SRC += stm32f1xx_hal_flash_ex.c
C_SRC += stm32f1xx_hal_gpio.c
C_SRC += stm32f1xx_hal_gpio_ex.c
C_SRC += stm32f1xx_hal_hcd.c
C_SRC += stm32f1xx_hal_i2c.c
C_SRC += stm32f1xx_hal_i2s.c
C_SRC += stm32f1xx_hal_irda.c
C_SRC += stm32f1xx_hal_iwdg.c
C_SRC += stm32f1xx_hal_mmc.c
C_SRC += stm32f1xx_hal_nand.c
C_SRC += stm32f1xx_hal_nor.c
C_SRC += stm32f1xx_hal_pccard.c
C_SRC += stm32f1xx_hal_pcd.c
C_SRC += stm32f1xx_hal_pcd_ex.c
C_SRC += stm32f1xx_hal_pwr.c
C_SRC += stm32f1xx_hal_rcc.c
C_SRC += stm32f1xx_hal_rcc_ex.c
C_SRC += stm32f1xx_hal_rtc.c
C_SRC += stm32f1xx_hal_rtc_ex.c
C_SRC += stm32f1xx_hal_sd.c
C_SRC += stm32f1xx_hal_smartcard.c
C_SRC += stm32f1xx_hal_spi.c
C_SRC += stm32f1xx_hal_sram.c
C_SRC += stm32f1xx_hal_tim.c
C_SRC += stm32f1xx_hal_tim_ex.c
C_SRC += stm32f1xx_hal_uart.c
C_SRC += stm32f1xx_hal_usart.c
C_SRC += stm32f1xx_hal_wwdg.c
C_SRC += stm32f1xx_ll_adc.c
C_SRC += stm32f1xx_ll_crc.c
C_SRC += stm32f1xx_ll_dac.c
C_SRC += stm32f1xx_ll_dma.c
C_SRC += stm32f1xx_ll_exti.c
C_SRC += stm32f1xx_ll_fsmc.c
C_SRC += stm32f1xx_ll_gpio.c
C_SRC += stm32f1xx_ll_i2c.c
C_SRC += stm32f1xx_ll_pwr.c
C_SRC += stm32f1xx_ll_rcc.c
C_SRC += stm32f1xx_ll_rtc.c
C_SRC += stm32f1xx_ll_sdmmc.c
C_SRC += stm32f1xx_ll_spi.c
C_SRC += stm32f1xx_ll_tim.c
C_SRC += stm32f1xx_ll_usart.c
C_SRC += stm32f1xx_ll_usb.c
C_SRC += stm32f1xx_ll_utils.c

# --- INCLUDE DIRECTORIES ---
INC  = -I ./inc
INC += -I ./cmsis/inc
INC += -I ./config
INC += -I ../../app/inc

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
	@$(AR) -rcs $@ $(addprefix $(BINDIR)/, $(OBJ))
	@echo "[$(LIBNAME)] Archived."

$(BINDIR):
	@echo "[$(LIBNAME)] Creating folder $(BINDIR)..."
	@mkdir -p $(BINDIR)

$(SRCDIR):
	@echo "[$(LIBNAME)] Creating folder $(SRCDIR)..."
	@mkdir -p $(SRCDIR)