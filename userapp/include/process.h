#ifndef PROCESS_H
#define PROCESS_H

#define pid_t unsigned int

#define ST_READY	0
#define	ST_BLOCK	1
#define ST_RUNNING	2
#define	ST_STOPED	3
#define VPAGE_SIZE	0x200000

#define TIME_SLICE	20
#define MAX_PID		255

#define pt_item	unsigned long
#define ppage	unsigned long*

struct pcb_entry {
	pid_t			ppid;
	pid_t			wait;
	unsigned long	pc;
	int				status;
	unsigned long	max_addr;
	unsigned long 	gpr[32];
	pid_t			next;
};

#endif
