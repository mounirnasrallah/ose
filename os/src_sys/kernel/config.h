#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <hal_arch.h>

#define CONFIG_MAGIC_APP        0xDEADBEEF

#define _MAX_(n,max)                 (((n)<=(max))?(n):(max))
#define CONFIG_CPU_NR               _MAX_(4,ARCH_CPU_NR        )
#define CONFIG_TTY_NR               _MAX_(4,ARCH_TTY_NR        )
#define CONFIG_TIMER_NR             _MAX_(4,ARCH_TIMER_NR      )
#define CONFIG_DMA_NR               _MAX_(1,ARCH_DMA_NR        )
#define CONFIG_FB_NR                _MAX_(1,ARCH_FB_NR         )
#define CONFIG_BD_NR                _MAX_(1,ARCH_BD_NR         )
#define CONFIG_ICU_NR               _MAX_(1,ARCH_ICU_NR        )
#define CONFIG_ICU_MAX_IRQ_NR       _MAX_(7,ARCH_ICU_MAX_IRQ_NR)
#define CONFIG_CPU_MAX_IRQ_NR       _MAX_(5,ARCH_CPU_MAX_IRQ_NR)
#define CONFIG_BOOT_STACK_SIZE      1024*16
#define CONFIG_STACK_SIZE           1024*16
#define CONFIG_FB_MAX_X             _MAX_(512,ARCH_FB_MAX_X)
#define CONFIG_FB_MAX_Y             _MAX_(512,ARCH_FB_MAX_Y)

#define CONSOLE     0
#define STDOUT      1
#define STDIN       1
#define STDERR      2
#define TTY_LOG     3

#endif
