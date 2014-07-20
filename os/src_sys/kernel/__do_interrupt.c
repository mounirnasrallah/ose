#include <libk.h>
#include <scheduler.h>

// -------------------------------------------------------------------------------------------------
// Interrupt wrapper called by kentry assembler code
// When an interruption happens to a CPU, we want to call the irq_handler pointed from cpu instance
// -------------------------------------------------------------------------------------------------

void __do_interrupt(unsigned cpu_id)
{
    device_t *cpu = dev_cpu(cpu_id);
    cpu->action.irq_handler(cpu->action.dev);
}
