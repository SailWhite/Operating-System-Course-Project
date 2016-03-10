#include <stdio.h>
#include <string.h>
#include <mach/config.h>
#include <asm/config.h>
#include <sys/types.h>
#include <stdarg.h>
#include <stddef.h>
#include <mach/io.h>
#include <mach/config.h>
#include <asm/smp.h>
#include <mach/mail.h>
#include <asm/spinlock.h>
#include <asm/mipsregs.h>
#include <mach/time.h>
#include <asm/cp0regdef.h>
#include <asm/stackframe.h>
#include <asm/mm/tlb.h>
#include <process.h>
#include <sys/syscall.h>
#include <ctype.h>
#include <asm/addrspace.h>

extern char __rodata_end__[];
extern char __data_begin__[], __data_end__[];
extern char __bss_begin__[], __bss_end__[];

unsigned char cpu_stack[MSIM_CPU_NUM][KSTACK_SIZE];

unsigned char	kernel_stack[MSIM_CPU_NUM][KSTACK_SIZE];
unsigned long	kernelsp[MSIM_CPU_NUM];
unsigned long	pt[MAX_PID][MAX_VPN];
unsigned long	free_ppage[MAX_VPN];
unsigned long	first_free_ppage;

pid_t				cur_pid=0;
pid_t				first_free_pid;
struct pcb_entry	pcb[MAX_PID];

unsigned long 	sc_num, sc_ret, sc_args[4];
pid_t			sc_pid;

extern void generic_exception();
extern void tlb_refill();
extern void __reset();
extern void* kmalloc(unsigned int);

void set_trap_handler() {
	uint32_t cp0_status=read_c0_status();
	cp0_status	|=ST_EXL
				|ST_BEV
				|ST_ERL
				|ST_IE
				|ST_IMx(INT_CLOCK)
				|ST_IMx(INT_CLOCK);
				
	cp0_status	^=ST_EXL
				|ST_BEV
				|ST_ERL;
	write_c0_status(cp0_status);
	memcpy((void*)TLB_REFILL_ENTRY, tlb_refill, INT_ENTRY_SIZE);
	memcpy((void*)GENERIC_EXC_ENTRY, generic_exception,INT_ENTRY_SIZE);
	
}

void handle_tlb_refill(struct trapframe *tf) {
	unsigned long	entryhi=read_c0_entryhi();
	unsigned long	vpn=TLB_VPN(entryhi);
	unsigned long	pfn=pt[ENTRYHI_ASID(entryhi)][vpn];
	if(pfn) {
		unsigned long entrylo=( TLB_PFN(pfn, entryhi) | TLB_COHERENT | TLB_VALID | TLB_DIRTY | TLB_GLOBAL)^TLB_GLOBAL;
		write_c0_entrylo0(entrylo);
		write_c0_entrylo1(TLB_ELO0TO1(entrylo));
		tlbwr();
	} else {
		kprintf("Fatal error, invalied page: %x with ASID= %d, rebooting...\n",vpn, ENTRYHI_ASID(entryhi));
		unsigned long* reg=(unsigned long*)tf;
		write_c0_status((read_c0_status()|ST_KSU)^ST_KSU);
		reg[ORD_STATUS]=(read_c0_status()|ST_KSU)^ST_KSU;
		reg[ORD_EPC]=__reset;
	}
	return;
}

#define	RESET_CLOCK			write_c0_compare(read_c0_count()+20*COUNT_PER_MS)

#define next_process\
	cur_pid=pcb[cur_pid].next;\
	while(pcb[cur_pid].status!=ST_READY) {\
		if(pcb[pcb[cur_pid].next].status==ST_STOPED)\
			cur_pid=pid_free(cur_pid,pcb[cur_pid].next);\
		if(pcb[cur_pid].status==ST_BLOCK) {\
			if(pcb[cur_pid].wait && pcb[pcb[cur_pid].wait].status==ST_STOPED) {\
				pcb[cur_pid].wait=0;\
				break;\
			}\
			if(pcb[cur_pid].compare && pcb[cur_pid].compare<read_c0_count()) {\
				pcb[cur_pid].compare=0;\
				break;\
			}\
		}\
		cur_pid=pcb[cur_pid].next;\
	}\
	pcb[cur_pid].status=ST_RUNNING
	
#define pcb_save(pid, i)\
	for(i=0;i<32;i++)pcb[pid].gpr[i]=reg[i];\
	pcb[pid].pc=reg[ORD_EPC]
	
#define pcb_restore(pid, i)\
	reg[ORD_EPC]=pcb[pid].pc;\
	for(i=0;i<32;i++)reg[i]=pcb[pid].gpr[i]

void handle_exception(struct trapframe *tf) {
	uint32_t cause=read_c0_cause();
	unsigned long* reg=(unsigned long*)tf;
	if(EXCCODE(cause)==EC_int) {
		if(cause&CR_IPx(INT_CLOCK)) {
			if(sc_num!=SYS_NONE) {
				write_c0_compare(NULL);
				return;
			}
			//kprintf("--DEBUG-- CPU%d time out: %d\n",cpuid(),cur_pid);
			pcb[cur_pid].status=ST_READY;
			unsigned long i;
			pcb_save(cur_pid,i);
			next_process;
			pcb_restore(cur_pid,i);
			reg[ORD_ENTRYHI]=cur_pid;
			RESET_CLOCK;
		}
	} else if(EXCCODE(cause)==EC_sys) {
		reg[ORD_EPC]+=INST_SIZE;
		unsigned long i;
		switch(reg[ORD_V0]) {
			case SYS_PID:
				reg[ORD_V0]=cur_pid;
				break;
			case SYS_PPID:
				reg[ORD_V0]=pcb[cur_pid].ppid;
				break;
			case SYS_FORK: {
				sc_pid=cur_pid;
				sc_num=SYS_FORK;
				reg[ORD_STATUS]=(read_c0_status()|ST_KSU)^ST_KSU;
				pcb_save(cur_pid, i);
				pcb[cur_pid].status=ST_READY;
				pcb_restore(0, i);
				cur_pid=0;
				break;
			}
			case SYS_EXEC:
				sc_pid=cur_pid;
				sc_num=SYS_EXEC;
				sc_args[0]=reg[ORD_A0];
				reg[ORD_STATUS]=(read_c0_status()|ST_KSU)^ST_KSU;
				pcb_save(cur_pid, i);
				pcb[cur_pid].status=ST_READY;
				pcb_restore(0, i);
				cur_pid=0;
				break;
			case SYS_SLEEP:
				if(reg[ORD_A0]==0)break;
				pcb[cur_pid].compare=read_c0_count()+reg[ORD_A0]*MS_PER_SEC*COUNT_PER_MS;
				pcb[cur_pid].status=ST_BLOCK;
				pcb_save(cur_pid,i);
				next_process;
				pcb_restore(cur_pid,i);
				reg[ORD_ENTRYHI]=cur_pid;
				RESET_CLOCK;
				break;
			case SYS_WAITPID:
				pcb[cur_pid].wait=reg[ORD_A0];
				pcb[cur_pid].status=ST_BLOCK;
				pcb_save(cur_pid,i);
				next_process;
				pcb_restore(cur_pid,i);
				reg[ORD_ENTRYHI]=cur_pid;
				RESET_CLOCK;
				break;
			case SYS_MLC:
				reg[ORD_V0]=kmalloc(reg[ORD_A0]);
				break;
			case SYS_PUTS:
				sc_pid=cur_pid;
				sc_num=SYS_PUTS;
				sc_args[0]=reg[ORD_A0];
				reg[ORD_STATUS]=(read_c0_status()|ST_KSU)^ST_KSU;
				pcb_save(cur_pid, i);
				pcb[cur_pid].status=ST_READY;
				pcb_restore(0, i);
				cur_pid=0;
				break;
			case SYS_KNL:
				reg[ORD_STATUS]=(read_c0_status()|ST_KSU)^ST_KSU;
				reg[ORD_V0]=0;
				break;
			case SYS_USR:
				pcb_save(0, i);
				pcb[cur_pid].status=ST_RUNNING;
				pcb_restore(cur_pid, i);
				reg[ORD_ENTRYHI]=cur_pid;
				reg[ORD_STATUS]=(read_c0_status()|ST_KSU)^KSU_SUPERVISOR;
				write_c0_compare(read_c0_count()+TIME_SLICE*COUNT_PER_MS);
				break;
			case SYS_EXIT: {
				kprintf("--DEBUG-- pid %d exited with %d!\n",cur_pid,reg[ORD_A0]);
				pcb[cur_pid].status=ST_STOPED;
				unsigned long max_page=TLB_VPN(pcb[cur_pid].max_addr);
				for(i=0;i<max_page;i++) {
					ppage_free(pt[cur_pid][i]);
					pt[cur_pid][i]=0;
				}
				next_process;
				reg[ORD_ENTRYHI]=cur_pid;
				pcb_restore(cur_pid,i);
				break;
			}
			case SYS_GETS: {
				sc_pid=cur_pid;
				sc_num=SYS_GETS;
				sc_args[0]=reg[ORD_A0];
				sc_args[1]=reg[ORD_A1];
				reg[ORD_STATUS]=(read_c0_status()|ST_KSU)^ST_KSU;
				pcb_save(cur_pid, i);
				pcb[cur_pid].status=ST_READY;
				pcb_restore(0, i);
				cur_pid=0;
				break;
			}
		}
	} else {
		kputs("Fatal error, unkown exception, rebooting...\n");
		write_c0_status((read_c0_status()|ST_KSU)^ST_KSU);
		reg[ORD_STATUS]=(read_c0_status()|ST_KSU)^ST_KSU;
		reg[ORD_EPC]=__reset;
	}
	return;
}

void tlb_init() {
	int index;
	write_c0_entrylo0((DEFAULT_PHY_ADDR|TLB_COHERENT | TLB_VALID | TLB_DIRTY | TLB_GLOBAL)^TLB_GLOBAL);
	write_c0_entrylo1((TLB_ELO0TO1(DEFAULT_PHY_ADDR)|TLB_COHERENT | TLB_VALID | TLB_DIRTY | TLB_GLOBAL)^TLB_GLOBAL);
	for(index=0;index<TLB_SIZE;index++) {
		write_c0_index(index);
		write_c0_entryhi(IVLD_ASID|(index<<13));
		tlbwi();
	}
	write_c0_entryhi(0x0);
}

void slave_main(void)
{
	send(MSIM_MASTER_ID, 0, 4);
	set_trap_handler();
	tlb_init();
	kprintf("--DEBUG-- Hello world from CPU #%d!\n", cpuid());
	for (;;);
}

void pcb_init() {
	cur_pid=0;
	pcb[0].ppid=-1;
	pcb[0].status=ST_RUNNING;
	pcb[0].max_addr=USEG_BEGIN;
	pcb[0].next=0;
	pcb[0].wait=0;
	pcb[0].compare=0;
	first_free_ppage=0;
	first_free_pid=1;
	unsigned long i;
	for(i=1;i<=MAX_PID;i++)pcb[i].next=i+1;
	for(i=0;i<1<<10;i++)free_ppage[i]=i+1;
}

void main() {
	kputs("--DEBUG-- Hello world from kernel!\n");
	
	unsigned long i;
	kernelsp[0]=(unsigned long)kernel_stack[1];
	for(i=1;i<MSIM_CPU_NUM;i++) {
		send(MSIM_MASTER_ID, 0, 0);
		kernelsp[i]=(unsigned long)kernel_stack[i+1];
		send(i, 1, (uint32_t)cpu_stack[i+1]);
		send(i, 0, (uint32_t)slave_main);
	}
	
	cur_pid=pid_alloc();
	write_c0_entryhi(cur_pid);
	sc_num=SYS_EXEC;
	sc_args[0]="init";
	sc_pid=cur_pid;
	
	for(;;) {
		write_c0_compare(0);
		switch(sc_num) {
			case SYS_PUTS:
				kputs(sc_args[0]);
				sc_ret=0;
				break;
			case SYS_FORK:
				sc_ret=kfork();
				break;
			case SYS_EXEC:
				sc_ret=kexec(sc_args[0]);
				break;
			case SYS_GETS:
				sc_ret=kgets(sc_args[0], sc_args[1]);
				break;
		}
		pcb[sc_pid].gpr[ORD_V0]=sc_ret;
		cur_pid=sc_pid;
		next_process;
		sc_num=SYS_NONE;
		__to_user();
	}
}

void init(void) {
	memcpy(__data_begin__, __rodata_end__, __data_end__ - __data_begin__);
	memset(__bss_begin__, 0, __bss_end__ - __bss_begin__);
	mach_init();
	set_trap_handler();
	tlb_init();
	pcb_init();
	main();
}
