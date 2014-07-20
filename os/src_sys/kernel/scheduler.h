#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_

#include <libk.h>
//#include <thread.h>
 
typedef struct thread_s thread_t;                     

extern thread_t* sched_get_thread_run();
extern void sched_set_thread_run(thread_t *thread);
extern void sched_set_thread_idle(thread_t *thread);
extern thread_t* sched_get_thread_idle();

extern void sched_init (void);              // initialise un scheduler
extern void sched_start (thread_t *thread); // demarre le scheduler sur le thread en argument
extern void sched_create (thread_t *thread);// ajoute un nouveau thread
extern void sched_wakeup (thread_t *thread);// remets un thread dans l'état READY
extern void sched_yield (thread_t *thread); // demande d'ordonnancement
extern void sched_sleep (thread_t *thread); // place le thread dans une liste d'attente
extern void sched_exit (thread_t *thread);  // termine un thread (ici on ne récupère pas la valeur)
extern void sched_clock (void);             // demande de commutation (si possible)
extern void sched_dump (void);              // affiche l'état du scheduler

#endif
