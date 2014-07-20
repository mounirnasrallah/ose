#ifndef _DRIVER_TTY_H_
#define _DRIVER_TTY_H_
#include <hal_arch.h>

// --------------------------------------------- Effective Architecture Dimension

#undef  ARCH_TTY_NR
#define ARCH_TTY_NR          4

// --------------------------------------------- Only called by drivers or arch_init

extern void tty_init  (device_t *dev, void *base, char *name);

#endif
