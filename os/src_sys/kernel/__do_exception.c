#include <libk.h>

// -------------------------------------------------------------------------------------------------
// Exception wrapper called by kentry assembler code
// -------------------------------------------------------------------------------------------------

void __do_exception(unsigned cpu_id)
{
    kprintf("Exception received from %d\n", cpu_id);
}

