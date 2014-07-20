#include <stdlib.h>
#include <crt0.h>

__attribute__((section(".header"))) 
struct header_app_s header = {
    &_start,
    0xDEADBEEF,
    &__data_end,
    &__ram_end
};

int errno;

void _start(void)
{
    // FIRST set bss section to zero 
    for(int *p = (int*)&__bss; p != (int*)&__bss_end; *p++ = 0);

    // initialize the heap 
    // warning, head user must be managed by both usr & kernel since kernel
    // must sometimes allocate user memory for user stack for exemple
    malloc_init(header.data_end, header.ram_end);

    // call the main
    main();
} 

