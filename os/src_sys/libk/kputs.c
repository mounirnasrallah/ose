#include <hal_arch.h>
#include <kstdio.h>
#include <stdarg.h>

//static spinlock_t spinlock_kputs = SPINLOCK_INITIALIZER ;

int kputs(char *buffer)
{
    char *p = buffer;
    //cpu_spin_lock(&spinlock_kputs);
    while (*p) kputc(*p++);
    //cpu_spin_unlock(&spinlock_kputs);
    return (int)((unsigned)p-(unsigned)buffer);
}
