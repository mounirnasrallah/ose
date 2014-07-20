#ifndef _THREAD_H_
#define _THREAD_H_

#include <libk.h>
#include <scheduler.h>

//typedef struct thread_s thread_t; 

typedef enum {
    ITHREAD,    // Idle thread
    KTHREAD     // Kernel thread
} thread_type_t;

typedef enum {
    CREATE, // état de démarrage attribué au thread lorsqu'il n'a jamais été chargé sur un core.
    READY,  // état dans lequel le thread est en attente du core.
    RUN,    // état en cours d'exécution.
    WAIT,   // état dans lequel le thread est en attente d'une ressource.
    DEAD,   // état de sortie dans lequel le thread peut être détruit.
	 ZOMBIE  // état d'attente de destruction par le thread parent lors du join
} thread_state_t;

extern list_t rope;             // liste des threads existants (quel que soit leur état)

typedef void *thread_fct_t (void *); // Thread function type

extern thread_t * 
 thread_create         (unsigned       thread_type,
                         unsigned            cpuid,
                         thread_fct_t * start,
                         void *         arg);


extern unsigned thread_save (thread_t * thread);
extern void thread_restore (thread_t * thread);
extern void thread_exit (void * exit_value);
extern void thread_yield (void);
extern int  thread_join  (thread_t * thread, void **value_ptr);
extern void         thread_init           (void);

extern void * thread_idle (void * arg);

//Debug functions
extern void thread_dump (char *messg, thread_t * thread);
extern void thread_rope_dump (void);
extern void thread_list_dump (char *messg, list_t *root);

//Gestion de liste chainée sur thread_t
extern thread_t * list_get_thread_element(list_t * el);
extern void list_add_thread_last(list_t * listing, thread_t * thread);

//Gestion des attributs de thread_t structure "cachée"
extern spinlock_t * thread_get_lock(thread_t * thread);

extern void thread_state_set(thread_t * thread, unsigned state);
extern thread_state_t thread_state_get(thread_t * thread);

extern thread_type_t thread_type_get(thread_t * thread);

extern unsigned thread_cpuid_get(thread_t * thread);

extern unsigned* thread_pws_get(thread_t * thread);




#endif
