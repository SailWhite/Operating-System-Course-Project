#ifndef MACH_IO_H
#define MACH_IO_H
#include <mach/config.h>
#include <asm/io.h>
#define MSIM_LP_REG(lp,MSIM_LP_OUT) (lp->dev.phys_addr + MSIM_LP_OUT)
void msim_lp_write_char(struct lp *lp, unsigned char c);
void msim_lp_register(unsigned long paddr);
void msim_lp_init(unsigned long paddr);
void mach_init(void);
#endif