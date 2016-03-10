#ifndef ASM_CONFIG_H
#define ASM_CONFIG_H

#define VTRC2PHY(addr) (addr-0x80000000)
#define PHY2VTRC(addr) (addr+0x80000000)
#define VTRE2PHY(addr) (addr-0xa0000000)
#define PHY2VTRE(addr) (addr+0xa0000000)
#define VTRE2VTRC(addr) (addr-0x20000000)
#define VTRC2VTRE(addr) (addr+0x20000000)

#define DEV_LP 19
#endif