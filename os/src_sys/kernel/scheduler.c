#include <libk.h>
#include <scheduler.h>
#include <thread.h>

// variable globale définissant un tableau de scheduler avec autant de cases que de core
  

typedef struct  schedroot_s {
    spinlock_t lock;    // protection contre les accès concurrents
    thread_t * run;     // thread en cours d'exec
    thread_t * idle;    // thread idle
    list_t dead;        // liste des threads à détruire
    list_t ready;       // liste des threads en attente du core
} schedroot_t;

schedroot_t schedroot[CONFIG_CPU_NR];

thread_t* sched_get_thread_run()
{
	return schedroot[CPUID].run;
}

thread_t * sched_set_run(thread_t *thread)
{
    return schedroot[CPUID].run = thread;
}

void sched_set_thread_run(thread_t *thread)
{
	schedroot[cpu_get_id()].run = thread;
}

void sched_set_thread_idle(thread_t *thread)
{
	schedroot[cpu_get_id()].idle = thread;
}

thread_t* sched_get_thread_idle()
{
	return schedroot[cpu_get_id()].idle;
}
void sched_init (void)
{
    spin_init(&(schedroot[CPUID].lock));
    schedroot[CPUID].run = NULL;
    schedroot[CPUID].idle = NULL;
    list_root_init(&(schedroot[CPUID].dead));
    list_root_init(&(schedroot[CPUID].ready));
}

void sched_dump(void)
{
    for(int c=0; c<CONFIG_CPU_NR; c++) {
        MESSG(VERBOSE,"SCHEDULER %d",c);
        thread_dump("RUN", schedroot[c].run);
        thread_list_dump("READY", &schedroot[c].ready);
    }
    MESSG(VERBOSE,"ROPE");
    thread_rope_dump();
}

void sched_create (thread_t *thread)
{
    spin_lock(&(schedroot[thread_cpuid_get(thread)].lock));
    thread_state_set(thread, CREATE);
    switch (thread_type_get(thread)) {
    case ITHREAD:
        schedroot[thread_cpuid_get(thread)].idle = thread;   
        break;
    case KTHREAD:
        //list_add_last(&(schedroot[thread->cpuid].ready), &(thread->list));
		  list_add_thread_last(&(schedroot[thread_cpuid_get(thread)].ready), thread); 
        break;
    } 
    spin_unlock(&(schedroot[thread_cpuid_get(thread)].lock));	 
}

void sched_start(thread_t *thread)
{
    thread_state_set(thread, RUN);
    schedroot[CPUID].run = thread;
    cpu_context_restore(thread_pws_get(thread),1);
}

static thread_t * elect(void)
{
    thread_t *next;
    if (list_isempty(&(schedroot[CPUID].ready)))
        next = schedroot[CPUID].idle;
    else {
        list_t *el = list_first(&(schedroot[CPUID].ready)); // pointeur du 1er thread en attente
        //next = list_item(el,thread_t,list);                 // obtention du pointeur sur le thread
	     next = list_get_thread_element(el);                 // obtention du pointeur sur le thread
        list_unlink(el);                                    // detachage de ce thread
    }
	thread_state_set(next, RUN);
    return next;
}

// Quand on entre dans commute les verrous du scheduler et du thread sont pris
static void commute(thread_t *current, thread_t *next)
{
    if (next != current) {
        schedroot[CPUID].run = next;
 		spin_unlock(&(schedroot[CPUID].lock));              // on a fini avec le scheduler 
        if (cpu_context_save(thread_pws_get(current)) == 0) {          // le thread courant est sauvé
            spin_unlock(thread_get_lock(current));                  // on a fini avec ce thread
           
            thread_state_set(next, RUN);
            cpu_context_restore(thread_pws_get(next),1);               // on restaure le suivant
        }
    } else {   
        spin_unlock(&(schedroot[CPUID].lock));              // on a fini avec le scheduler 
		  spin_unlock(thread_get_lock(current));                      // on a fini avec ce thread
    }
}


// Quand on entre dans sched_wakeup, sched_yield, sched_sleep et sched_exit
// - le verrou sur le thread est pris
// - le thread est dans l'état précédent WAIT et RUN
// - pour sleep le thread est déjà dans une liste d'attente de ressource

void sched_wakeup (thread_t *thread)
{
	 thread_state_set(thread, READY);   
    spin_lock(&(schedroot[thread_cpuid_get(thread)].lock));
    
	 list_add_thread_last(&(schedroot[thread_cpuid_get(thread)].ready), thread); 
    spin_unlock(&(schedroot[thread_cpuid_get(thread)].lock));
    spin_unlock(thread_get_lock(thread));

/*    unsigned old_SR;
    thread_state_set(thread, READY);  
    spin_lock_noirq(thread_get_lock(sched_get_thread_run()), &old_SR);
    
	 list_add_thread_last(&(schedroot[thread_cpuid_get(thread)].ready), thread); 
    spin_unlock_noirq(thread_get_lock(sched_get_thread_run()), old_SR);
    spin_unlock(thread_get_lock(thread));*/
}

void sched_yield (thread_t *thread)
{    
    thread_state_set(thread, READY);
    spin_lock(&(schedroot[CPUID].lock));
    if (thread_type_get(thread) != ITHREAD)
			list_add_thread_last(&(schedroot[thread_cpuid_get(thread)].ready), thread); 
        //list_add_last(&(schedroot[CPUID].ready), &(thread->list));
    commute(thread, elect());    
}

void sched_sleep (thread_t *thread)
{
    thread_state_set(thread, WAIT);
    spin_lock(&(schedroot[CPUID].lock));
    commute(thread, elect());
    spin_unlock(&(schedroot[CPUID].lock));              // on a fini avec le scheduler 
}

void sched_exit (thread_t *thread)
{
    thread_state_set(thread, DEAD);
    spin_lock(&(schedroot[CPUID].lock));

    list_add_thread_last(&(schedroot[thread_cpuid_get(thread)].dead), thread); 
    commute(thread, elect());
    spin_unlock(&(schedroot[CPUID].lock));              // on a fini avec le scheduler 
}
/*
//Fonction appelée lors d'une interruption du timer
void sched_clock (void)
{    if (thread_get_lock(sched_get_thread_run()) == 0)             
        sched_yield(sched_get_thread_run());           
}
*/

//Fonction appelée lors d'une interruption du timer
void sched_clock(void)
{
    thread_t *thread = sched_get_thread_run();
    thread_state_set(thread, READY);
    spin_lock(&(schedroot[CPUID].lock));
    if (thread_type_get(thread) != ITHREAD)
        list_add_thread_last(&(schedroot[thread_cpuid_get(thread)].ready), thread);
			//list_add_last(&(schedroot[CPUID].ready), &(thread->list));
    commute(thread, elect());	
}
