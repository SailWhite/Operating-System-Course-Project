#include<stdio.h>
#include<sys/syscall.h>
int main() {
	fork();
	fork();
	int i;
	for(i=0;i<10;i++) {
		printf("My pid is %d\n",pid());
		sleep(1);
	}
	return 0;
}
