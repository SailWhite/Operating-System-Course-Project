#ifndef MACH_TIME_H
#define MACH_TIME_H
#include<mach/config.h>
#include<sys/types.h>
#include<asm/mipsregs.h>
static inline uint32_t time() {
	return read32(MSIM_TIMER_BASE);
}
/*
static inline void sleep(unsigned int secs) {
	unsigned int alm=time()+secs+1;
	while(time()<alm);
}

static inline uint32_t usleep(unsigned int msecs) {
	unsigned int alm=read_c0_count()+msecs*COUNT_PER_MS;
	while(read_c0_count()<alm);
}*/
#endif
