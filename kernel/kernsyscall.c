#include<process.h>
#include<string.h>
#include<asm/stackframe.h>
#include<asm/mipsregs.h>
#include <asm/addrspace.h>
#include <asm/mm/tlb.h>

#define min(x,y) ((x)<(y)?(x):(y))

extern struct pcb_entry	pcb[];
extern pid_t			cur_pid;
extern pid_t			first_free_pid;
extern struct pcb_entry	pcb[MAX_PID];
extern unsigned int 	pt[][MAX_VPN];
unsigned long			pbuf[STACK_SIZE];

extern unsigned long 	sc_num, sc_ret, sc_args[];
extern pid_t			sc_pid;

extern unsigned long cur_pages[];
extern unsigned long free_ppage[];
extern unsigned long first_free_ppage;

extern char _binary____userapp_init_bin_start[],_binary____userapp_init_bin_end[];
extern char _binary____userapp_swsh_bin_start[],_binary____userapp_swsh_bin_end[];
extern char _binary____userapp_pidspeaker_bin_start[],_binary____userapp_pidspeaker_bin_end[];
extern char _binary____userapp_aplusb_bin_start[],_binary____userapp_aplusb_bin_end[];
extern char _binary____userapp_isort_bin_start[],_binary____userapp_isort_bin_end[];

pid_t pid_alloc() {
	pcb[first_free_pid].ppid=cur_pid;
	pid_t ret=first_free_pid;
	first_free_pid=pcb[first_free_pid].next;
	pcb[ret].next=pcb[0].next;
	pcb[0].next=ret;
	pcb[ret].max_addr=USEG_BEGIN;
	pcb[ret].status=ST_READY;
	pcb[ret].wait=0;
	pcb[ret].compare=0;
	unsigned long i;
	for(i=0;i<MAX_VPN;i++)pt[ret][i]=0;
	return ret;
}

pid_t pid_free(pid_t pp, pid_t p) {
	pcb[pp].next=pcb[p].next;
	pcb[p].next=first_free_pid;
	first_free_pid=p;
	return pcb[pp].next;
}


unsigned long ppage_alloc() {
	unsigned long ret=PHY_ADDR_BEGIN+first_free_ppage*VPAGE_SIZE;
	first_free_ppage=free_ppage[first_free_ppage];
	return ret;
}

void ppage_free(unsigned long pp) {
	free_ppage[pp]=free_ppage[first_free_ppage];
	first_free_ppage=pp;
}

void* kmalloc(unsigned int n) {
	unsigned long ret=pcb[cur_pid].max_addr;
	unsigned long last=TLB_VLAST(ret);
	if(last==0)last=VPAGE_SIZE;
	if(last+n<VPAGE_SIZE) {
		pcb[cur_pid].max_addr+=n;
	} else {
		pcb[cur_pid].max_addr+=VPAGE_SIZE-last;
		for(n-=VPAGE_SIZE-last;n>VPAGE_SIZE;n-=VPAGE_SIZE) {
			pcb[cur_pid].max_addr+=VPAGE_SIZE;
			pt[cur_pid][TLB_VPN(pcb[cur_pid].max_addr)]=ppage_alloc();
		}
		pcb[cur_pid].max_addr+=n;
		pt[cur_pid][TLB_VPN(pcb[cur_pid].max_addr)]=ppage_alloc();
	}
	return ret;
}

pid_t kfork() {
	pid_t pp=sc_pid;
	cur_pid=pid_alloc();
	//kprintf("$$$%d %d\n",sc_pid,cur_pid);
	unsigned long i;
	pcb[cur_pid].pc=pcb[pp].pc;
	for(i=0;i<32;i++)pcb[cur_pid].gpr[i]=pcb[pp].gpr[i];
	pcb[cur_pid].gpr[TF_V0/LONGSIZE]=0;
	kmalloc(pcb[pp].max_addr);
	unsigned int vpn=0;
	volatile unsigned char *s=pcb[pp].gpr[TF_SP/LONGSIZE], *e=pcb[pp].max_addr;
	while(s<e) {
		write_c0_entryhi(pp);
		memcpy(pbuf, s, min(STACK_SIZE,e-s));
		write_c0_entryhi(cur_pid);
		memcpy(s, pbuf, min(STACK_SIZE,e-s));
		s+=min(STACK_SIZE,e-s);
	}
	return cur_pid;
}

void ksleep(unsigned int ms) {

}

void kexec(char* fname) {
	unsigned long start,size;
	if(!strcmp(fname,"init")) {
		start=_binary____userapp_init_bin_start;
		size=_binary____userapp_init_bin_end-_binary____userapp_init_bin_start;
	} else if(!strcmp(fname,"swsh")) {
		start=_binary____userapp_swsh_bin_start;
		size=_binary____userapp_swsh_bin_end-_binary____userapp_swsh_bin_start;
	} else if(!strcmp(fname,"pidspeaker")) {
		start=_binary____userapp_pidspeaker_bin_start;
		size=_binary____userapp_pidspeaker_bin_end-_binary____userapp_pidspeaker_bin_start;
	} else if(!strcmp(fname,"aplusb")) {
		start=_binary____userapp_aplusb_bin_start;
		size=_binary____userapp_aplusb_bin_end-_binary____userapp_aplusb_bin_start;
	} else if(!strcmp(fname,"isort")) {
		start=_binary____userapp_isort_bin_start;
		size=_binary____userapp_isort_bin_end-_binary____userapp_isort_bin_start;
	} else {
		kprintf("No such application: %s\n",fname);
		return -1;
	}
	unsigned long vpn=0;
	
	cur_pid=sc_pid;
	while(pt[sc_pid][vpn]) {
		pt[cur_pid][vpn]=0;
		ppage_free(pt[cur_pid][vpn]);
		vpn++;
		
	}
	pcb[cur_pid].max_addr=0;
	unsigned long sp=kmalloc(STACK_SIZE)+STACK_SIZE;
	ppage	entry=kmalloc(size);
	memcpy(entry, start, size);
	pcb[cur_pid].pc=entry;
	pcb[cur_pid].gpr[TF_SP/LONGSIZE]=sp;
}
