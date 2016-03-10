#ifndef ASM_SPINLOCK_H
#define ASM_SPINLOCK_H
#include <sys/types.h>
#define LOCK_BIT 0
struct spinlock {
	uint32_t state;
};

static inline void lock(struct spinlock* sl) {
	bool result;
	asm volatile (
		"1:	ll	%[reg], %[mem];"
		"	bnez	%[reg], 1b;"
		"	or	%[reg], %[val];"
		"	sc	%[reg], %[mem];"
		"	beqz	%[reg], 1b;"
		: [reg]"=&r"(result), [mem]"+m"(sl->state)
		: [val]"ir"(1 << LOCK_BIT)
	);
}

static inline void unlock(struct spinlock* sl) {
	sl->state=0;
}

#endif
