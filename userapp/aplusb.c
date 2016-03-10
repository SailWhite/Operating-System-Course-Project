#include<stdio.h>
#include<ctype.h>
#include<sys/syscall.h>
int main() {
	char sa[50],sb[50];
	puts("a = ");	
	gets(sa,50);
	puts("b = ");	
	gets(sb,50);
	char *p;
	unsigned long a=0,b=0;
	for(p=sa;isdigit(*p);p++)a=a*10+*p-'0';
	for(p=sb;isdigit(*p);p++)b=b*10+*p-'0';
	printf("a + b = %d\n",a+b);
	return 0;
}
