#include <asm/io.h>
#include <asm/config.h>
#include <mach/config.h>
#include <mach/io.h>
#include <ctype.h>
/* The following code will be changed after dynamic memory allocation */
extern int ndevs;                      /* number of devices */
extern struct device *devs[MAXDEVS];   /* device pointer list */

void kputs(char *s)
{
	unsigned int i;
	struct lp *lp;
	for (i = 0; i < ndevs; ++i) {
		if (dev_support_lp(devs[i])) {
			lock(&(devs[i]->sl));
			lp = (struct lp *)devs[i];
			lp_puts(lp, s);
			unlock(&(devs[i]->sl));
		}
	}
}

unsigned long kgets(char* s, unsigned long size) {
	int len=0;
	char last_key='\0';
	read32(MSIM_KB_BASE);
	while(last_key!='\n' && len<size-1) {
		last_key=read32(MSIM_KB_BASE);
		while(!isprint(last_key)) {
			last_key=read32(MSIM_KB_BASE);
			if(last_key==0x7f && len>0) {
				kprintf("%c %c",last_key,last_key);
				len--;
			}
		}
		s[len++]=last_key;
		kprintf("%c",last_key);
	}
	s[len]='\0';
	return len;
}
