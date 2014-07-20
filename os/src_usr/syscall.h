#ifndef _SYSCALL_H_
#define _SYSCALL_H_

//#include "src_usr/libpthread/pthread.h"
#include "pthread.h"
#include <stdint.h>

// -------------------------------------------------------------------------------------------------
// Device identifiers
// -------------------------------------------------------------------------------------------------
enum DEVICES_E {
    // ---------------------------- used by read/write syscall
    DEV_IO   = 1,   // TTY1
    DEV_ERR  = 2,   // TTY1
    DEV_LOG  = 3,   // TTY3
    // ---------------------------- used by ioctl syscall
    DEV_DMA  = 4,   // DMA
    DEV_BD   = 5,   // Bloc Device
    DEV_FB   = 6,   // Frame Buffer
};

// -------------------------------------------------------------------------------------------------
// Requests and Arguments for ioctl syscall
// -------------------------------------------------------------------------------------------------
enum IOCTL_COMMAND_E {
	DEV_FB_REQ_PIXEL,
	
};

// -------------------------------------------------------------------------------------------------
// Syscall indexes
// -------------------------------------------------------------------------------------------------
enum SYSCALL_E {
    WRITE,
    READ,
    IOCTL,
    CPU_ID,                 // cpu identifier
    CPU_TSC,                // cpu Time Stamp Counter
    PTHREAD_CREATE,
    PTHREAD_EXIT,
    PTHREAD_JOIN,
    PTHREAD_YIELD,
    PTHREAD_MUTEX_LOCK,
    PTHREAD_MUTEX_TRYLOCK,
    PTHREAD_MUTEX_UNLOCK,
    ERRNO_LOCATION,         // return &errno in user space of the current thread
    SYSCALL_UNDEF
};

#define SYSCALL_NR  16

#ifdef __KERNEL__
#   define SYS(name) sys_##name
#else
#   define SYS(name) __attribute__((noinline)) name
#endif

// ----------------------------------------------------------------------------------- sys_drivers.c
extern int        SYS(write)                (int fd, const void *buf, size_t size);
extern int        SYS(read)                 (int fd, const void *buf, size_t size);
extern int        SYS(ioctl)                (int fd, int request, void *arg);
// ----------------------------------------------------------------------------------- sys_pthread.c
extern int        SYS(pthread_create)       (pthread_t *pthread, const pthread_attr_t *attr,
                                            void *(*start_routine)(void*), void *arg);
extern void       SYS(pthread_exit)         (void * value_ptr);
extern int        SYS(pthread_join)         (pthread_t thread, void ** value_ptr);
extern int        SYS(pthread_yield)        (void);
extern int        SYS(pthread_mutex_lock)   (pthread_mutex_t *mutex);
extern int        SYS(pthread_mutex_trylock)(pthread_mutex_t *mutex);
extern int        SYS(pthread_mutex_unlock) (pthread_mutex_t *mutex);
// -------------------------------------------------------------------------------------- sys_info.c
extern unsigned * SYS(errno_location)       (void);
extern unsigned   SYS(cpu_id)               (void);
extern unsigned   SYS(cpu_tsc)              (void);


#endif
