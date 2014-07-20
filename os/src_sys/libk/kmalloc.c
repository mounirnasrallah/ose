#include <kstdlib.h>
#include <kstdio.h>
#include <stdint.h>

#define CACHE_LINE  64
#define BLOCK       CACHE_LINE
#define FLOOR(p)    (block_info_t *)(((unsigned long)(p)/(BLOCK))*(BLOCK))
#define CEIL(p)     (block_info_t *)((((unsigned long)(p)+(BLOCK)-1)/(BLOCK))*(BLOCK))
#define DIFF(p1,p2) ((unsigned long)(p1) - (unsigned long)(p2));

struct block_info_s
{
    uint32_t busy:1;        // 1 plein, 0 vide
    uint32_t size:31;       // nombre d'octets entre deux block_info_s
} __attribute__ ((packed));

typedef struct block_info_s block_info_t;

struct heap_s
{
    spinlock_t lock;
    block_info_t *start;    // debut du zone heap (aligne sur CACHE_LINE)
    block_info_t *limit;    // fin du heap (aligne sur CACHE_LINE)
};

static struct heap_s heap;

int   kmalloc_init (void *start, void *limit)
{
    spin_init(&(heap.lock));
    heap.start = CEIL(start);
    heap.limit = FLOOR(limit);
    heap.start->busy = 0;
    heap.start->size = DIFF(heap.limit,heap.start);
    MESSG(STANDARD,"start %p limit %p size %d\n", heap.start, heap.limit,  heap.start->size);
    return 0;
}


static void* try_kmalloc(unsigned size)
{
    block_info_t *oldnext, *newnext, *new = heap.start;

    while ((new != heap.limit) && ((new->busy) || (((new->size) - sizeof(block_info_t)) < size)))
        new = new + (new->size)/sizeof(block_info_t);

    if (new == heap.limit)
        return NULL;

    new->busy = 1;
    oldnext = new + (new->size)/sizeof(block_info_t);
    newnext = CEIL((unsigned long)new + (sizeof(block_info_t) + size));

    if (newnext != oldnext) {
        new->size = DIFF(newnext,new);
        newnext->busy = 0;
        newnext->size = DIFF(oldnext,newnext);
    }
    return (void *)(new + 1);
}

static void kfusion(block_info_t *pt)
{
    for (; pt != heap.limit; pt += (pt->size)/sizeof(block_info_t)) {
        if (pt->busy == 0) {
            block_info_t * next = pt + (pt->size)/sizeof(block_info_t);
            while ((next != heap.limit) && (next->busy == 0)) {
                pt->size += next->size;
                next = pt + (pt->size)/sizeof(block_info_t);
            }
        }
    }
}

void* kmalloc(unsigned size)
{
    spin_lock(&(heap.lock));
    block_info_t *new = try_kmalloc(size);
    spin_unlock(&(heap.lock));
    if (new) return (void *)new;
    spin_lock(&(heap.lock));
    kfusion(heap.start);
    new = try_kmalloc(size);
    spin_unlock(&(heap.lock));
    return new;
}

void  kfree(void *pt)
{
    block_info_t *info = (block_info_t *)pt - 1;
    info->busy = 0;
}
