# firmware/Makefile

CROSS_COMPILE	= mips-linux-gnu-
LDSCRIPT		= firmware.ld
CC				= $(CROSS_COMPILE)gcc
CPP				= $(CROSS_COMPILE)cpp
LD				= $(CROSS_COMPILE)ld
OBJCOPY			= $(CROSS_COMPILE)objcopy
OBJDUMP			= $(CROSS_COMPILE)objdump
INCS			=  -I. -I../include -I../include/arch/mips -I../include/arch/mips/asm/mach-msim
LIB				=  ../lib

# TODO:
# Compiler-defined macros are given here using
#     -D<symbol>[=<value>] ...
DEFS		= -DELF32 -DFIRMWARE

CFLAGS		= -EL -nostdinc -nostdlib -mabi=32 -mips32 -fno-pic -g -w -mno-abicalls $(DEFS) $(INCS)

LDFLAGS		= -N -T $(LDSCRIPT)

# TODO:
# List your object files here, either compiled from C or from assembly.
OBJS		= firmware-entry.o firmware-main.o $(LIB)/*.o

ELF		= firmware
BINARY		= ../firmware.bin

# Overloads the behavior of compiling object files from assemblies
.S.o:
	@$(CC) $(CFLAGS) -c -o $*.o $<

$(ELF): $(OBJS)
	@$(LD) $(LDFLAGS) -o $@ $^

$(BINARY): $(ELF)
	@$(OBJCOPY) -O binary -j.text -j.rodata -j.data -j.bss $^ $@

all: $(BINARY)
clean:
	@rm -rf $(OBJS) $(ELF) $(BINARY)
msim: all
.PHONY: all clean msim
