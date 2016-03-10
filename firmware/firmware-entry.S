/*firmware-entry.S*/

#include <asm/asm.h>
#include <asm/regdef.h>
#include <asm/cp0regdef.h>
#include <asm/smp.h>

NESTED(__reset, 0, ra)          /* 0xbfc00000 */
        mfc0    a0, CP0_STATUS
        ori     a0, ST_ERL
        xori    a0, ST_ERL
        mtc0    a0, CP0_STATUS
        cpuid   a1, a2          /* Put CPU ID in register a1 */
        bnez    a1, __slave_entry   /* Jump elsewhere if slave */
        li      sp, 0x8f000000  /* assuming RAM size > 0x0f000000 */
        jal     init

__slave_entry:
		sll		a1, 4
		addi	a2, a1, 4
        sw      zero, MSIM_ORDER_MAILBOX_BASE_VA(a1)
poll:   lw      a0, MSIM_ORDER_MAILBOX_BASE_VA(a1)
        beqz    a0, poll
		lw		sp, MSIM_ORDER_MAILBOX_BASE_VA(a2)
		jr      a0

END(__reset)

        .align  9
NESTED(__tlbrefill, 0, ra)               /* 0xbfc00200 */
		jal     __generic_exception
        /* Note that this function is empty */
END(__tlbrefill)

        .align  8
NESTED(__cacheerror, 0, ra)              /* 0xbfc00300 */
        jal     __generic_exception
END(__cacheerror)

LEAF(__generic_exception)              /* 0xbfc00380 */
        l0:
		j l0
		nop
END(__generic_exception)
