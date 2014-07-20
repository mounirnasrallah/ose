#include <libk.h>
#include <syscall.h>

#define s(a)            #a
#define v(a)            s(a)

// -------------------------------------------------------------------------------------------------
// CPU CONTEXT
// -------------------------------------------------------------------------------------------------

#define     S0_16        0
#define     S1_17        1
#define     S2_18        2
#define     S3_19        3
#define     S4_20        4
#define     S5_21        5
#define     S6_22        6
#define     S7_23        7
#define     SP_29        8
#define     S8_30        9
#define     RA_31        10
#define     C0_SR        11
#define     EXIT_FUNC    12
#define     ARG1         13
#define     LOADABLE     14

#define TTY_BASE        0xd0200000

void cpu_context_init(unsigned ctx[], unsigned mode_usr, unsigned stack_ptr,
                      unsigned entry_func, unsigned exit_func,
                      unsigned arg1)
{
    ctx[C0_SR]      = (mode_usr) ? 0xFC13 : 0xFC03;
    ctx[SP_29]      = stack_ptr;
    ctx[RA_31]      = entry_func;
    ctx[EXIT_FUNC]  = exit_func;
    ctx[ARG1]       = arg1;
    ctx[LOADABLE]   = 1;
    // required to be sure that all data are written
    __asm__ volatile ("sync");
}

// unsigned cpu_context_save(struct cpu_context_s * ctx)
__asm__(
".section   .text, \"ax\", @progbits                \n"
".align     2                                       \n"
".globl     cpu_context_save                        \n" // external function
".ent       cpu_context_save                        \n"
"                                                   \n"
"cpu_context_save:                                  \n"
"   mfc0    $2,     $12                             \n" // get SR
"   sw      $16,    "v(S0_16)"*4($4)                \n" // save registers
"   sw      $17,    "v(S1_17)"*4($4)                \n"
"   sw      $18,    "v(S2_18)"*4($4)                \n"
"   sw      $19,    "v(S3_19)"*4($4)                \n"
"   sw      $20,    "v(S4_20)"*4($4)                \n"
"   sw      $21,    "v(S5_21)"*4($4)                \n"
"   sw      $22,    "v(S6_22)"*4($4)                \n"
"   sw      $23,    "v(S7_23)"*4($4)                \n"
"   sw      $29,    "v(SP_29)"*4($4)                \n"
"   sw      $30,    "v(S8_30)"*4($4)                \n"
"   sw      $31,    "v(RA_31)"*4($4)                \n"
"   sw      $2,     "v(C0_SR)"*4($4)                \n" // save SR
"   li      $2,     0                               \n" // return 0 after save
"   jr      $31                                     \n"
"                                                   \n"
".end       cpu_context_save                        \n"
);

// void cpu_context_restore(struct cpu_context_s * ctx, unsigned val)
__asm__(
".section   .text, \"ax\", @progbits                \n"
".align     2                                       \n"
".globl     cpu_context_restore                     \n" // external function
".ent       cpu_context_restore                     \n"
"                                                   \n"
"cpu_context_restore:                               \n"
"   lw      $27,    "v(LOADABLE)"*4($4)             \n" // get loadable flag
"   lw      $26,    "v(C0_SR)"*4($4)                \n" // get mode
"   lw      $29,    "v(SP_29)"*4($4)                \n" // get stack ptr
"   bne     $27,    $0,     cpu_context_load        \n" // if set then context_load
"   lw      $31,    "v(RA_31)"*4($4)                \n" // return to cpu_context_save
"   lw      $16,    "v(S0_16)"*4($4)                \n" // retore registers
"   lw      $17,    "v(S1_17)"*4($4)                \n"
"   lw      $18,    "v(S2_18)"*4($4)                \n"
"   lw      $19,    "v(S3_19)"*4($4)                \n"
"   lw      $20,    "v(S4_20)"*4($4)                \n"
"   lw      $21,    "v(S5_21)"*4($4)                \n"
"   lw      $22,    "v(S6_22)"*4($4)                \n"
"   lw      $23,    "v(S7_23)"*4($4)                \n"
"   lw      $30,    "v(S8_30)"*4($4)                \n"
"   mtc0    $26,    $12                             \n" // restore SR
"   move    $2,     $5                              \n"
"   jr      $31                                     \n"
"                                                   \n"
"cpu_context_load:                                  \n"
"   addiu   $29,    $29,  -8                        \n" // entry_func has one arg
"   sw      $0,     "v(LOADABLE)"*4($4)             \n" // reset loadable flag
"   lw      $27,    "v(RA_31)"*4($4)                \n" // get entry_func
"   lw      $5,     "v(EXIT_FUNC)"*4($4)            \n" // get exit_func
"   la      $31,    cpu_exit_wrap                   \n" // when entry_func returns, => exit_func($2)
"   sw      $5,     4($29)                          \n" // save exit_func address
"   lw      $4,     "v(ARG1)"*4($4)                 \n" // get thread arg1
"   mtc0    $27,    $14                             \n" // set EPC with entry_func used by eret
"   mtc0    $26,    $12                             \n" // set SR
"   eret                                            \n"
"                                                   \n"
"cpu_exit_wrap:                                     \n"
"   lw      $5,     4($29)                          \n" // get exit_func
"   addu    $4,     $0,   $2                        \n"
"   jr      $5                                      \n"
"                                                   \n"
".end       cpu_context_restore                     \n"
);

// -------------------------------------------------------------------------------------------------
// KENTRY
// -------------------------------------------------------------------------------------------------

#define     AT          0
#define     V0          1
#define     V1          2
#define     A0          3
#define     A1          4
#define     A2          5
#define     A3          6
#define     T0          7
#define     T1          8
#define     T2          9
#define     T3          10
#define     T4          11
#define     T5          12
#define     T6          13
#define     T7          14
#define     T8          15
#define     T9          16
#define     S0          17
#define     S1          18
#define     S2          19
#define     S3          20
#define     S4          21
#define     S5          22
#define     S6          23
#define     S7          24
#define     S8          25
#define     LO          26
#define     HI          27
#define     K0          28
#define     K1          29
#define     GP          30
#define     SP          31
#define     RA          32
#define     EPC         33
#define     CR          34
#define     SR          35
#define     BAR         36
#define     CPU         37
#define     TSC         38
#define     NC          39
#define     SAVE_REG_NB 40

static const char * const REGISTER_NAME[] = {
    "AT ", "V0 ", "V1 ", "A0 ", "A1 ", "A2 ", "A3 ", "T0 ", "T1 ", "T2 ",
    "T3 ", "T4 ", "T5 ", "T6 ", "T7 ", "T8 ", "T9 ", "S0 ", "S1 ", "S2 ",
    "S3 ", "S4 ", "S5 ", "S6 ", "S7 ", "S8 ", "LO ", "HI ", "K0 ", "K1 ",
    "GP ", "SP ", "RA ", "EPC", "CR ", "SR ", "BAR", "CPU", "TSC", "NC "
};

#define NBA 1 // number of arguments of called functions

__asm__(
".section   .kentry,\"ax\",@progbits                \n"
".ent        kentry                                 \n"
".set        noat                                   \n"
".org        0x180                                  \n"
"                                                   \n"
"kentry:                                            \n"
"   mfc0    $26,    $13                             \n" // read CR
"   andi    $26,    $26,    0x3C                    \n" // apply cause mask
"   li      $27,    0x20                            \n" // syscall code
"   bne     $26,    $27,    not_syscall             \n" // that is not a syscall
"                                                   \n"
"syscall:                                           \n" // $4, $5, $6, $7 contains syscall args
"   addiu   $29,    $29,    -7*4                    \n" // for $31 SR + service + args
"   mfc0    $27,    $12                             \n" // read SR
"   sw      $2,     4*4($29)                        \n" // save service
"   sw      $27,    5*4($29)                        \n" // save SR
"   sw      $31,    6*4($29)                        \n" // save $31 return of funct with syscall
"   la      $26,    syscall_vector                  \n" // function to call
"   andi    $2,     $2,     "v(SYSCALL_NR)"-1       \n" // SYSCALL_NB must a power of 2
"   sll     $2,     $2,     2                       \n" // compute syscall index
"   addu    $26,    $26,    $2                      \n" // in syscall_vector
"   lw      $26,    ($26)                           \n" // get syscall function address
"   li      $27,    0xFF00                          \n" // UM=0 ERL=0 EXL=0 IE=0
"   mtc0    $27,    $12                             \n" // SR <- kenel-mode, without INT
"   bnez    $26,    syscall_do                      \n" // problem in syscall vector
"   la      $26,    sys_undefined                   \n" // dump arguments
"syscall_do:                                        \n"
"   jalr    $26                                     \n" // call syscall function
"   lw      $27,    5*4($29)                        \n" // get old SR
"   lw      $31,    6*4($29)                        \n" // restore $31
"   mtc0    $27,    $12                             \n" // restore SR
"   mtc0    $31,    $14                             \n" // EPC <- $31 since using eret
"   addiu   $29,    $29,    7*4                     \n" // restore stack pointer
"   eret                                            \n" // equiv to jr $31 
"                                                   \n"
"                                                   \n"
"not_syscall:                                       \n"
"   addiu   $29,    $29, -("v(SAVE_REG_NB+NBA)")*4  \n" // saved regs + function args
"   sw      $1,     ("v(NBA+AT)")*4($29)            \n" // save temporary registers
"   sw      $2,     ("v(NBA+V0)")*4($29)            \n"
"   sw      $3,     ("v(NBA+V1)")*4($29)            \n"
"   sw      $4,     ("v(NBA+A0)")*4($29)            \n"
"   sw      $5,     ("v(NBA+A1)")*4($29)            \n"
"   sw      $6,     ("v(NBA+A2)")*4($29)            \n"
"   sw      $7,     ("v(NBA+A3)")*4($29)            \n"
"   sw      $8,     ("v(NBA+T0)")*4($29)            \n"
"   sw      $9,     ("v(NBA+T1)")*4($29)            \n"
"   sw      $10,    ("v(NBA+T2)")*4($29)            \n"
"   sw      $11,    ("v(NBA+T3)")*4($29)            \n"
"   sw      $12,    ("v(NBA+T4)")*4($29)            \n"
"   sw      $13,    ("v(NBA+T5)")*4($29)            \n"
"   sw      $14,    ("v(NBA+T6)")*4($29)            \n"
"   sw      $15,    ("v(NBA+T7)")*4($29)            \n"
"   sw      $24,    ("v(NBA+T8)")*4($29)            \n"
"   sw      $25,    ("v(NBA+T9)")*4($29)            \n"
"   sw      $28,    ("v(NBA+GP)")*4($29)            \n"
"   sw      $31,    ("v(NBA+RA)")*4($29)            \n"
"   mflo    $1                                      \n"
"   mfhi    $2                                      \n"
"   mfc0    $3,     $14                             \n" // EPC Exception Program Counter
"   sw      $1,     ("v(NBA+LO)")*4($29)            \n"
"   sw      $2,     ("v(NBA+HI)")*4($29)            \n"
"   sw      $3,     ("v(NBA+EPC)")*4($29)           \n"
"                                                   \n"
"interrupt:                                         \n"
"   la      $27,    __do_interrupt                  \n" // get __do_interrupt address
"   bne     $26,    $0, exception                   \n" // compare CR to 0
"   mfc0    $4,     $0                              \n" // CPU_ID, 1th arg
"   jalr    $27                                     \n" // __do_interrupt(CPUID)
"                                                   \n"
"return_from_interrupt:                             \n"
"   lw      $3,     ("v(NBA+EPC)")*4($29)           \n" // restore temporary registers
"   lw      $2,     ("v(NBA+HI)")*4($29)            \n"
"   lw      $1,     ("v(NBA+LO)")*4($29)            \n"
"   mtc0    $3,     $14                             \n"
"   mthi    $2                                      \n"
"   mtlo    $1                                      \n"
"   lw      $31,    ("v(NBA+RA)")*4($29)            \n"
"   lw      $28,    ("v(NBA+GP)")*4($29)            \n"
"   lw      $25,    ("v(NBA+T9)")*4($29)            \n"
"   lw      $24,    ("v(NBA+T8)")*4($29)            \n"
"   lw      $15,    ("v(NBA+T7)")*4($29)            \n"
"   lw      $14,    ("v(NBA+T6)")*4($29)            \n"
"   lw      $13,    ("v(NBA+T5)")*4($29)            \n"
"   lw      $12,    ("v(NBA+T4)")*4($29)            \n"
"   lw      $11,    ("v(NBA+T3)")*4($29)            \n"
"   lw      $10,    ("v(NBA+T2)")*4($29)            \n"
"   lw      $9,     ("v(NBA+T1)")*4($29)            \n"
"   lw      $8,     ("v(NBA+T0)")*4($29)            \n"
"   lw      $7,     ("v(NBA+A3)")*4($29)            \n"
"   lw      $6,     ("v(NBA+A2)")*4($29)            \n"
"   lw      $5,     ("v(NBA+A1)")*4($29)            \n"
"   lw      $4,     ("v(NBA+A0)")*4($29)            \n"
"   lw      $3,     ("v(NBA+V1)")*4($29)            \n"
"   lw      $2,     ("v(NBA+V0)")*4($29)            \n"
"   lw      $1,     ("v(NBA+AT)")*4($29)            \n"
"   addiu   $29,    $29, ("v(SAVE_REG_NB+NBA)")*4   \n"
"   eret                                            \n"
"                                                   \n"
"exception:                                         \n"
"   mfc0    $4,     $0                              \n" // CPU identifier
"   mfc0    $5,     $8                              \n" // BAR Bad Address Regsiter
"   mfc0    $6,     $9                              \n" // TSC Time Stamp Counter
"   mfc0    $7,     $12                             \n" // SR  Status Register
"   mfc0    $8,     $13                             \n" // CR  Cause Register
"   addiu   $9,     $29,("v(SAVE_REG_NB+NBA)")*4    \n"
"   sw      $4,     ("v(NBA+CPU)")*4($29)           \n"
"   sw      $5,     ("v(NBA+BAR)")*4($29)           \n"
"   sw      $6,     ("v(NBA+TSC)")*4($29)           \n"
"   sw      $7,     ("v(NBA+SR)")*4($29)            \n"
"   sw      $8,     ("v(NBA+CR)")*4($29)            \n"
"   sw      $9,     ("v(NBA+SP)")*4($29)            \n"
"   sw      $16,    ("v(NBA+S0)")*4($29)            \n"
"   sw      $17,    ("v(NBA+S1)")*4($29)            \n"
"   sw      $18,    ("v(NBA+S2)")*4($29)            \n"
"   sw      $19,    ("v(NBA+S3)")*4($29)            \n"
"   sw      $20,    ("v(NBA+S4)")*4($29)            \n"
"   sw      $21,    ("v(NBA+S5)")*4($29)            \n"
"   sw      $22,    ("v(NBA+S6)")*4($29)            \n"
"   sw      $23,    ("v(NBA+S7)")*4($29)            \n"
"   sw      $30,    ("v(NBA+S8)")*4($29)            \n"
"   sw      $26,    ("v(NBA+K0)")*4($29)            \n"
"   sw      $27,    ("v(NBA+K1)")*4($29)            \n"
"                                                   \n"
"   addiu   $4,     $29,    "v(NBA)"*4              \n" // regs_table
"   la      $27,    __do_exception_cpu              \n" // __do_exception(regs_table)
"   jr      $27                                     \n"
"                                                   \n"
"   .set at                                         \n"
"   .end kentry                                     \n"
);

// -------------------------------------------------------------------------------------------------
// EXCEPTION DUMP
// even there is not assembler line, this code depends on registers thus, it must be here.
// -------------------------------------------------------------------------------------------------

__attribute__((used))
static void __do_exception_cpu(unsigned *reg_table)
{
    char buffer[1024];//80 + (SAVE_REG_NB/4)*(16*4+2)];
    unsigned written = 0;
    static const char * const CAUSE_NAME[] = {
        "INT Interruption",
        "Unused",
        "Unused",
        "Unused",
        "ADEL Address Error Load",
        "ADES Adress Error Store",
        "IBE Instruction Bus Error",
        "DBE Data Bus Error",
        "SYS Syscall",
        "BP BeakPoint",
        "RI Illegal Instruction",
        "CPU Bad Coprocessor",
        "OVF Overflow ",
        "Unused",
        "Unused",
        "Unused"
    };
    written = ksnprintf(buffer, sizeof(buffer), "[%d:%d] FATAL ERROR = %s\n",
                        CPUID,CPUTIME,CAUSE_NAME[(reg_table[CR]>>2)&0xF]);
    for (int r=0; r < SAVE_REG_NB/4; r++) {
        written += ksnprintf(buffer+written, sizeof(buffer)-written,
                            "| %s  %x\t| %s  %x\t| %s  %x\t| %s  %x\t|\n",
                            REGISTER_NAME[r],    reg_table[r],
                            REGISTER_NAME[r+10], reg_table[r+10],
                            REGISTER_NAME[r+20], reg_table[r+20],
                            REGISTER_NAME[r+30], reg_table[r+30]);
    }
    kputs(buffer);
    while (1);
}

// -------------------------------------------------------------------------------------------------
// BOOT
// -------------------------------------------------------------------------------------------------

#define STACK_SIZE  CONFIG_BOOT_STACK_SIZE

__asm__(//
".section   .boot ,\"ax\",@progbits                 \n"
".extern     boot_signal                            \n"
".extern     __boot_loader                          \n"
".ent        boot                                   \n"
".align      2                                      \n"
"                                                   \n"
"boot:                                              \n"
"   li      $26,    0                               \n"
"   mtc0    $26,    $12                             \n" // Status Register
"   mfc0    $16,    $0                              \n" // CPU_ID
"   la      $27,    "v(RAM_BASE+RAM_SIZE)"          \n" // top memory
"   li      $26,    ("v(STACK_SIZE)")               \n"
"   mult    $16,    $26                             \n"
"   mflo    $29                                     \n"
"   subu    $29,    $29,    $27                     \n" // $29 <= TOP - procid * STACK_SIZE
"   addiu   $29,    $29,    -1*4                    \n" // for __do_init/__boot_loade  argument
"   bne     $16,    $0,     boot_wait               \n" // goto to boot_wait if procidi=0
"   la      $26,    __boot_loader                   \n"
"   jalr    $26                                     \n"
"                                                   \n"
"call_do_init:                                      \n"
"   or      $4,     $0,     $16                     \n"
"   la      $26,    __do_init                       \n"
"   jr      $26                                     \n"
"                                                   \n"
"boot_wait:                                         \n"
"   la      $26,    boot_signal                     \n"
"   lw      $27,    0($26)                          \n"
"   sub     $5,     $27,    $16                     \n" // $5 <- bootsignal - cpuid
"   bgez    $5,     call_do_init                    \n" // excute __do_init only if cpuid is greater
"   j       boot_wait                               \n" // than bootsignal
"                                                   \n"
".end boot                                          \n"
);
