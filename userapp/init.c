#include<sys/syscall.h>
#include<stdio.h>
#include<process.h>
int main() {
	puts("--DEBUG-- Hello World from user space!\n");
	for(;;){
		pid_t p=fork();
		if(!p) {
			exec("swsh");
		}
		waitpid(p);
	}
	return 0;
}
