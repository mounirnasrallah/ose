#include <stdint.h>
#include <kstdio.h>

char * kgets(char * restrict str, int size)
{
    register char * p = str;
    if (size <= 0)
        return NULL;
    while ((--size) && ((*p++ = kgetc()) != '\n'));
    *p = 0;
    return str;
}
