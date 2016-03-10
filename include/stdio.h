/*
 * Copyright (C) 2015 Gan Quan <coin2028@hotmail.com>
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 *
 */

#ifndef _STDIO_H
#define _STDIO_H

#include <sys/types.h>
#include <stdarg.h>
#include <stddef.h>

int snprintf(char *str, size_t size, const char *fmt, ...);
int vsnprintf(char *str, size_t size, const char *fmt, va_list ap);
void kputs(const char *str);
int kprintf(const char *fmt, ...);

void readdisk(size_t sec_num, ssize_t offset, void *buf, size_t len);
void writedisk(size_t sec_num, ssize_t offset, const void *buf, size_t len);
#endif
