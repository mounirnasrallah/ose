#ifndef _KSTDIO_H_
#define _KSTDIO_H_

#include <hal_arch.h>
#include <stdarg.h>
#include <libk.h>
#include <scheduler.h>

// affiche le message format sur le terminal 0
// Les messages sont tronque a KPRINTF_MAX
// kprintf gere formatage %[-][number]<fmt> :
//  [-] pour : la justification a droite [optionnel]
//  [number] : pour le nombre de caractere utilises pour THRl'affichage
//  <fmt>    : c s d x X p

#define KPRINTF_MAX 1024
extern int kputs(char *buffer);
extern int kprintf(const char *restrict fmt, ...);
extern char * kstring(const char *restrict fmt, ...);
extern int ksnprintf(char *buffer, unsigned size, const char *restrict fmt, ...);
extern int kvsnprintf(char *buffer, unsigned size, const char *restrict fmt, va_list ap);

#define THREAD sched_get_thread_run()
//schedroot[CPUID].run //MISBAH

#define SILENT   0
#define QUIET    DEBUG>=1
#define STANDARD DEBUG>=2
#define VERBOSE  DEBUG>=3

#if NOMESSG
#   define MESSG(cond,fmt,arg...)
#   define ASSERT(cond,fmt,arg...)
#   define ASSERT_ERROR(cond,fmt,arg...)
#   define ERROR(fmt,arg...)
#   define BIP(a)
#   define _p(c)
#else
#   define FMT0(fmt) "[%d:%d] " fmt"\n",CPUID,CPUTIME
#   define FMT1(fmt) "[%d:%p:%d:%s/%s] " fmt"\n",CPUID,THREAD,CPUTIME,__FILE__,__FUNCTION__
#   define FMT2(fmt) "[%d:%p:%d:%s/%s] ASSERT %s "fmt"\n",CPUID,THREAD,CPUTIME,__FILE__,__FUNCTION__
#   ifndef DEBUG
#       define DEBUG 1
#   endif
#   define MESSG(cond,fmt,arg...) \
        if (cond) kprintf(FMT1(fmt),##arg)
#   define ASSERT(cond,fmt,arg...) \
        if (!(cond)) kprintf(FMT2(fmt),#cond,##arg)
#   define ASSERT_ERROR(cond,fmt,arg...) \
        do{if (!(cond)) {kprintf(FMT2(fmt),#cond,##arg);while (1);}}while (0)
#   define ERROR(fmt,arg...) \
        do{kprintf(FMT1(fmt),##arg);while(1);}while (0)
#   define BIP(a)   kputc(a);
#   define _p(c) ".set noat\n li $1,'"v(c)"'\n lui $28,0xd020\n sw $1,($28)\n.set at\n"
#endif

// lecture d'un octet ou d'une chaine de caractere depuis le terminal 0
// pour kgets str est un buffer d'au moins size octets
// la lecture du terminal est faite jusqu'au \n qui est mis dans str
// si le buffer est rempli avant d'atteindre le \n les char sont lus mais
// mis a la poubelle. Dans tous les cas un \0 est mis en fin de chaine.

extern char * kgets(char * restrict str, int size);

#ifndef NULL
#define NULL (void *)0
#endif

#endif

