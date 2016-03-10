#include<mach/io.h>

struct lp lps[NR_LPS];          /* list of line printers */
void msim_lp_write_char(struct lp *lp, unsigned char c)
{
	write8(MSIM_LP_REG(lp, MSIM_LP_OUT), c);
}

int ndevs;                      /* number of devices */
struct device *devs[MAXDEVS];   /* device pointer list */

/* Probably in somewhere else, not necessarily in the same file */
void lp_puts(struct lp *lp, char *s) {
	for (; *s != '\0'; ++s) {
		lp->write_char(lp,*s);
	}
}

void device_register(struct device *dev) {
	devs[ndevs++] = dev;
}

int dev_support_lp(struct device *dev) {
	return dev->type == DEV_LP;
}

void msim_lp_register(unsigned long paddr) {
	struct device *dev = &lps[0].dev;
	dev->phys_addr = paddr;
	dev->sl.state = 0;
	dev->type = DEV_LP;                     /* device type */
	lps[0].write_char = msim_lp_write_char; /* our implementation */
	device_register(dev);                   /* register to device list */
}

void msim_lp_init(unsigned long paddr)
{
	msim_lp_register(paddr);
}

void mach_init(void) {
	ndevs=0;
    msim_lp_init(MSIM_LP_BASE);
}
