#include <hal_cpu.h>
#include <config.h>
#include <libk.h>
#include <scheduler.h>
#include <thread.h>
#include <app.h>

// -------------------------------------------------------------------------------------------------
// System Initialisation
// -------------------------------------------------------------------------------------------------

unsigned boot_signal;

void __do_init(unsigned cpu_id)
{
   //if (CPUID == 0)
		//MESSG(STANDARD,"################################# __do_init cpu_id= %d\n", cpu_id);
	if (CPUID == 0) {
	       
		 arch_init();
		 thread_init();//TODO: activate
		 //MESSG(STANDARD,"__do_init send signal availibility from cpu_id= %d\n", cpu_id);
	    boot_signal = CONFIG_CPU_NR;
		  MESSG(STANDARD,"__do_init initializations for cpu_id= %d\n", cpu_id);
		 //MESSG(STANDARD,"__do_init end initializations for cpu_id= %d\n", cpu_id);
     
    }
	 //MESSG(STANDARD,"__do_init sched_init for cpu_id= %d\n", cpu_id);
    sched_init  ();

    if ( CPUID == 0){
		  MESSG(STANDARD,"__do_init app for cpu_id= %d\n", cpu_id);
        thread_create(KTHREAD, 0, appthread, NULL);
	}

	//MESSG(STANDARD,"__do_init thread call app for cpu_id= %d\n", cpu_id);
	//thread_create(KTHREAD, 0, &app, NULL);

	//MESSG(STANDARD,"__do_init thread_idle for cpu_id= %d\n", cpu_id);
	//thread_restore(thread_create(ITHREAD,cpu_id,&thread_idle,NULL));

	//MESSG(STANDARD,"__do_init sched_start for cpu_id= %d\n", cpu_id);
   sched_start(thread_create(ITHREAD, CPUID, thread_idle, NULL));
}
