#ifndef _CRT0_H_
#define _CRT0_H_

typedef void (start_function_t)(void);

struct header_app_s {
    start_function_t *  start;
    uint32_t            magic;
    void *              data_end;
    void *              ram_end;
};

extern void _start(void);   // root function 
extern int main();          // main funtion of application
extern unsigned __data_end; // last address of data section
extern unsigned __ram_end;  // last address of ram section
extern unsigned __bss;      // first addr of bss section
extern unsigned __bss_end;  // last addr of bss section

#endif

