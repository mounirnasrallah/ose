#include <driver_cpu.h>
#include <driver_icu.h>
#include <config.h>
#include <libk.h>

static device_t cpu_tbl   [CONFIG_CPU_NR];
device_t * dev_cpu   (unsigned id) {return &(cpu_tbl  [id % CONFIG_CPU_NR]);}

// -------------------------------------------------------------------------
// DRIVERS OPERATIONS
// -------------------------------------------------------------------------

static void cpu_irq_handler (device_t *dev)
{
    irq_action_t **action = dev->data;
    int irq = cpu_get_highest_irq();
    action[irq]->irq_handler(action[irq]->dev);
}

// -------------------------------------------------------------------------
// DEVICE INITIALIZATION
// -------------------------------------------------------------------------

void cpu_init(device_t *dev, void * base, char *name)
{
    spin_init(&dev->lock);
    dev->base                   = base;
    dev->irq                    = -1; // never bound
    dev->name                   = name;
    dev->type                   = DEV_CPU;
    dev->action.dev             = dev;
    dev->action.irq_handler     = &cpu_irq_handler;
    dev->data                   = kmalloc(sizeof(irq_action_t *) * CONFIG_CPU_MAX_IRQ_NR);
}

void cpu_bind(device_t *cpu, device_t *dev, unsigned irq)
{
    irq_action_t **action_tbl   = cpu->data;
    dev->irq                    = irq; // irq of dev is bound to cpu
    action_tbl[irq]             = &(dev->action);
}
