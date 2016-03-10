# TODO:
# Refer to firmware.mk for TODOs.

CROSS_COMPILE	= mips-linux-gnu-
LDSCRIPT	= user.ld
PARTNO		= 2

CC		= $(CROSS_COMPILE)gcc
CPP		= $(CROSS_COMPILE)cpp
LD		= $(CROSS_COMPILE)ld
OBJCOPY		= $(CROSS_COMPILE)objcopy
OBJDUMP		= $(CROSS_COMPILE)objdump

INCS		= -I. -I./include -I./include/arch/mips
LIB			= ./lib
DEFS		= -DELF32

CFLAGS		= -EL -nostdinc -nostdlib -mabi=32 -mips32 -fno-pic -g -w -mno-abicalls $(DEFS) $(INCS)

LDFLAGS		= -N -T $(LDSCRIPT)

APPS		= $(wildcard *.c)
OBJS		= $(patsubst %.c,%.o, $(APPS)) $(LIB)/syscall.o $(LIB)/stdio.o $(LIB)/string.o user_entry.o
ELFS		= $(patsubst %.c,%,$(APPS))
BINS		= $(patsubst %.c,%.bin,$(APPS))

.S.o:
	@$(CC) $(CFLAGS) -c -o $*.o $<

elf:$(OBJS)
	@for fname in $(APPS);\
		do $(LD) $(LDFLAGS) -o $${fname%.c}\
			user_entry.o $${fname%.c}.o $(LIB)/*.o;\
	done

bin: elf
	@for fname in $(APPS);\
		do $(OBJCOPY) -O binary -j.text -j.rodata -j.data -j.bss\
			$${fname%.c} $${fname%.c}.bin;\
	done
	
all:bin


clean:
	@rm -rf $(OBJS) $(ELFS) $(BINS)

msim: all $(APPS)

.PHONY: all clean msim bin elf
