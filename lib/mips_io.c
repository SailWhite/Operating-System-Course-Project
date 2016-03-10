#include<asm/io.h>
void write8(unsigned long addr, uint8_t data)
{
	*(volatile uint8_t *)PHY2VTRE(addr) = data;
}

void write32(unsigned long addr, uint32_t data)
{
	*(volatile uint32_t *)PHY2VTRE(addr) = data;
}

uint32_t read32(unsigned long addr) {
	return *(volatile uint32_t *)PHY2VTRE(addr);
}

void write64(unsigned long addr, uint64_t data) {
	*(volatile uint64_t *)PHY2VTRE(addr) = data;
}

uint64_t read64(unsigned long addr) {
	return *(volatile uint64_t *)PHY2VTRE(addr);
}
