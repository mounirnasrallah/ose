#ifndef _KSTDLIB_H_
#define _KSTDLIB_H_

#include <limits.h>
#include <ctype.h>

// initialise le heap
// rend un buffer de size octets ou NULL
// libere un buffer alloué par kmalloc

extern int   kmalloc_init (void *start, void *limit);
extern void* kmalloc(unsigned size);                 
extern void  kfree(void *ptr);    

// kmemmove: memory move with possible intersection
// kstrcpy : copy string src in buffer dst;
// kstrcmp : string comparison
// kstrlen : string length
// kstrdup : malloc + strcpy
// katob   : ascii to binary 

extern void * kmemmove(void *dst, const void *src, unsigned size);
extern char * kstrcpy (char *dst, const char *src);
extern int    kstrcmp (const char *s1, const char *s2);
extern int    kstrlen (const char *s);
extern char * kstrdup (const char *s);
extern int    katob(const char *s);

// rand    : return a random number between 0 and RAND_MAX
// srand   : defined a random sequence, default is srand(1)

#define RAND_MAX INT_MAX
extern int krand(void);
extern void ksrand(unsigned seed);

#ifndef NULL
#define NULL (void *)0
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE !FALSE
#endif

#endif
