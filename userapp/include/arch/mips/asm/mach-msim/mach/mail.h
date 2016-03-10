#ifndef ARCH_MAIL_H
#define ARCH_MAIL_H
#include<sys/types.h>
/*void send(unsigned long cid, uint64_t);
void send2(unsigned long cid, uint32_t content0, uint32_t content1);
uint64_t receive(unsigned long cid);
uint32_t receive0(unsigned long cid);
uint32_t receive1(unsigned long cid);*/
void send(unsigned long cid, int mid, uint32_t content);

uint32_t receive(unsigned long cid,int mid);
#endif