#include <kstdlib.h>

char * kstrdup(const char *src)
{
    char *dst = kmalloc(kstrlen(src)+1);
    if (dst) kstrcpy(dst,src);
    return dst;
}
