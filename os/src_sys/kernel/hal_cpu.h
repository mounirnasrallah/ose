#ifndef  _HAL_CPU_H_
#define  _HAL_CPU_H_

// ----------------------------------------------------------- cpu context
#define CONTEXT_SIZE _TO_BE_DEFINED_       // context table size
extern void cpu_context_init               (unsigned ctx[], unsigned mode_usr, unsigned stack_ptr,
                                            unsigned entry_func, unsigned exit_func,
                                            unsigned arg1);
extern unsigned cpu_context_save           (unsigned *ctx);
extern void cpu_context_restore            (unsigned *ctx, unsigned val);

// ----------------------------------------------------------- Time & identity
#define CPUID          cpu_get_id()
#define CPUTIME        cpu_time_stamp()
static inline unsigned cpu_get_id          (void);
static inline unsigned cpu_time_stamp      (void);

// ----------------------------------------------------------- IRQ register/enable/disable/restore
static inline int  cpu_get_highest_irq     (void); // return irq number or -1 if none irq is up
static inline void cpu_disable_single_irq  (unsigned irq_num, unsigned * old);
static inline void cpu_enable_single_irq   (unsigned irq_num, unsigned * old);
static inline void cpu_disable_all_irq     (unsigned * old);
static inline void cpu_enable_all_irq      (unsigned * old);
static inline void cpu_restore_irq         (unsigned old);

// ----------------------------------------------------------- Spinlock trylock/unlock/init
static inline void cpu_spin_init           (unsigned * lock);
static inline void cpu_spin_lock           (unsigned * lock);
static inline unsigned cpu_spin_trylock    (unsigned * lock);
static inline void cpu_spin_unlock         (unsigned * lock);
static inline void cpu_spin_destroy        (unsigned * lock);

// ----------------------------------------------------------- Cache operations
#define CACHE_LINE_SIZE _TO_BE_DEFINED_
static inline void cpu_invalid_dcache_line (void *ptr);

#include <hal_cpu_code.h>

#endif
