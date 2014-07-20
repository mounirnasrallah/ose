#ifndef _DRIVER_ICU_H_
#define _DRIVER_ICU_H_
#include <segmentation.h>
#include <hal_cpu.h>

// --------------------------------------------- Effective Architecture Dimension

#undef  ARCH_ICU_NR
#define ARCH_ICU_NR             1
#undef  ARCH_ICU_MAX_IRQ_NR   
#define ARCH_ICU_MAX_IRQ_NR   7

// --------------------------------------------- Only called by drivers or arch_init

extern void icu_init  (device_t *dev, void *base, char *name);
extern void icu_bind(device_t *icu, device_t *dev, unsigned irq);

#endif
