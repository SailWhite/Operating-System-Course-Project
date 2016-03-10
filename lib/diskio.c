#include <stdio.h>
#include <string.h>
#include <mach/config.h>
#include <asm/config.h>

char DMAbuf[SECTOR_SIZE];
volatile unsigned long *DMA = DMA_ADDR;

void readdisk(size_t sec_num, ssize_t offset, void *buf, size_t len) {
	while(len) {
		SET_DMA(DMA, DMAbuf, sec_num, DMA_READ);
		while(DMA_ISREAD(DMA));
		int slen=(len+offset<SECTOR_SIZE)?len:(SECTOR_SIZE-offset);
		memcpy(buf, DMAbuf+offset, slen);
		buf+=slen;
		sec_num++;
		len-=slen;
		offset=0;
	}
}


void writedisk(size_t sec_num, ssize_t offset, const void *buf, size_t len) {
	while(len) {
		readdisk(sec_num,0,NULL,0);
		int slen=(len+offset<SECTOR_SIZE)?len:(SECTOR_SIZE-offset);
		memcpy(DMAbuf+offset, buf, slen);
		SET_DMA(DMA, DMAbuf, sec_num, DMA_WRITE);
		while(DMA_ISWRITE(DMA));
		buf+=slen;
		sec_num++;
		len-=slen;
		offset=0;
	}
}
