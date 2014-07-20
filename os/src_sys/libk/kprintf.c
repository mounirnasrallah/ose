#include <kstdio.h>
#include <stdarg.h>

int kprintf(const char *restrict fmt, ...)
{
    char buffer[KPRINTF_MAX];
    va_list ap;
    va_start (ap, fmt);
    int res = kvsnprintf(buffer, sizeof(buffer), fmt, ap);
    va_end(ap);
    kputs(buffer);
    return res;
}
