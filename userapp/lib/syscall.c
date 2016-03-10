#include<sys/syscall.h>

pid_t pid() {
	syscall(SYS_PID);
}

pid_t ppid() {
	syscall(SYS_PPID);
}

pid_t fork() {
	syscall(SYS_FORK);
}

pid_t waitpid(pid_t pid) {
	syscall(SYS_WAITPID);
}

unsigned int sleep(unsigned int sec) {
	syscall(SYS_SLEEP);
}

unsigned int exec(unsigned char* fname) {
	syscall(SYS_EXEC);
}

unsigned int* malloc(unsigned int n) {
	syscall(SYS_MLC);
}

unsigned int __to_kernel() {
	syscall(SYS_KNL);
}

unsigned int __to_user() {
	syscall(SYS_USR);
}

unsigned int puts(unsigned char* s) {
	syscall(SYS_PUTS);
}

unsigned int gets(unsigned char* s,unsigned int len) {
	syscall(SYS_GETS);
}

unsigned int exit() {
	syscall(SYS_EXIT);
}
