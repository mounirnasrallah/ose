#include <kstdio.h>
#include <kstdlib.h>
#include <stdarg.h>

char * kstring(const char *restrict fmt, ...)
{
    char buffer[KPRINTF_MAX];
    va_list ap;
    va_start (ap, fmt);
    kvsnprintf(buffer, sizeof(buffer), fmt, ap);
    va_end(ap);
    return kstrdup(buffer);
}
