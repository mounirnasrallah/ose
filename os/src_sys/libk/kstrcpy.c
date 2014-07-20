#include <kstdlib.h>

char * kstrcpy(char *dst, const char *src)
{
    register char *d = dst;
    while (*src) *d++ = *src++;
    *d=0;
    return dst;
}
