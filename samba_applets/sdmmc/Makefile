# ----------------------------------------------------------------------------
#         SAM Software Package License
# ----------------------------------------------------------------------------
# Copyright (c) 2016, Atmel Corporation
#
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# - Redistributions of source code must retain the above copyright notice,
# this list of conditions and the disclaimer below.
#
# Atmel's name may not be used to endorse or promote products derived from
# this software without specific prior written permission.
#
# DISCLAIMER: THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR
# IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
# DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
# OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
# LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
# NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
# EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
# ----------------------------------------------------------------------------

# Makefile for compiling the SD/MMC SAM-BA applet

AVAILABLE_TARGETS = sama5d2-generic

AVAILABLE_VARIANTS = sram ddram

VARIANT ?= sram

TOP := ../..

BINNAME = applet-sdmmc

CONFIG_SAMBA_APPLET = y
CONFIG_TIMER_POLLING = y
CONFIG_SDMMC = y
CONFIG_LIB_SDMMC = y

CFLAGS_DEFS += -DSDMMC_TRIM_SDIO
CFLAGS_DEFS += -DSDMMC_LIB_TRACE_LEVEL=3
CFLAGS_DEFS += -DSDMMC_DRV_TRACE_LEVEL=3

obj-y += samba_applets/sdmmc/main.o
obj-y += samba_applets/sdmmc/pin_defs_$(chip-family).o

include $(TOP)/scripts/Makefile.rules
