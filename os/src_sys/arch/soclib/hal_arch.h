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
// Definitions of ALL the drivers
// ------------------------------------------------------------------------------

typedef struct device_s device_t;

// ---------------------------------------------------------------------- DEV_ICU

typedef void     (icu_setclr_mask_t)     (device_t * icu, uint32_t mask);
typedef uint32_t (icu_get_mask_t)        (device_t * icu);
typedef uint32_t (icu_get_highest_irq_t) (device_t * icu);

struct dev_icu_op {
    icu_setclr_mask_t *set_mask;
    icu_setclr_mask_t *clr_mask;
    icu_get_mask_t *get_mask;
    icu_get_highest_irq_t *get_highest_irq;
};

// -------------------------------------------------------------------- DEV_TIMER 

typedef void     (timer_set_period_t) (device_t * timer, uint32_t period);
typedef uint32_t (timer_get_value_t)  (device_t * timer);
typedef void     (timer_reset_t)      (device_t * timer);

struct dev_timer_op {
    timer_set_period_t *set_period;
    timer_get_value_t *get_value;
    timer_reset_t *reset;
};

// ---------------------------------------------------------------------- DEV_CPU

struct dev_cpu_op {
};

// -------------------------------- DEV_GENERIC (tty, dma, block device, fb, ...)

typedef struct dev_request_s {
    void *src;          
    void *dst;
    uint32_t size;
    int err;
    list_t list;
    void *data;
} dev_request_t;

typedef struct dev_params_s {
    uint32_t type;
    uint32_t count32;
    uint32_t speed;
    uint32_t size;
    uint32_t xsize;
    uint32_t ysize;
} dev_params_t;

typedef int32_t (dev_generic_request_t) (device_t * dev, dev_request_t * req);
typedef int32_t (dev_generic_params_t)  (device_t * dev, dev_params_t * params);

struct dev_generic_op {
    dev_generic_request_t * read;
    dev_generic_request_t * write;
    dev_generic_params_t  * set_params;
    dev_generic_params_t  * get_params;
};

// ----------------------------------- At last, union of all drivers of all types

typedef union driver_u {
    struct dev_cpu_op cpu;
    struct dev_icu_op icu;
    struct dev_timer_op timer;
    struct dev_generic_op generic;
} driver_t;

// ------------------------------------------------------------------------------
// Definition of generic device
// ------------------------------------------------------------------------------

// irq_action : interrupt handler for a device

typedef void (irq_handler_t) (device_t *dev); // function type

typedef struct irq_action_s {
    device_t *      dev;            // ptr to the own device
    irq_handler_t * irq_handler;    // ptr to the handler
} irq_action_t;

// Generic device (i.e. tty, dma, bloc device, etc.)   

typedef enum {                      // device types
    DEV_CPU,
    DEV_ICU,
    DEV_TIMER,
    DEV_GENERIC
} dev_type_t;

struct device_s {
    spinlock_t   lock;              // proctect for parallel access
    char *       name;              // useful for debug
    unsigned     index;             // device index (number)
    void *       base;              // base address in physical memory
    unsigned     irq;               // irq line number used for this device
    dev_type_t   type;              // device type
    driver_t     op;                // possible operations (type dependant)
    irq_action_t action;            // irq_handler for this device
    void *       data;              // for driver purpose
};

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

// ----------------------------------------- get device pointer from identifier

extern device_t * dev_icu     (unsigned id);
extern device_t * dev_timer   (unsigned id);
extern device_t * dev_tty     (unsigned id);
extern device_t * dev_dma     (unsigned id);
extern device_t * dev_fb      (unsigned id);
extern device_t * dev_bd      (unsigned id);
extern device_t * dev_cpu     (unsigned id);

#include <hal_arch_code.h>

#endif
