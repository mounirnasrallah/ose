#include <driver_icu.h>
#include <libk.h>

static device_t icu_tbl   [CONFIG_ICU_NR];
device_t * dev_icu   (unsigned id) {return &(icu_tbl  [id % ARCH_ICU_NR]);}

// -------------------------------------------------------------------------
// DRIVER REGISTER MAP
// -------------------------------------------------------------------------

enum SoclibIUCRegisters {
    INPUT_REG,          // IRQ state without MASK
    MASK_REG,           // MASK value
    MASK_SET_REG,       // set the MASK where bits are 1
    MASK_CLEAR_REG,     // clear the MASK where bits are 1
    HIGHEST_IRQ,        // return the highest IRQ index
};

#define INT_LINES_NR    32

// -------------------------------------------------------------------------
// DRIVERS OPERATIONS
// -------------------------------------------------------------------------

static void icu_set_mask (device_t * dev, unsigned mask)
{
    volatile uint32_t *base = dev->base;
    base[MASK_SET_REG] = mask;
}

static void icu_clr_mask (device_t * dev, unsigned mask)
{
    volatile uint32_t *base = dev->base;
    base[MASK_CLEAR_REG] = mask;
}

static unsigned icu_get_mask (device_t * dev)
{
    volatile uint32_t *base = dev->base;
    return base[MASK_REG];
}

static unsigned icu_get_highest_irq (device_t * dev)
{
    volatile uint32_t *base = dev->base;
    return base[HIGHEST_IRQ];
}

static void icu_irq_handler (device_t *dev)
{
    irq_action_t **action = dev->data;
    int irq = icu_get_highest_irq(dev);
    action[irq]->irq_handler(action[irq]->dev);
}

// -------------------------------------------------------------------------
// DEVICE INITIALIZATION
// -------------------------------------------------------------------------

void icu_init(device_t *dev, void * base, char *name)
{
    spin_init(&dev->lock);
    dev->base                   = base;
    dev->name                   = name;
    dev->type                   = DEV_ICU;
    dev->op.icu.set_mask        = &icu_set_mask;
    dev->op.icu.clr_mask        = &icu_clr_mask;
    dev->op.icu.get_mask        = &icu_get_mask;
    dev->op.icu.get_highest_irq = &icu_get_highest_irq;
    dev->action.dev             = dev;
    dev->action.irq_handler     = &icu_irq_handler;
    dev->data                   = kmalloc(sizeof(irq_action_t *) * CONFIG_ICU_MAX_IRQ_NR);
}

void icu_bind(device_t *cpu, device_t *dev, unsigned irq)
{
    irq_action_t **action_tbl   = cpu->data;
    dev->irq                    = irq;
    action_tbl[irq]             = &(dev->action);
}
