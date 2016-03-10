/*bootstrap.S*/

#include <asm/asm.h>
#include <asm/regdef.h>
#include <asm/cp0regdef.h>
#include <mach/config.h>
#include <asm/stackframe.h>
#include <asm/mm/tlb.h>

.global cpu_stack
.global kernelsp
.global pt
.global cur_pid

NESTED(__reset, 0, ra)          /* 0x80300000 */
    mfc0    a0, CP0_STATUS
    ori     a0, ST_ERL
    xori    a0, ST_ERL
    mtc0    a0, CP0_STATUS
    LA      sp, cpu_stack
    ADDIU   sp, KSTACK_SIZE
    jal     init
END(__reset)

NESTED(generic_exception, 0, ra)
	j	__generic_exception
END(generic_exception)

__generic_exception:
	SAVE_ALL
	move	a0,	sp
	jal		handle_exception
	RESTORE_ALL_AND_RET
	
NESTED(tlb_refill, 0, ra)
    j       __tlb_refill
END(tlb_refill)

__tlb_refill:
	SAVE_ALL
	move	a0,	sp
	jal		handle_tlb_refill
	RESTORE_ALL_AND_RET
