#include <libk.h>
#include <hal_arch.h>
#include <driver_tty.h>
#include <driver_icu.h>
#include <driver_cpu.h>
#include <config.h>

void arch_init(void)
{
	 MESSG(STANDARD,"arch_init for malloc...\n");

    //---------------------------------------------------- initialization of kernel heap
    extern unsigned int __kdata_end;                       // defined in ldscript
    void *start = &__kdata_end;                            // first free addr of kdata section
    void *limit = (void *) (KDATA_BASE + KDATA_SIZE);      // last free addr of kdata section
    kmalloc_init(start, limit);
	 
	 MESSG(STANDARD,"arch_init for devices...\n");
//TODO: virer ca

   // Configurer le CPUID 0 pour ne recevoir que les interruptions du timer
    unsigned volatile *icu_base = (unsigned *) ICU_BASE;
    icu_base[2]= 1;//car bit 0 branche a TIMER
    //icu_base[ICU_MASK_SET]= 1;//car bit 0 branche a TIMER

    // Configurer le timer pour envoyer une interruption periodique
    
    unsigned volatile *timer_base = (unsigned *) TIMER_BASE;
    timer_base[TIMER_MODE]= 3;//Nombre de ticks avant d'envoyer une interruption
     timer_base[TIMER_PERIOD]= (int) 50000;//Nombre de ticks avant d'envoyer une interruption
     
    timer_base = (unsigned *) TIMER_BASE + 4 ;
    timer_base[TIMER_MODE]= 3;//Nombre de ticks avant d'envoyer une interruption
    timer_base[TIMER_PERIOD]= (int) 50000;//Nombre de ticks avant d'envoyer une interruption

    timer_base = (unsigned *) TIMER_BASE + (2 * 4) ;
    timer_base[TIMER_MODE]= 3;//Nombre de ticks avant d'envoyer une interruption
    timer_base[TIMER_PERIOD]= (int) 50000;//Nombre de ticks avant d'envoyer une interruption

    timer_base = (unsigned *) TIMER_BASE +  (3 *4);
    timer_base[TIMER_MODE]= 3;//Nombre de ticks avant d'envoyer une interruption
    timer_base[TIMER_PERIOD]= (int) 50000;//Nombre de ticks avant d'envoyer une interruption

//TODO: virer ca (fin de section)

    //---------------------------------------------------- initialization of all devices
    /*for (int dev=0; dev < CONFIG_CPU_NR; dev++)
        cpu_init(dev_cpu(dev), NULL, kstring("cpu-%d",dev));*/

    for (int dev=0; dev < CONFIG_ICU_NR; dev++)
        icu_init(dev_icu(dev), (uint32_t *)ICU_BASE+ICU_SPAN*dev, kstring("icu-%d",dev));

    for (int dev=0; dev < CONFIG_TTY_NR; dev++)
        tty_init(dev_tty(dev), (uint32_t*)TTY_BASE+TTY_SPAN*dev, kstring("tty-%d",dev));

    for (int dev=0; dev < CONFIG_TIMER_NR; dev++)
        timer_init(dev_timer(dev), (uint32_t*)TIMER_BASE+TIMER_SPAN*dev,kstring("timer-%d",dev));
/*
    for (int dev=0; dev < CONFIG_FB_NR; dev++)
        fb_init(&(fb_tbl[dev]),(uint8_t *)FB_BASE+FB_SIZE*dev, kstring("frame-buffer-0",dev));

    for (int dev=0; dev < CONFIG_DMA_NR; dev++)
        dma_init(&(dma_tbl[dev]), (uint32_t*)DMA_BASE+DMA_SPAN*dev, kstring("dma-%d",dev));

    for (int dev=0; dev < CONFIG_BD_NR; dev++)
        bd_init(&(bd_tbl[dev]), (uint32_t*)BD_BASE+BD_SPAN*dev, kstring("block-device-%d",dev));
*/
    //---------------------------------------------------- bind irq lines
    // bind the cpu irq input
    // interrupt line of timer n°i is bound to input n°0 of all cpu but cpu n°0
/*	 MESSG(STANDARD,"arch_init for binding cpu ...\n");
    cpu_bind(dev_cpu(0),dev_icu(0),   0);
    cpu_bind(dev_cpu(1),dev_timer(1), 0);
    cpu_bind(dev_cpu(2),dev_timer(2), 0);
    cpu_bind(dev_cpu(3),dev_timer(3), 0);

    // bind the irq irq inputs
    // interrupt line of tty n°i is bound to input n°i+1 of icu
	 MESSG(STANDARD,"arch_init for binding icu ...\n");
    icu_bind(dev_icu(0),dev_timer(0), 0);
    icu_bind(dev_icu(0),dev_tty(0),   1);
    icu_bind(dev_icu(0),dev_tty(1),   2);
    icu_bind(dev_icu(0),dev_tty(2),   3);
    icu_bind(dev_icu(0),dev_tty(3),   4);
    //icu_bind(dev_icu(0),dev_dma(0),   5);
    //icu_bind(dev_icu(0),dev_bd(0),    6);

    // enable interrupt from all devices
*/    MESSG(STANDARD,"arch_init for enabling interrupts ...\n");
    //dev_icu(0)->op.icu.set_mask(dev_icu(0),katob("1_1_1111_1"));
//TODO: activate
}
