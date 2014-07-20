#ifndef _P_THREAD_H_
#define _P_THREAD_H_

typedef unsigned long pthread_t;
/*
typedef enum {
    PITHREAD,    // Idle thread
    PKTHREAD     // Kernel thread
} pthread_type_t;

typedef enum {
    PCREATE, // état de démarrage attribué au thread lorsqu'il n'a jamais été chargé sur un core.
    PREADY,  // état dans lequel le thread est en attente du core.
    PRUN,    // état en cours d'exécution.
    PWAIT,   // état dans lequel le thread est en attente d'une ressource.
    PDEAD    // état de sortie dans lequel le thread peut être détruit.
} pthread_state_t;

typedef struct pthread_s {
    spinlock_t lock;            // protection contre les accès concurrents
    thread_state_t state;       // CREATE, READY, ...
    thread_type_t type;         // ITHREAD, KTHREAD, ...
    unsigned cpuid;             // numéro du core affecté au thread
    list_t list;                // utilisé pour le chainage des threads dans les listes d'attente
    list_t rope;                // chaine tous les threads quelque soit leur état
    unsigned pws[CONTEXT_SIZE]; // context
} pthread_t;
*/

//source : http://refspecs.linuxbase.org/LSB_3.1.1/LSB-Core-generic/LSB-Core-generic/libpthread-ddefs.html
typedef struct {
    int __detachstate;
    int __schedpolicy;
    //struct sched_param __schedparam;
    int __inheritsched;
    int __scope;
    size_t __guardsize;
    int __stackaddr_set;
    void *__stackaddr;
    unsigned long int __stacksize;
} pthread_attr_t;

typedef struct pthread_mutex_s {
    spinlock_t lock;    // protection contre les accès concurents
    unsigned state;     // etat du mutex
    list_t  wait;       // liste des threads en attente du mutex
} pthread_mutex_t;

#endif
