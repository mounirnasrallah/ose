#ifndef _HAL_ARCH_H_
#define _HAL_ARCH_H_

#include <stdint.h>
#include <klist.h>

// ------------------------------------------------------------------------------
// Definition of spinlock
// ------------------------------------------------------------------------------

typedef unsigned spinlock_t;

// ------------------------------------------------------------------------------
// Architecture dimension
// ------------------------------------------------------------------------------

#define ARCH_CPU_NR             UNDEFINED
#define ARCH_TTY_NR             UNDEFINED
#define ARCH_TIMER_NR           UNDEFINED
#define ARCH_DMA_NR             UNDEFINED
#define ARCH_FB_NR              UNDEFINED
#define ARCH_BD_NR              UNDEFINED
#define ARCH_ICU_NR             UNDEFINED
#define ARCH_ICU_MAX_IRQ_NR     UNDEFINED
#define ARCH_CPU_MAX_IRQ_NR     UNDEFINED
#define ARCH_FB_MAX_X           UNDEFINED
#define ARCH_FB_MAX_Y           UNDEFINED

// ------------------------------------------------------------------------------
// architecture API
// ------------------------------------------------------------------------------

// ----------------------------------------- minimalist kernel console access 

static int  kgetc  (void);
static void kputc  (int c);

static inline void spin_init         (spinlock_t *lock);
static inline void spin_destroy      (spinlock_t *lock);
static inline int  spin_trylock      (spinlock_t *lock);
static inline void spin_lock         (spinlock_t *lock);
static inline void spin_unlock       (spinlock_t *lock);
static inline void spin_lock_noirq   (spinlock_t *lock, unsigned * status);
static inline void spin_unlock_noirq (spinlock_t *lock, unsigned status);

// ----------------------------------------- initiatize the whole architecture

extern void arch_init(void);

#include <hal_arch_code.h>

#endif
