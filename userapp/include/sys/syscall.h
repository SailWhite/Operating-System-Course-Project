#ifndef SYSCALL_H
#define SYSCALL_H

#define SYS_PID		0x0
#define SYS_PPID	0x1
#define SYS_FORK	0x2
#define SYS_EXEC	0x3
#define SYS_SLEEP	0x4
#define SYS_WAITPID	0x5
#define SYS_MLC		0x6
#define SYS_KNL		0x7
#define SYS_USR		0x8
#define SYS_EXIT	0x9
#define SYS_PUTS	0xA
#define SYS_GETS	0xB

#ifndef __ASSEMBLER__
#include <process.h>
#define MAC2STR(s)   #s
#define STR(m) MAC2STR(m)

#define syscall(sys_num)\
	unsigned int ret;\
	asm volatile (\
		"li	$2, "STR(sys_num)";"\
		"syscall;"\
		"move	%[ret], $2;"\
		: [ret]"=&r"(ret)\
	);\
	return ret

pid_t pid();
pid_t ppid();
pid_t fork();
pid_t waitpid(pid_t pid);
unsigned int sleep(unsigned int sec);
unsigned int exec(unsigned char* fname);
unsigned int exit();
unsigned int* malloc(unsigned int n);
unsigned int __to_kernel();
unsigned int __to_user();
unsigned int puts(unsigned char*s);
unsigned int gets(unsigned char* s,unsigned int len);
#endif

#endif
