# TODO:
# Refer to firmware.mk for TODOs.

CROSS_COMPILE	= mips-linux-gnu-

CC		= $(CROSS_COMPILE)gcc
CPP		= $(CROSS_COMPILE)cpp
LD		= $(CROSS_COMPILE)ld
OBJCOPY		= $(CROSS_COMPILE)objcopy
OBJDUMP		= $(CROSS_COMPILE)objdump

INCS		= -I. -I../include -I../include/arch/mips
LIB			= ../lib
DEFS		= -DELF32

# If you're out of bootloader space and you're not willing to
# debug your bootloader, consider adding a -Os option in
# CFLAGS.
CFLAGS		= -EL -nostdinc -nostdlib -mabi=32 -mips32 -w -fpic -g -mabicalls $(DEFS) $(INCS)

# TODO:
# Your bootloader should only contain one C source code file.
OBJS		= bootsect.o
BINARY		= bootsect.bin

# TODO:
# Give your disk image file here.
DEVICE		= ../disk.img

$(BINARY): $(OBJS)
	@$(OBJCOPY) -O binary -j.text -j.data -j.bss $^ $@

all: $(BINARY)

clean:
	@rm $(OBJS) $(BINARY)

msim: all
	@if [ $(shell wc -c $(BINARY) | cut -f 1 -d " ") -ge 446 ]; then \
		echo Bootloader too large; \
	else \
		dd if=$(BINARY) of=$(DEVICE) conv=notrunc; \
	fi
.PHONY: all clean msim
