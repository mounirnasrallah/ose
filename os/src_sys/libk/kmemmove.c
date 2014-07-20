#include <kstdlib.h>

void * kmemmove(void *dst, const void *src, unsigned size)
{
    register char *d = (char *)dst;
    register const char *s = src;
    if (s > d)
        while (size--) *d++ = *s++;
    else {
        d+=size-1;
        s+=size-1;
        while (size--) *d-- = *s--;
    }
    return (void *)dst;
}
