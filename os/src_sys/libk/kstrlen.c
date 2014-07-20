#include <kstdlib.h>

int kstrlen(const char *s)
{
    char * e = (char *)s;
    while ( *e++ );
    return (int) (e-s-1);
}
