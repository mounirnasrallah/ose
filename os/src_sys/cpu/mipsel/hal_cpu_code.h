#ifndef  _HAL_CPU_CODE_H_
#define  _HAL_CPU_CODE_H_

#ifndef _HAL_CPU_H_
#error this file cannot be included directly, use hal_cpu.h instead
#endif

#include <config.h>

#undef      CONTEXT_SIZE
#define     CONTEXT_SIZE 15

//---------------------------------------------- Special Registers 

static inline unsigned cpu_get_id(void)
{
    register unsigned int proc_id;
    __asm__ volatile ("mfc0  %0, $0":"=r" (proc_id));
    return proc_id;
}
static inline unsigned cpu_time_stamp(void)
{
    register unsigned cycles;
    __asm__ volatile ("mfc0  %0, $9":"=r" (cycles));
    return cycles;
}

//------------------------------------------------ IRQ set/mask

#undef  CPU_IRQ_NR
#define CPU_IRQ_NR   6

static inline int cpu_get_highest_irq (void)
{
    register unsigned cause;
    __asm__ volatile ("mfc0 %0, $13":"=r" (cause));
    cause = (cause >> 10) & 0x3F;
    for (int irq = 0; cause; cause >>= 1, irq++)
	    if (cause & 1)
	        return irq;
    return -1;
}
static inline void cpu_disable_single_irq_mask(unsigned irq_mask, unsigned * old)
{
    register unsigned old_sr, new_sr;
    __asm__ volatile ("mfc0   %0, $12  \n":"=r" (old_sr));  // get old_SR
    if (old) *old = old_sr;
    new_sr = old_sr & ~irq_mask;
    __asm__ volatile ("mtc0   %0, $12  \n"::"r" (new_sr));  // set new_SR
}
static inline void cpu_enable_single_irq_mask(unsigned irq_mask, unsigned * old)
{
    register unsigned old_sr, new_sr;
    __asm__ volatile ("mfc0   %0, $12  \n":"=r" (old_sr));  // get old_SR
    if (old) *old = old_sr;
    new_sr = old_sr | irq_mask;
    __asm__ volatile ("mtc0   %0, $12  \n"::"r" (new_sr));  // set new_SR
}
static inline void cpu_disable_single_irq(unsigned irq_num, unsigned * old)
{
    cpu_disable_single_irq_mask((1 << (10 + irq_num)),old);
}
static inline void cpu_enable_single_irq(unsigned irq_num, unsigned * old)
{
    cpu_enable_single_irq_mask((1 << (10 + irq_num)),old);
}
static inline void cpu_disable_all_irq(unsigned * old)
{
    cpu_disable_single_irq_mask(1,old);
}
static inline void cpu_enable_all_irq(unsigned * old)
{
    cpu_enable_single_irq_mask(1,old);
}
static inline void cpu_restore_irq(unsigned old)
{
    __asm__ volatile ("mtc0 %0, $12"::"r" (old));
}

//-------------------------------------------------- spinlock 

static inline void cpu_spin_init(unsigned * lock)
{
    __asm__ volatile (
        "sync             \n" // to be sure that previous store are acheived
        "sw $0, (%0)      \n" // unlock
        "sync             \n" // to empty the write buffer
        ::"r" (lock));
}

static inline unsigned cpu_spin_trylock(unsigned * lock)
{
    register unsigned state;
    //__(Y);
    __asm__ volatile (
        ".set noreorder   \n" // do not modify the sequence
        "lw    %0, (%1)   \n" // load lock state
        "bnez  %0, 32     \n" // forgive whenever lock is busy (i.e. != 0)
        "nop              \n" // delayed slot
        "ll    %0, (%1)   \n" // load lock value in $2 and try to register in memory
        "nop              \n" // delayed slot
        "bnez  %0, 16     \n" // forgive whenever lock is busy
        "ori   %0, 1      \n" // preload 1
        "sc    %0, (%1)   \n" // sc returns in %0 <- 1 if free, 0 if busy
        "nop              \n" // delayed slot
        "xori  %0, %0,  1 \n" // trylock returns 0 if free, 1 if busy
        ".set  reorder    \n" // return to normal mode
        :"=r" (state)
        :"r" (lock));
    return state;
}
static inline void cpu_spin_lock(unsigned * lock)
{
    while ((cpu_spin_trylock(lock)));
}
static inline void cpu_spin_unlock(unsigned * lock)
{
    cpu_spin_init(lock);
}
static inline void cpu_spin_destroy(unsigned * lock)
{
}

//---------------------------------------------- Cache operations

#undef CACHE_LINE_SIZE
#define CACHE_LINE_SIZE 64

static inline void cpu_invalid_dcache_line(void *ptr)
{
    __asm__ volatile ("cache %0, (%1) \n"::"i" (0x11), "r"(ptr));
}

//---------------------------------------------- USER -> KERNEL

// for MIPS : $2 must contain the service number

#define SYSCALL(service) {  \
    __asm__ volatile (      \
    "   li      $2, %0 \n"  \
    "   syscall        \n"  \
    ::"i"(service));        \
    return 0;               \
}

#define SYSCALL_VOID(service) {  \
    __asm__ volatile (      \
    "   li      $2, %0 \n"  \
    "   syscall        \n"  \
    ::"i"(service));        \
}

#endif
