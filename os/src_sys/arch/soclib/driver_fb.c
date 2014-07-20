#include <config.h>
#include <scheduler.h>
#include <libk.h>

// -------------------------------------------------------------------------
// DEVICE REGISTER MAP
// -------------------------------------------------------------------------

/*enum SoclibTTYRegisters {
    WRITE_REG,
    STATUS_REG,
    READ_REG,
};*/

// -------------------------------------------------------------------------
// DRIVER OPERATIONS
// -------------------------------------------------------------------------

static ssize_t fb_read(device_t *dev, dev_request_t * req)
{
    /*volatile uint32_t *base = dev->base;
    char *dst = req->dst;
    while (base[STATUS_REG] == 0);
    *dst = base[READ_REG];*/
    return 1;
}

static ssize_t fb_write(device_t *dev, dev_request_t * req)
{
    /*int c;
    volatile uint32_t *base = dev->base;
    char *src = req->src;
    unsigned size = req->size;
    while ((size--) && (c=*(src++))) {base[WRITE_REG] = c;}*/
    return 1;
}

static ssize_t fb_params(device_t *dev, dev_params_t * params)
{
    MESSG(DEBUG,"%s does not have parameters\n", dev->name);
    return 0;
}

static void fb_irq_handler(device_t *dev)
{
    /*volatile uint32_t *base = dev->base;
    char c = base[READ_REG];
    base[WRITE_REG] = c;    // lookback*/
}

// -------------------------------------------------------------------------
// DEVICE INITIALIZATION
// -------------------------------------------------------------------------

void fb_init(device_t *dev, void *base, char *name)
{
    spin_init(&dev->lock);
    dev->base                   = base;
    dev->name                   = name;
    dev->irq                    = -1; // not bound yet
    dev->type                   = DEV_GENERIC;
    dev->op.generic.read        = &fb_read;
    dev->op.generic.write       = &fb_write;
    dev->op.generic.set_params  = &fb_params;
    dev->op.generic.set_params  = &fb_params;
    dev->action.dev             = dev;
    dev->action.irq_handler     = &fb_irq_handler;
    dev->data                   = NULL;
}
