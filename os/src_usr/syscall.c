#include <syscall.h>
#include <stdint.h>
#include <hal_cpu.h>

int write (int fd, const void *buf, size_t size)                SYSCALL(WRITE)
int read (int fd, const void *buf, size_t size)                 SYSCALL(READ)
int ioctl (int fd, int request, void *argp)                     SYSCALL(IOCTL)

unsigned cpu_id (void)                                          SYSCALL(CPU_ID)
unsigned cpu_tsc (void)                                         SYSCALL(CPU_TSC)

int pthread_create (pthread_t *thread, const pthread_attr_t *attr,
                    void *(*start_routine)(void*), void *arg)   SYSCALL(PTHREAD_CREATE)
void pthread_exit (void * value_ptr)                            SYSCALL_VOID(PTHREAD_EXIT)
int pthread_join (pthread_t thread, void **value_ptr)           SYSCALL(PTHREAD_JOIN)
int pthread_yield (void)                                        SYSCALL(PTHREAD_YIELD)

int pthread_mutex_lock (pthread_mutex_t *mutex)                 SYSCALL(PTHREAD_MUTEX_LOCK)
int pthread_mutex_trylock (pthread_mutex_t *mutex)              SYSCALL(PTHREAD_MUTEX_TRYLOCK)
int pthread_mutex_unlock (pthread_mutex_t *mutex)               SYSCALL(PTHREAD_MUTEX_UNLOCK)

unsigned * errno_location (void)                                SYSCALL(ERRNO_LOCATION)
