#ifndef _MUTEX_H_
#define _MUTEX_H_

#include <libk.h>

typedef struct mutex_s {
    spinlock_t lock;    // protection contre les accès concurents
    unsigned state;     // etat du mutex
    list_t  wait;       // liste des threads en attente du mutex
} mutex_t;

extern int mutex_init (mutex_t *mutex);     // initialise un mutex dont on passe le pointeur 
extern int mutex_lock (mutex_t *mutex);     // prend le verrou s'il est libre sinon met en attente
extern int mutex_try_lock (mutex_t *mutex);  // tentative non bloquante de prise de verrou
extern int mutex_unlock (mutex_t *mutex);   // rend le verrou et réveille un des threads en attente

#endif
