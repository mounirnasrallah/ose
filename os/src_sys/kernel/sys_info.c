#include <stdint.h>
#include <hal_cpu.h>

unsigned * sys_errno_location (void)
{
    return NULL;
}

unsigned sys_cpu_id (void)
{
    return CPUID;
}

unsigned sys_cpu_tsc (void)
{
    return CPUTIME;
}
