#include <kstdio.h>
#include <stdarg.h>

int ksnprintf(char *buffer, unsigned size, const char *restrict fmt, ...)
{
    va_list ap;
    va_start (ap, fmt);
    int res = kvsnprintf(buffer, size, fmt, ap);
    va_end(ap);
    return res;
}
