#include <scheduler.h>
#include <thread.h>

spinlock_t thread_rope_lock;
list_t thread_rope = LIST_INITIALIZER;

struct thread_s {
    spinlock_t lock;            // protection contre les accès concurrents
    thread_state_t state;       // CREATE, READY, ...
    thread_type_t type;         // ITHREAD, KTHREAD, ...
    unsigned cpuid;             // numéro du core affecté au thread
    list_t list;                // utilisé pour le chainage des threads dans les listes d'attente
    list_t rope;                // chaine tous les threads quelque soit leur état
	 thread_t *  join;           //thread attendant exit de ce thread
    unsigned pws[CONTEXT_SIZE]; // context
};

void thread_state_set(thread_t * thread, unsigned state)
{
	thread->state = state;
}

thread_state_t thread_state_get(thread_t * thread)
{
	return thread->state;
}

thread_type_t thread_type_get(thread_t * thread)
{
	return thread->type;
}

unsigned thread_cpuid_get(thread_t * thread)
{
	return thread->cpuid;
}

unsigned* thread_pws_get(thread_t * thread)
{
	return thread->pws;
}

thread_t * 
list_get_thread_element(list_t * el)
{
	thread_t *thread = list_item(el,thread_t,list);
	return thread;
}

void list_add_thread_last(list_t * listing, thread_t * thread)
{
	list_add_last(listing, &thread->list);
	return;
}

spinlock_t * thread_get_lock(thread_t * thread)
{
	if(!thread)
	return 0;

	return &thread->lock;
}

thread_t * thread_create (unsigned type, unsigned cpuid, thread_fct_t *start, void *arg)
{
    unsigned * stack = (unsigned *)kmalloc(CONFIG_STACK_SIZE) + CONFIG_STACK_SIZE/4 - 1;
    thread_t * thread = kmalloc(sizeof(thread_t));
    spin_init(&(thread->lock));

    thread->type = type;
    thread->cpuid = cpuid;
    list_root_init(&thread->list);

	 thread->join = 0;

    cpu_context_init(
        thread->pws,              // context space
        (unsigned)0,              // kernel
        (unsigned)stack,          // stack pointer
        (unsigned)start,          // behaviour function
        (unsigned)thread_exit,    // exit function
        (unsigned)arg);           // argument of start function
	
	 //Impacts on scheduler
    if(type==ITHREAD)
	 {
		sched_set_thread_idle(thread);
	 }	
		else
	 {
    	spin_lock(&thread_rope_lock);
    	list_add_last (&thread_rope, &(thread->rope));
	   //list_add_thread_last(&thread_rope,thread->rope);
    	spin_unlock(&thread_rope_lock);
    	sched_create(thread);
	 }	

    return thread;
}

void thread_dump(char *messg, thread_t * thread)
{
    char BUFFER[1024];
    char * thread_state_name [] = {"CREATE", "READY", "RUN", "WAIT", "DEAD"};
    char * thread_type_name  [] = {"ITHREAD", "KTHREAD"};
    MESSG(VERBOSE,"thread %p",thread);
    ksnprintf(BUFFER,sizeof(BUFFER),
        "\nthread %s %p\n"
        "    lock  : %d\n"
        "    state : %s\n"
        "    type  : %s\n"
        "    cpuid : %d\n"
        "    list  : %p (%p,%p)\n"
        "    rope  : %p (%p,%p)\n"
        "    S0_16 : %p S1_17 : %p S2_18 : %p S3_19 : %p\n"
        "    S4_20 : %p S5_21 : %p S6_22 : %p S7_23 : %p\n"
        "    SP_29 : %p S8_30 : %p RA_31 : %p C0_SR : %p\n"
        "    EXIT_FUNC : %p ARG1 : %p LOADABLE : %p\n"
        , messg, thread
        , thread->lock
        , thread_state_name[thread->state]
        , thread_type_name[thread->type]
        , thread->cpuid
        , &(thread->list), thread->list.next, thread->list.prev
        , &(thread->rope), thread->rope.next, thread->rope.prev
        , thread->pws[0], thread->pws[1], thread->pws[2], thread->pws[3]
        , thread->pws[4], thread->pws[5], thread->pws[6], thread->pws[7]
        , thread->pws[8], thread->pws[9], thread->pws[10], thread->pws[11]
        , thread->pws[12], thread->pws[13], thread->pws[14]);
    MESSG(VERBOSE,"%s",BUFFER);
}

void thread_rope_dump(void)
{
    list_foreach(&thread_rope,thread) {
        thread_dump("ROPE",list_item(thread,thread_t,rope));
    }
}

void thread_list_dump(char *messg, list_t *root)
{
    list_foreach(root,thread) {
        thread_dump(messg, list_item(thread,thread_t,list));
    }
}

unsigned thread_save(thread_t *thread)
{
    return cpu_context_save(thread->pws);
}

void thread_restore(thread_t *thread)
{
    MESSG(STANDARD,"%p", thread);
    sched_set_thread_run(thread);
    thread_state_set(thread, RUN);
    cpu_context_restore(thread->pws, 1);
    ERROR("THREAD_RESTORE : never return");
}

void thread_yield(void)
{
    spin_lock(&(THREAD->lock));
    sched_yield(THREAD);
}

void thread_exit (void * exit_value)
{
    spin_lock(&(THREAD->lock));
	 thread_state_set(sched_get_thread_run(), ZOMBIE);  
	 spin_unlock(&(THREAD->lock));
    if(sched_get_thread_run()->join)
	 {
		sched_wakeup(sched_get_thread_run()->join);
 	 }
    sched_exit(THREAD);
}

int thread_join(thread_t *thread, void ** value_ptr)
{
    thread_t *this = sched_get_thread_run();    
    spin_lock(&(thread->lock));
    if (thread_state_get(thread) == ZOMBIE)
	 {     
        spin_unlock(&(thread->lock));
    } else 
	 {                                            
        thread->join = this;                            
        thread_state_set(this, WAIT);

        spin_unlock(&(thread->lock));
        sched_sleep(this);
    }
    return 0;
}

void * thread_idle (void * arg)
{
    kprintf("thread_idle started on %d\n", CPUID);
    while(1) thread_yield();
    return NULL;
}

// -------------------------------------------------------------------------------------------------
// Initialisation
// -------------------------------------------------------------------------------------------------

void thread_init(void)
{
    MESSG(STANDARD,"thread_init ...\n");
	 spin_init(&thread_rope_lock);
    list_root_init(&thread_rope);
	 MESSG(STANDARD,"thread_init done\n");
}


