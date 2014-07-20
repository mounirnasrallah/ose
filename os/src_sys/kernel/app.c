#include <scheduler.h>
#include <thread.h>
#include <mutex.h>
#include <barrier.h>

mutex_t cpt_lock;
unsigned cpt;

barrier_t barrier;

void *emetmutex(void *arg)
{
    unsigned mycpt=0;
    mutex_lock( &cpt_lock);
    mycpt = cpt++;
    mutex_unlock( &cpt_lock);
    kprintf("th %p : %d\n", arg, mycpt);
    while (1)
    thread_yield();
	 return 0;//MISBAH: ajout pour compilation
}

void *emet_barrier(void *arg)
{
    unsigned mycpt;
    
    mycpt = cpt++;    
    kprintf("th %p : %d running in barrier \n", arg, mycpt);
	 MESSG(STANDARD,"app running\n");
	 //sleep(arg);
	 int status = barrier_wait(&barrier); 
    kprintf("th %p : %d barrier has finished with status = %d. \n", arg, mycpt, status);

    while (1)
    thread_yield();
    return 0;//MISBAH -->A VERIFIER
}

void *appthread(void *arg)
{
	//kprintf("app running \n");
	MESSG(STANDARD,"app running\n");
	barrier_init (&barrier, 5);

	 //threads/mutex runnings    
	 unsigned th=0x1000;
	/*
    thread_create(KTHREAD,1,emetmutex,(void*)(th++));
    thread_create(KTHREAD,1,emetmutex,(void*)(th++));
    thread_create(KTHREAD,1,emetmutex,(void*)(th++));
    thread_create(KTHREAD,1,emetmutex,(void*)(th++));
    thread_create(KTHREAD,1,emetmutex,(void*)(th++));

    for(int i=0;i<100000;i++) __asm__("nop");
    sched_dump();
	*/

	//Barrier/mutex runninng	
	th=0x1000;
    thread_create(KTHREAD,0,emet_barrier,(void*)(th++));
    thread_create(KTHREAD,0,emet_barrier,(void*)(th++));
    thread_create(KTHREAD,0,emet_barrier,(void*)(th++));
    thread_create(KTHREAD,0,emet_barrier,(void*)(th++));
    thread_create(KTHREAD,0,emet_barrier,(void*)(th++));

    for(int i=0;i<100000;i++) __asm__("nop");
    sched_dump();
	
    while (1);
	 return 0;//MISBAH: ajout pour compilation
} 
