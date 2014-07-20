#ifndef _DRIVER_BD_H_
#define _DRIVER_BD_H_
#include <hal_arch.h>

// --------------------------------------------- Effective Architecture Dimension

#undef  ARCH_BD_NR
#define ARCH_BD_NR        4

//#undef CONFIG_BD_NR
//#define CONFIG_BD_NR 4

// --------------------------------------------- Only called by drivers or arch_init

extern void bd_init(device_t *dev, void *base, char *name);

#endif
