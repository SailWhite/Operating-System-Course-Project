# lib/Makefile

CROSS_COMPILE	= mips-linux-gnu-
CC				= $(CROSS_COMPILE)gcc
CPP				= $(CROSS_COMPILE)cpp
INCS			=  -I. -I../include -I../include/arch/mips -I../include/arch/mips/asm/mach-msim
DEFS			= -DELF32
CFLAGS			= -EL -nostdinc -nostdlib -mabi=32 -mips32 -fno-pic -g -w -mno-abicalls $(DEFS) $(INCS)
OBJS			= $(patsubst %.c,%.o,$(wildcard *.c))
all: $(OBJS)
clean:
	@rm *.o
msim:all
.PHONY:all clean msim
