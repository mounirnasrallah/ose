#include <kstdlib.h>

int kstrcmp(const char *s1, const char *s2)
{
    unsigned char c1, c2;
    do {
        c1 = (unsigned char) *s1++;
        c2 = (unsigned char) *s2++;
    }
    while (c1 && (c1 == c2));
    return c1 - c2;
}
