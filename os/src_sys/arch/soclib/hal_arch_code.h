#ifndef _HAL_ARCH_CODE_H_
#define _HAL_ARCH_CODE_H_
#include <segmentation.h>
#include <hal_cpu.h>

#ifndef _HAL_ARCH_H_
#error this file connot be included directly, use hal_arch.h instead
#endif

// --------------------------------------------- Drivers

#include <driver_cpu.h>
#include <driver_tty.h>
#include <driver_timer.h>
#include <driver_icu.h>

// --------------------------------------------- Console Kernel

#define TTY_WRITE_REG      0
#define TTY_STATUS_REG     1
#define TTY_READ_REG       2

#define TIMER_VALUE 0
#define TIMER_MODE 1
#define TIMER_PERIOD 2
#define TIMER_RESETIRQ 3

static inline int kgetc(void)
{
    unsigned volatile *tty_base = (unsigned *) TTY_BASE;
    while (tty_base[TTY_STATUS_REG] == 0);
    return tty_base[TTY_READ_REG];
}
static inline void kputc(int c)
{
    unsigned volatile *tty_base = (unsigned *) TTY_BASE;
    tty_base[TTY_WRITE_REG] = c;
}

// --------------------------------------------- Arch Spinlock

#define SPINLOCK_INITIALIZER 0

static inline void spin_init(spinlock_t *lock) {
    cpu_spin_init((unsigned *)lock);
}
static inline void spin_destroy(spinlock_t *lock) {
    cpu_spin_destroy((unsigned *)lock);
}
static inline int spin_trylock(spinlock_t *lock) {
    return cpu_spin_trylock((unsigned *)lock);
}
static inline void spin_lock(spinlock_t *lock) {
    cpu_spin_lock((unsigned *)lock);
}
static inline void spin_unlock(spinlock_t *lock) {
    cpu_spin_unlock((unsigned *)lock);
}
static inline void spin_lock_noirq(spinlock_t *lock, unsigned * status) {
    cpu_disable_all_irq(status);
    cpu_spin_lock((unsigned *)lock);
}
static inline void spin_unlock_noirq(spinlock_t *lock, unsigned status) {
    cpu_spin_unlock((unsigned *)lock);
    cpu_restore_irq(status);
}

#endif
