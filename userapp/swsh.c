#include<sys/syscall.h>
#include<sys/types.h>
#include<stdio.h>
#include<ctype.h>
#include<string.h>
#include<process.h>
#ifndef NULL
#define NULL 0
#endif

#define BUF_SIZE	0x800

int main() {
	char *cmd[BUF_SIZE];
	char *path[BUF_SIZE];
	puts("Welcome to SWSH, type 'help' for help\n");
	while(1) {
		printf("SWSH: $ ");
		gets(cmd,BUF_SIZE);
		execute(cmd);
	}
	return 0;
}

int execute(char *cmd) {
	char *p=cmd,*q=cmd;
	bool join=1;
	while(*p)p++;
	while(!isgraph(*q))q++;
	while(!isgraph(*p) && p>q)(*(p--))='\0';
	/*if(*p=='&') {
		join=0;
		(*p--)='\0';
		while(!isgraph(*p) && p>q)(*(p--))='\0';
	}*/
	if(p<q)return;
	run(q,1);
}

int dispHelp() {
	puts(
		"	Usage: appName\n"
		"	appName in :\n"
		"		init			the init program, can also be excuted manually\n"
		"		swsh			this shell, you can run another as a child-shell\n"
		"		aplusb			input a,b, out put sum of them, a,b should both be pure digits in uint range\n"
		"		isort			a list based insertion sort, the claim of numbers are same as above\n"
		"		pidspeaker		fork twice then print pid 10 times with 1 second interval\n"
		"		help			display this help\n"
		"		exit			exit this shell\n"
		"	For details, please check the report\n"
	);
}

void run(char *cmd, bool join) {
	if(strcmp(cmd,"exit")==0 && join) {
		exit();
	}
	pid_t p=fork();
	if (p == 0) {
		if(strcmp(cmd,"help")==0) {
			dispHelp();
		} else if(strcmp(cmd,"exit")==0) {
			exit();
		} else {
			int err=exec(cmd);
			if(err!=0)printf("SWSH: %s: Invalid command, type 'help' for help\n",cmd);
		}
		exit(0);
	} else {
		if(join)
			waitpid(p);
	}
}
