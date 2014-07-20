#include <config.h>
#include <scheduler.h>
#include <libk.h>

// -------------------------------------------------------------------------
// DEVICE REGISTER MAP
// -------------------------------------------------------------------------

enum SoclibTTYRegisters {
BLOCK_DEVICE_BUFFER = 0,
BLOCK_DEVICE_LBA = 1,
BLOCK_DEVICE_COUNT =2 ,
BLOCK_DEVICE_OP = 3
};
#define BLOCK_SIZE 512

enum SoclibBdOperations{
BLOCK_DEVICE_NOOP, 	//No operation
BLOCK_DEVICE_READ, 	//Transfer from block device to memory
BLOCK_DEVICE_WRITE 	//Transfer from memory to block device
};

static int LBA_WRITE = 0;
static int LBA_READ = 0;

/*
#define BLOCK_DEVICE_BUFFER 0
#define BLOCK_DEVICE_COUNT 2
#define BLOCK_DEVICE_LBA 1
#define BLOCK_DEVICE_OP 3
#define BLOCK_DEVICE_WRITE 2

void bd_send(char *buffer)
{
    unsigned volatile *bd_base = (unsigned *) BD_BASE;
    //while (*buffer)
    bd_base[BLOCK_DEVICE_BUFFER]= (int) buffer;//BLOCK_DEVICE_BUFFER 
    bd_base[BLOCK_DEVICE_COUNT]=1;//BLOCK_DEVICE_COUNT 
    bd_base[BLOCK_DEVICE_LBA]=0;//BLOCK_DEVICE_LBA 
    bd_base[BLOCK_DEVICE_OP]=BLOCK_DEVICE_WRITE;//BLOCK_DEVICE_OP
       
}
*/


// -------------------------------------------------------------------------
// DRIVER OPERATIONS
// -------------------------------------------------------------------------

static ssize_t bd_read(device_t *dev, dev_request_t * req)
{
    volatile uint32_t *bd_base = dev->base;
    char *dst = req->dst;

    bd_base[BLOCK_DEVICE_BUFFER] = (int) dst;//BLOCK_DEVICE_BUFFER 
    bd_base[BLOCK_DEVICE_COUNT] = sizeof(dst)/BLOCK_SIZE;//BLOCK_DEVICE_COUNT 
    bd_base[BLOCK_DEVICE_LBA] = LBA_READ;//BLOCK_DEVICE_LBA 
    bd_base[BLOCK_DEVICE_OP] = BLOCK_DEVICE_READ;//BLOCK_DEVICE_OP

	kprintf("j ai lu src = %s", dst);	

    return 1;
}

static ssize_t bd_write(device_t *dev, dev_request_t * req)
{
    volatile uint32_t *bd_base = dev->base;
    char *src = req->src;
    kprintf("j ecris src = %s", src);	

    bd_base[BLOCK_DEVICE_BUFFER] = (int) src;//BLOCK_DEVICE_BUFFER 
    bd_base[BLOCK_DEVICE_COUNT] = sizeof(src)/BLOCK_SIZE;//BLOCK_DEVICE_COUNT 
    bd_base[BLOCK_DEVICE_LBA] = LBA_WRITE;//BLOCK_DEVICE_LBA 
    bd_base[BLOCK_DEVICE_OP] = BLOCK_DEVICE_WRITE;//BLOCK_DEVICE_OP

	 LBA_WRITE += (sizeof(src)/BLOCK_SIZE);
	
    return 1;
}

static ssize_t bd_params(device_t *dev, dev_params_t * params)
{
    MESSG(DEBUG,"%s does not have parameters\n", dev->name);
    return 0;
}

static void bd_irq_handler(device_t *dev)
{
    /*volatile uint32_t *base = dev->base;
    char c = base[READ_REG];
    base[WRITE_REG] = c;    // lookback*/
}

// -------------------------------------------------------------------------
// DEVICE INITIALIZATION
// -------------------------------------------------------------------------

void bd_init(device_t *dev, void *base, char *name)
{
    spin_init(&dev->lock);
    dev->base                   = base;
    dev->name                   = name;
    dev->irq                    = -1; // not bound yet
    dev->type                   = DEV_GENERIC;
    dev->op.generic.read        = &bd_read;
    dev->op.generic.write       = &bd_write;
    dev->op.generic.set_params  = &bd_params;
    dev->op.generic.set_params  = &bd_params;
    dev->action.dev             = dev;
    dev->action.irq_handler     = &bd_irq_handler;
    dev->data                   = NULL;
}
