#include <config.h>
#include <klist.h>
#include <libk.h>

static device_t tty_tbl   [CONFIG_TTY_NR];
device_t * dev_tty   (unsigned id) {return &(tty_tbl  [id % ARCH_TTY_NR]);}

// -------------------------------------------------------------------------
// DEVICE REGISTER MAP
// -------------------------------------------------------------------------

enum SoclibTTYRegisters {
    WRITE_REG,
    STATUS_REG,
    READ_REG,
};

// -------------------------------------------------------------------------
// DRIVER OPERATION
// -------------------------------------------------------------------------

static ssize_t tty_read(device_t *dev, dev_request_t * req)
{
    volatile uint32_t *base = dev->base;
    char *dst = req->dst;
    while (base[STATUS_REG] == 0);
    *dst = base[READ_REG];
    return 1;
}

static ssize_t tty_write(device_t *dev, dev_request_t * req)
{
    int c;
    volatile uint32_t *base = dev->base;
    char *src = req->src;
    unsigned size = req->size;
    while ((size--) && (c=*(src++))) {base[WRITE_REG] = c;}
    return 1;
}

static ssize_t tty_params(device_t *dev, dev_params_t * params)
{
    MESSG(DEBUG,"%s does not have parameters\n", dev->name);
    return 0;
}

static void tty_irq_handler(device_t *dev)
{
    volatile uint32_t *base = dev->base;
    char c = base[READ_REG];
    base[WRITE_REG] = c;    // lookback
}

// -------------------------------------------------------------------------
// DEVICE INITIALIZATION                                               
// -------------------------------------------------------------------------

void tty_init(device_t *dev, void *base, char *name)
{
    spin_init(&dev->lock);
    dev->base                   = base;
    dev->name                   = name;
    dev->irq                    = -1; // not bound yet
    dev->type                   = DEV_GENERIC;
    dev->op.generic.read        = &tty_read;
    dev->op.generic.write       = &tty_write;
    dev->op.generic.set_params  = &tty_params;
    dev->op.generic.set_params  = &tty_params;
    dev->action.dev             = dev;
    dev->action.irq_handler     = &tty_irq_handler;
    dev->data                   = NULL;
}
