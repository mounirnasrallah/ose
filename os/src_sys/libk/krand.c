#include <kstdlib.h>

// http://www.gnu.org/software/gsl/manual/html_node/Unix-random-number-generators.html
//
// This is the BSD rand generator. Its sequence is
//     x_{n+1} = (a x_n + c) mod m
// with a = 1103515245, c = 12345 and m = RAND_MAX.
// The seed specifies the initial value, 1.
// The period of this generator is RAND_MAX, and it uses 1 word of storage per generator.

static unsigned next = 1;

int krand(void)
{
     return (next = next * 1103515245 + 12345) % ((unsigned)RAND_MAX + 1);
}

void ksrand(unsigned seed)
{
    next = seed;
}
