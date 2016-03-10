#ifndef MACH_CONFIG_H
#define MACH_CONFIG_H

#define KSTACK_SIZE     32768
#define PRT_ADDR (volatile unsigned char*)0xbf000010
#define SECTOR_SIZE 0x200
#define DMA_ADDR (volatile unsigned long*)0xbf000020
#define DMA_READ 1
#define DMA_WRITE 2
#define SET_DMA(DMA, ADDR, SEC_NUM, DMA_OP)\
			DMA[0] = (unsigned long)VTRC2PHY(ADDR);\
			DMA[1] = SEC_NUM;\
			DMA[2] = DMA_OP
#define DMA_ISREAD(DMA) (DMA[2] & DMA_READ)
#define DMA_ISWRITE(DMA) (DMA[2] & DMA_WRITE)
 
#define NR_LPS 2
#define MAXDEVS 2
#define MSIM_LP_BASE 0x1f000000
#define MSIM_LP_IN 0
#define MSIM_LP_OUT 0x10

#define MSIM_ORDER_BASE 0x1f000030
#define MSIM_ORDER_BASE_VA 0xbf000030
#define MSIM_ORDER_MAILBOX_BASE 0x1f000040
#define MSIM_ORDER_MAILBOX_BASE_VA 0xbf000040
#define MSIM_ORDER_MAILBOX_SIZE 0x10
#define MSIM_MASTER_ID 0
#define MSIM_CPU_NUM 4

#define MSIM_TIMER_BASE 0x1f000080
#define MSIM_TIMER_BASE_VA 0xbf000080
#define COUNT_PER_MS 5392

#define MSIM_KB_BASE 0x1f000000
#define MSIM_KB_BASE_VA 0xbf000000
#endif
