#include <stdio.h>
#include <syscall.h>

int putc(int c, int fd)
{
    return write(STDOUT, (char *)&c, 1);
}

