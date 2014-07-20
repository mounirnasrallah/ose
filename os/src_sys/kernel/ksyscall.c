#include <libk.h>
#define __KERNEL__
#include <syscall.h>

int sys_undefined( int a0, int a1, int a2, int a3, int service)
{
    MESSG(STANDARD,"Unused syscall n°%d", service);
    return service;
}

void * syscall_vector[SYSCALL_NR] =
{
    [WRITE                         ] = &sys_write,
    [READ                          ] = &sys_read,
    [IOCTL                         ] = &sys_ioctl,
    [CPU_ID                        ] = &sys_cpu_id,
    [CPU_TSC                       ] = &sys_cpu_tsc,
    [PTHREAD_YIELD                 ] = &sys_pthread_yield,
    [PTHREAD_CREATE                ] = &sys_pthread_create,
    [PTHREAD_EXIT                  ] = &sys_pthread_exit,
    [PTHREAD_JOIN                  ] = &sys_pthread_join,
    [PTHREAD_MUTEX_LOCK            ] = &sys_pthread_mutex_lock,
    [PTHREAD_MUTEX_TRYLOCK         ] = &sys_pthread_mutex_trylock,
    [PTHREAD_MUTEX_UNLOCK          ] = &sys_pthread_mutex_unlock,
    [ERRNO_LOCATION                ] = &sys_errno_location,
    [SYSCALL_UNDEF ... SYSCALL_NR-1] = &sys_undefined
};
