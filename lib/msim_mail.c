#include<mach/mail.h>
#include<asm/io.h>
#include<mach/config.h>

void send(unsigned long cid, int mid, uint32_t content) {
	write32(MSIM_ORDER_MAILBOX_BASE+cid*MSIM_ORDER_MAILBOX_SIZE+mid*4, content);
}

uint32_t receive(unsigned long cid,int mid) {
	return read32(MSIM_ORDER_MAILBOX_BASE+cid*MSIM_ORDER_MAILBOX_SIZE+mid*4);
}
