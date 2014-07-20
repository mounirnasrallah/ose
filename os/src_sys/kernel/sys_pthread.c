#include <error.h>
#include <thread.h>
#include <mutex.h>
#include <pthread.h>

int sys_pthread_create(pthread_t * pthread, const pthread_attr_t * attr,
		       void *(*start_routine) (void *), void *arg)
{
    thread_t *th = thread_create(KTHREAD, -1, start_routine, arg);
    if (pthread)
	*pthread = (pthread_t) th;
    return SUCCESS;
}

void sys_pthread_exit(void *value_ptr)
{
    thread_exit(value_ptr);
}

int sys_pthread_join(pthread_t pthread, void **value_ptr)
{
    thread_t *th = (thread_t *) pthread;  // ici, il faudrait vérifier que pthread est valide !!!
    return thread_join(th, value_ptr);
}

int sys_pthread_yield(void)
{
    sched_yield(sched_get_thread_run());
    return SUCCESS;
}

int sys_pthread_mutex_lock(pthread_mutex_t * mutex)
{
    return mutex_lock((mutex_t *)mutex);
}

int sys_pthread_mutex_trylock(pthread_mutex_t * mutex)
{
    return mutex_try_lock((mutex_t *)mutex);
}

int sys_pthread_mutex_unlock(pthread_mutex_t * mutex)
{
    return mutex_unlock((mutex_t *)mutex);
}
