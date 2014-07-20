#ifndef _DRIVER_CPU_H_
#define _DRIVER_CPU_H_
#include <segmentation.h>
#include <hal_cpu.h>

// --------------------------------------------- Effective Architecture Dimension

#undef  ARCH_CPU_NR
#define ARCH_CPU_NR          4
#undef  ARCH_CPU_MAX_IRQ_NR  
#define ARCH_CPU_MAX_IRQ_NR  1

// --------------------------------------------- Only called by drivers or arch_init

device_t * dev_cpu   (unsigned id);
extern void cpu_init  (device_t *dev, void *base, char *name);
extern void cpu_bind(device_t *cpu, device_t *dev, unsigned irq);

#endif
