#include <driver_timer.h>
#include <driver_icu.h>
#include <config.h>
#include <libk.h>

static device_t timer_tbl [CONFIG_TIMER_NR];
device_t * dev_timer (unsigned id) {return &(timer_tbl[id % ARCH_TIMER_NR]);}

// -------------------------------------------------------------------------
// DEVICE REGISTER MAP
// -------------------------------------------------------------------------

#define RUNNING   1
#define IRQENABLE 2

enum SoclibTimerRegisters {
    VALUE,      // counter value
    MODE,       // bit 0 = run, bit 1 = irqenable
    PERIOD,     // periode in cycles
    RESETIRQ,   // any write reset IRQ
};

// -------------------------------------------------------------------------
// DRIVER OPERATIONS
// -------------------------------------------------------------------------

static void timer_set_period(device_t *dev, unsigned period)
{
    volatile uint32_t *base = dev->base;
    base[PERIOD]            = period;
    base[MODE]              = (period) ? (RUNNING|IRQENABLE) : 0;       
}

static unsigned timer_get_value(device_t *dev)
{
    volatile uint32_t *base = dev->base;
    return base[VALUE];
}

static void timer_reset(device_t *dev)
{
    volatile uint32_t *base = dev->base;
    base[RESETIRQ]          = 0;
}

static void timer_irq_handler(device_t *dev)
{
    timer_reset(dev);
    sched_clock();      
}

// -------------------------------------------------------------------------
// DEVICE INITIALIZATION
// -------------------------------------------------------------------------

void timer_init(device_t *dev, void *base, char *name)
{
    spin_init(&dev->lock);
    dev->base                   = base;
    dev->name                   = name;
    dev->irq                    = -1; // not bound yet
    dev->type                   = DEV_GENERIC;
    dev->op.timer.set_period    = &timer_set_period;
    dev->op.timer.get_value     = &timer_get_value;
    dev->op.timer.reset         = &timer_reset;
    dev->action.dev             = dev;
    dev->action.irq_handler     = &timer_irq_handler;
    dev->data                   = NULL;
}
