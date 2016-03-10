# TODO:
# Refer to firmware.mk for TODOs.

CROSS_COMPILE	= mips-linux-gnu-
LDSCRIPT	= kern.ld
PARTNO		= 2

CC		= $(CROSS_COMPILE)gcc
CPP		= $(CROSS_COMPILE)cpp
LD		= $(CROSS_COMPILE)ld
OBJCOPY		= $(CROSS_COMPILE)objcopy
OBJDUMP		= $(CROSS_COMPILE)objdump

INCS		= -I. -I../include -I../include/arch/mips -I../include/arch/mips/asm/mach-msim
LIB			= ../lib
DEFS		= -DELF32

CFLAGS		= -EL -nostdinc -nostdlib -mabi=32 -mips32 -fno-pic -g -w -mno-abicalls $(DEFS) $(INCS)

LDFLAGS		= -N -T $(LDSCRIPT)
APPS		= $(patsubst ../userapp/%.c,%.bin, $(wildcard ../userapp/*.c))
#init swsh pidspeaker aplusb
OBJS		= kernsyscall.o kernio.o kernel.o bootstrap.o ../lib/*.o
ELF		= kernel

# TODO: see boot.mk
DEVICE		= ../disk.img

# Specify the directory containing the install-kernel.pl script file.
TOOLSDIR	= ./tools

.S.o:
	@$(CC) $(CFLAGS) -c -o $*.o $<

$(ELF): $(OBJS)
	@$(LD) $(LDFLAGS) -o $@ $^ $(foreach app, $(APPS), -b binary ../userapp/$(app))

all: $(ELF)

clean:
	@rm -rf $(OBJS) $(ELF) $(BINARY)

msim: all
	@$(TOOLSDIR)/install-kernel.pl $(DEVICE) $(ELF) $(PARTNO)

.PHONY: all clean msim
