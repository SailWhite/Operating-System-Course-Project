/*
 * Copyright (C) 2015 Gan Quan <coin2028@hotmail.com>
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 *
 */

#ifndef _ASM_IO_H
#define _ASM_IO_H

/*
 * TODO:
 * Change these header locations after you organize them
 */
#include <sys/types.h>
#include <asm/addrspace.h>
#include <asm/config.h>
#include <asm/spinlock.h>
#define iomap(addr)		(IO_BASE + (addr))

struct device {
	unsigned long           phys_addr;
	struct spinlock sl;
	//struct device_reader    reader;
	//struct device_writer    writer;
	unsigned int type;
	/* ... */
};

struct device_reader {
	/* The members can be null */
	uint8_t         (*in8)(struct device *, uint32_t);
	uint16_t        (*in16)(struct device *, uint32_t);
	uint32_t        (*in32)(struct device *, uint32_t);
	uint64_t        (*in64)(struct device *, uint32_t);
};

struct device_writer {
	void            (*out8)(struct device *, uint32_t, uint8_t);
	void            (*out16)(struct device *, uint32_t, uint16_t);
	void            (*out32)(struct device *, uint32_t, uint32_t);
	void            (*out64)(struct device *, uint32_t, uint64_t);
};


struct lp {
	struct device   dev;
	void            (*write_char)(struct lp *, unsigned char);
};

void write8(unsigned long addr, uint8_t data);
uint32_t read32(unsigned long addr);
void write32(unsigned long addr, uint32_t data);
void write64(unsigned long addr, uint64_t data);
uint64_t read64(unsigned long addr);
void device_register(struct device* dev);
#endif
