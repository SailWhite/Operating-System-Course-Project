#include<stdio.h>
#include<ctype.h>
#include<sys/syscall.h>
#ifndef NULL
#define NULL 0
#endif

unsigned long read_int() {
	char buf[50];
	gets(buf,50);
	char *p;
	unsigned long num=0;
	for(p=buf;isdigit(*p);p++)
		num=num*10+*p-'0';
	return num;
}

struct node {
	struct node*	next;
	unsigned long	val;
};

int main() {
	puts("n = ");
	unsigned long n=read_int();
	int i;
	struct node* h=malloc(sizeof(struct node));
	h->next=NULL;
	
	struct node* t;
	for(i=0;i<n;i++) {
		printf("a%d = ",i);
		struct node* a=malloc(sizeof(struct node));
		a->val=read_int();
		for(t=h;t!=NULL;t=t->next)
			if(t->next==NULL || t->next->val>=a->val) {
				a->next=t->next;
				t->next=a;
				break;
			}
	}
	
	puts("result: ");
	for(t=h->next;t!=NULL;t=t->next)
		printf("%d ",t->val);
	puts("\n");
	return 0;
}
