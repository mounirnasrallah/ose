#include "segmentation.h"

MEMORY
{
    boot    : ORIGIN = BOOT_BASE,   LENGTH = BOOT_SIZE 
    ktext   : ORIGIN = KTEXT_BASE,  LENGTH = KTEXT_LMA_SIZE 
    kdata   : ORIGIN = KDATA_BASE,  LENGTH = KDATA_LMA_SIZE 
}

SECTIONS
{
    .boot   : { *(.boot) *(.boot.*) } > boot
    .ktext  : AT ( KTEXT_LMA_BASE ) { 
        *(.kentry) *(.text) 
        . = ALIGN(4); __ktext_end = .; 
    } > ktext
    .kdata  : AT ( KDATA_LMA_BASE ) { 
        *(.data*) *(.rodata*) . = ALIGN(4); __bss = .; 
        *(.scommon*) *(.bss*) *(COMMON*) . = ALIGN(4); __kdata_end = .; 
    } > kdata
}
