#include <mutex.h>
#include <scheduler.h>
#include <thread.h>

//#define THREAD schedroot[CPUID].run //MISBAH
int mutex_init (mutex_t *mutex)
{
    spin_init(&(mutex->lock));                          // initialise le verrou
    mutex->state = 0;                                   // la valeur initiale du lock est libre
    list_root_init( &(mutex->wait));                    // et aucun thread n'est en attente
    return 0;
}

int mutex_try_lock (mutex_t *mutex)
{
    spin_lock(&(mutex->lock));                          // prendre le verrou du mutex
    if (mutex->state == 0) {                            // le mutex est libre
        mutex->state = 1;                               // prise du mutex
        spin_unlock(&(mutex->lock));                    // lacher le verrou du mutex
		  return 0;													  // retourne OK, verrou dispo
    } else {
        spin_unlock(&(mutex->lock));                    // lacher le verrou du mutex
        return 1;													  // retourne verrou occupe
    }
}

int mutex_lock (mutex_t *mutex)
{
    spin_lock(&(mutex->lock));                          // prendre le verrou du mutex
    if (mutex->state == 0) {                            // le mutex est libre
        mutex->state = 1;                               // prise du mutex
        spin_unlock(&(mutex->lock));                    // lacher le verrou du mutex
    } else {
		  spin_lock(thread_get_lock( sched_get_thread_run())); // PRENDRE LE VERROU DU THREAD
        list_add_thread_last(&(mutex->wait), sched_get_thread_run()); // mise en fin de la liste d'attente

        spin_unlock(&(mutex->lock));                    // lacher le verrou du mutex
        sched_sleep(sched_get_thread_run());            // demande d'endormissement du thread
    }
    return 0;
}

int mutex_unlock (mutex_t *mutex)
{
    spin_lock(&(mutex->lock));                          // prendre le verrou du mutex
    if (list_isempty(&(mutex->wait))) {                 // aucun thread n'est en attente
        mutex->state = 0;                               // liberation du mutex
        spin_unlock(&(mutex->lock));                    // lacher le verrou du mutex
    } else {
        list_t *el = list_first(&(mutex->wait));        // pointeur sur le 1er thread en attente
        //thread_t *thread = list_item(el,thread_t,list); // obtention du pointeur sur le thread
        thread_t *thread = list_get_thread_element(el);   // obtention du pointeur sur le thread
		  //spin_lock(&(thread->lock));                     // PRENDRE LE VERROU DU THREAD
		  spin_lock(thread_get_lock( thread )) ;                    // PRENDRE LE VERROU DU THREAD

        list_unlink(el);                                // detachage de ce thread
        spin_unlock(&(mutex->lock));                    // lacher le verrou du mutex
        sched_wakeup(thread);                           // demande de réveil du thread
    }
    return 0;
}
