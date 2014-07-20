#include <segmentation.h>
#include <hal_arch.h>

// the only C function executed from the boot ROM
void __boot_loader(void) __attribute__ ((section(".boot")));

void __boot_loader()
{
    extern unsigned __ktext_end;        // first free addr of ktext section
    extern unsigned __bss;              // first addr of bss section
    extern unsigned __kdata_end;        // first free addr of kdata section
    unsigned *src, *dst;

    // move the code
    src = (unsigned *) KTEXT_LMA_BASE;
    dst = (unsigned *) KTEXT_BASE;
    while (dst != (unsigned *) &__ktext_end)
        *dst++ = *src++;

    // move the data
    src = (unsigned *) KDATA_LMA_BASE;
    dst = (unsigned *) KDATA_BASE;
    while (dst != (unsigned *) &__bss)
        *dst++ = *src++;

    // erase the bss section
    unsigned *bss = (unsigned *) &__bss;
    unsigned *end_bss = (unsigned *) &__kdata_end;
    while (bss != end_bss)
        *bss++ = 0;
}
