#include <barrier.h>
#include <scheduler.h>
#include <thread.h>

int barrier_init (barrier_t *barrier, unsigned init)        // init est le nombre de th attendu
{
    spin_init(&(barrier->lock));                            // initialise le verrou
    barrier->init = init;                                   // init thread attendu
    barrier->counter = 0;                                   // aucun arrivé encore
    list_root_init( &(barrier->wait));                      // liste vide
    return 0;
}

int barrier_wait (barrier_t *barrier)                      // mise en attente ou reveil de tous
{
    spin_lock(&(barrier->lock));                            // prendre le verrou de la barriere
    if (++barrier->counter == barrier->init) {
        barrier->counter = 0;              
        list_foreach(&(barrier->wait),el) {
            thread_t *thread = list_get_thread_element(el);

				//list_item(el,thread_t,list); // obtention du pointeur sur le thread
				spin_lock(thread_get_lock( thread )) ;
            //spin_lock(&(thread->lock));                     // PRENDRE LE VERROU DU THREAD
            list_unlink(el);                                // detachage de ce thread
            sched_wakeup(thread);                           // demande de réveil du thread
        }
        spin_unlock(&(barrier->lock));                      // lacher le verrou du barrier
    } else {
		  spin_lock(thread_get_lock(sched_get_thread_run())); // PRENDRE LE VERROU DU THREAD        		  
        list_add_thread_last(&(barrier->wait), sched_get_thread_run());// mise en fin de la liste d'attente
		  spin_unlock(&(barrier->lock)); 
		  spin_unlock(thread_get_lock(sched_get_thread_run()));// lacher le verrou de la barriere
		  
		   sched_sleep(sched_get_thread_run());                 // demande d'endormissement du thread
    }
    return 0;
}
