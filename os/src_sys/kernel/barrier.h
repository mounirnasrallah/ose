#ifndef _BARRIER_H_
#define _BARRIER_H_

#include <libk.h>

typedef struct barrier_s {
    spinlock_t lock;    // protection contre les accès concurents
    unsigned init;      // nombre de thread en attente
    unsigned counter;   // nombre de thread attendu
    list_t  wait;       // liste des threads en attente de la barriere
} barrier_t;

extern int barrier_init (barrier_t *barrier, unsigned init);// init est le nombre de th attendu
extern int barrier_wait (barrier_t *barrier);               // mise en attente ou reveil de tous

#endif
