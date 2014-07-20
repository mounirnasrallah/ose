#ifndef _DRIVER_TIMER_H_
#define _DRIVER_TIMER_H_
#include <hal_arch.h>

// --------------------------------------------- Effective Architecture Dimension

#undef  ARCH_TIMER_NR
#define ARCH_TIMER_NR        4

// --------------------------------------------- Only called by drivers or arch_init

extern void timer_init(device_t *dev, void *base, char *name);

#endif
