#include <asm/asm.h>
#include <asm/regdef.h>
#include <sys/syscall.h>

NESTED(__start, 0, a0)
	jal		main
	move	a0,	v0
	li		v0,	SYS_EXIT
	syscall
END(__start)
