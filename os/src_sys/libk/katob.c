#include <kstdlib.h>

int katob(const char *s)
{
    int res;
    for(res=0; *s; res=((*s=='0')||(*s=='1'))?(res<<1)+(*s-'0'):res,s++);
    return res;
}
