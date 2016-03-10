
#ifndef _ASM_MM_TLB_H
#define _ASM_MM_TLB_H

/* C functions corresponding to TLB instructions */

#define tlbp()		asm volatile ("tlbp")
#define tlbr()		asm volatile ("tlbr")
#define tlbwr()		asm volatile ("tlbwr")
#define tlbwi()		asm volatile ("tlbwi")

/* TLB "physical" entries (ENTRYLO0 and ENTRYLO1) */
#define TLB_GLOBAL		0x1		/* ignore ASID */
#define TLB_VALID		0x2		/* valid */
#define TLB_DIRTY		0x4		/* writable */
#define TLB_CACHEMASK	0x38
# define TLB_UNCACHED	0x10		/* uncached access */
# define TLB_COHERENT	0x18		/* cache coherent */

#define TLB_SIZE			0x30
#define	IVLD_ASID			0xff
#define	MAX_VPN				(1<<10)
#define TLB_REFILL_ENTRY	0x80000000
#define	GENERIC_EXC_ENTRY	0x80000180
#define	INT_ENTRY_SIZE		0x80
#define	INT_CLOCK			7
#define	TLB_VPN(addr)		(addr>>21)
#define	TLB_VLAST(addr)		(addr&(0x1fffff))
#define VPAGE_SIZE			(1<<21)
#define	DEFAULT_PHY_ADDR	(0x90000000>>6)

/* TLB "virtual" entries (ENTRYHI) */
#define ENTRYHI_ASID_MASK	0xff
#define ENTRYHI_ASID(entryhi)	((entryhi) & ENTRYHI_ASID_MASK)

#define	TLB_PFN_MASK		0x001ff000
#define TLB_PFN(pfn, ehi)	((pfn|(ehi&TLB_PFN_MASK))>>6)
#define	TLB_ELO0TO1(elo0)	(elo0|0x40)
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
