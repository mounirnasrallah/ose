#include <segmentation.h>

MEMORY
{
    ram   : ORIGIN = RAM_BASE, LENGTH = RAM_SIZE_USER 
}

__ram_end = RAM_BASE + RAM_SIZE_USER ;

SECTIONS
{
    .ram   : { 
        *(.header) 
        *(.text) 
        . = ALIGN(4); __data = .;
        *(.data*) *(.rodata*) 
        . = ALIGN(4); __bss = .; 
        *(.scommon*) *(.bss*) *(COMMON*) 
        . = ALIGN(4); __bss_end = .; 
        . = ALIGN(512); __data_end = .;
    } > ram
}

// all following command can have several arguments separated by space
// SEARCH_DIR for all directories where librairies are placed, eq. to -L
// GROUP      force librairies to be read, eq. to -l
// INPUT      force object file to be read 

SEARCH_DIR(LIB_DIR)        
GROUP(LIB_DIR/libc.a)
INPUT(LIB_DIR/crt0.o LIB_DIR/syscall.o)


