#include <hal_arch.h>
#include <stdint.h>
#include <syscall.h>
#include <error.h>
#include <libk.h>

int sys_write (int fd, const void *buf, size_t size)
{
    MESSG(STANDARD,"fd %d buf %p size %d", fd, buf, size);
    dev_request_t req, *req1 = &req;
    switch (fd) {case DEV_IO: case DEV_ERR: case DEV_LOG:
        req.src = (void *)buf;
        req.size = (uint32_t)size;
        dev_tty(fd)->op.generic.write(dev_tty(fd),req1);
        return req.err;
    }
    // errno = ENOTTY;
    return 0;
}

int sys_read  (int fd, const void *buf, size_t size)
{
    return SUCCESS;
}

// READ / WRITE are used for TTY
// IOCTL is used for DMA, BD, FB
int sys_ioctl  (int fd, int request, void *arg)
{
/*    unsigned * tab = arg;
    switch (fd) {

        case DEV_BD:

            switch (request) {
                case DEV_BD_REQ_READ: {
                    dev_request_t req={ .src=(void *)tab[DEV_BD_ARG_LBA], 
                                        .dst=(void *)tab[DEV_BD_ARG_MEM], 
                                        .size=tab[DEV_BD_ARG_SIZE]};  
                    return (unsigned)dev_bd(0)->op.generic.read(dev_bd(0),&req);
                }
                case DEV_BD_REQ_WRITE: {
                    dev_request_t req={ .src=(void *)tab[DEV_BD_ARG_MEM],
                                        .dst=(void *)tab[DEV_BD_ARG_LBA],
                                        .size=tab[DEV_BD_ARG_SIZE]};
                    return (unsigned)dev_bd(0)->op.generic.write(dev_bd(0),&req);
                }
            }
        break;

		default:

			MESSG(STANDARD,"not yet implemented ....");
			break;
    }
*/
    return 0;
}
