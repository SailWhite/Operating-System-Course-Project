
#ifndef _ASM_MM_TLB_H
#define _ASM_MM_TLB_H

/* C functions corresponding to TLB instructions */

#define tlbp()		asm volatile ("tlbp")
#define tlbr()		asm volatile ("tlbr")
#define tlbwr()		asm volatile ("tlbwr")
#define tlbwi()		asm volatile ("tlbwi")

/* TLB "physical" entries (ENTRYLO0 and ENTRYLO1) */
#define TLB_PFN(entrylo)	((entrylo) >> 6)	/* PFN */
#define TLB_GLOBAL		0x1		/* ignore ASID */
#define TLB_VALID		0x2		/* valid */
#define TLB_DIRTY		0x4		/* writable */
#define TLB_CACHEMASK	0x38
# define TLB_UNCACHED	0x10		/* uncached access */
# define TLB_COHERENT	0x18		/* cache coherent */

/* TLB "virtual" entries (ENTRYHI) */
#define ENTRYHI_ASID_MASK	0xff
#define ENTRYHI_ASID(entryhi)	((entryhi) & ENTRYHI_ASID_MASK)

/* Only available in MIPS64 */
#if defined(_MIPS_ARCH_MIPS64) || defined(_MIPS_ARCH_MIPS64R2)
/* KSU mask is used for matching VA[63:62], though you could ignore it
 * safely. */
#define ENTRYHI_KSU_MASK	0xc000000000000000LL
# define ENTRYHI_KSU_USER	0x0000000000000000LL
# define ENTRYHI_KSU_SUPERVISOR	0x4000000000000000LL
# define ENTRYHI_KSU_KERNEL	0xc000000000000000LL
#define ENTRYHI_KSU(entryhi)	((entryhi) & ENTRYHI_KSU_MASK)
#endif

/*
 * You should probably write a function/macro for obtaining VPN
 * by your own, according to your design.
 */

#endif
