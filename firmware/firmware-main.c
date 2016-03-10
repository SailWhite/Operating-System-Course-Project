/*firmware-main.c*/

#include <stdio.h>
#include <string.h>
#include <mach/config.h>
#include <asm/config.h>
#include <sys/types.h>
#include <stdarg.h>
#include <stddef.h>
#include <asm/smp.h>

extern char __rodata_end__[];
extern char __data_begin__[], __data_end__[];
extern char __bss_begin__[], __bss_end__[];

void kputs(const char *str)
{
    volatile unsigned char *printer = PRT_ADDR;
    for (; *str != '\0'; ++str)
        *printer = *(unsigned char *)str;
}

void main(void)
{
    kputs("--DEBUG-- Hello world from BIOS in C!\n");
	char mbr[SECTOR_SIZE];
	readdisk(0,0,mbr,SECTOR_SIZE);
	(*((void (*)(void*,void*))(VTRC2VTRE(mbr))))(&readdisk,mbr);
}

void init(void)
{
	memcpy(__data_begin__, __rodata_end__, __data_end__ - __data_begin__);
	memset(__bss_begin__, 0, __bss_end__ - __bss_begin__);
	main();
}
