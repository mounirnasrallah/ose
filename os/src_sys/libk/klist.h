#ifndef _KLIST_H_
#define _KLIST_H_

//--------------------------------------------------------------------------------------------------
// copyright Ghassan Almaless
//
// TESTS
//  - list_empty(   list_t *root)
//  - list_islast(  list_t *root, list_t *curr)
//  - list_isfirst( list_t *root, list_t *curr)
//
// GET CONTAINER FROM LIST
//  - type_of_container *list_element( list_t *current, type_of_container, list_member_name)
//  - type_of_container *list_first(   list_t *root, type_of_container, list_member_name)
//  - type_of_container *list_last(    list_t *root, type_of_container, list_member_name)
//
// ITERATORS
//  - list_foreach_forward(  list_t *root, list_t *iter)
//  - list_foreach_backward( list_t *root, list_t *iter)
//  - list_foreach(          list_t *root, list_t *iter)
//  - list_t *list_next(     list_t *root, list_t *current);
//  - list_t *list_prev(     list_t *root, list_t *current);
//
// INIT - ADD - REPLACE - UNLINK
//  - void list_root_init( list_t *root);
//  - void list_add(       list_t *root,    list_t *entry);
//  - void list_add_next(  list_t *current, list_t *entry)
//  - void list_add_prev(  list_t *current, list_t *entry)
//  - void list_unlink(    list_t *entry)
//  - void list_replace(   list_t *current, list_t *new)
//  - void list_detach(    list_t *root,    list_t *rootcopy)
//--------------------------------------------------------------------------------------------------

typedef struct list_entry {
    struct list_entry *next, *prev;
} list_t;

// TEST --------------------------------------------------------------------------------------------

#define list_isempty(root)              (((root)->next==NULL)||((root)==((root)->next)))
#define list_islast(root,curr)          ((root)==((curr)->next))
#define list_isfirst(root,curr)         ((root)==((curr)->prev))

// GET CONTAINER FROM LIST -------------------------------------------------------------------------

#define _list_container(curr,type,member) ((type*)((char*)curr - (unsigned long)&((type*)0)->member))
#define list_item(root,type,member)       _list_container(root,type,member)
#define list_first_item(root,type,member) _list_container((root)->next,type,member)
#define list_last_item(root,type,member)  _list_container((root)->prev,type,member)

// ITERATORS ---------------------------------------------------------------------------------------

#define list_foreach_forward(r,i)       list_t *_n_;list_t *i;for(i=(r)->next,_n_=(i)->next;(i)!=(r);i=_n_, _n_=_n_->next)
#define list_foreach_backward(r,i)      list_t *_p_;list_t *i;for(i=(r)->prev,_p_=(i)->prev;(i)!=(r);i=_p_, _p_=_p_->prev)
#define list_foreach(root,iter)         list_foreach_forward(root,iter)
#define list_first(root)                (list_isempty(root)) ? (void*)0 : (root)->next
#define list_last(root)                 (list_isempty(root)) ? (void*)0 : (root)->prev
#define list_next(root,curr)            (list_isempty(root)||list_islast(curr)) ? (void*)0 : (curr)->next
#define list_prev(root,curr)            (list_isempty(root)||list_isfirst(curr)) ? (void*)0 : (curr)->prev

// INIT - LINK - UNLINK ----------------------------------------------------------------------------

#define list_root_init(root)            (root)->next=(root)->prev=(root)

#define LIST_INITIALIZER                {(void *)0,(void *)0}

static inline void list_add(list_t * root, list_t * entry) {
    entry->next = root->next;
    entry->prev = root;
    root->next->prev = entry;
    root->next = entry;
}

#define list_add_next(current,entry)    list_add((current), (entry))
#define list_add_prev(current,entry)    list_add((current)->prev, (entry))

static inline void list_add_first(list_t * root, list_t * entry) {
    if ((root->next)==NULL) list_root_init(root);
    list_add(root, entry);
}
static inline void list_add_last(list_t * root, list_t * entry) {
    if ((root->next)==NULL) list_root_init(root);
    list_add(root->prev, entry);
}

static inline void list_detach(list_t *root, list_t *rootcopy) {
    root->next->prev = root->prev->next = rootcopy;
    rootcopy->next = root->next;
    rootcopy->prev = root->prev;
    list_root_init(root);
}

static inline list_t * list_unlink(list_t * entry) {
    entry->prev->next = entry->next;
    entry->next->prev = entry->prev;
    return entry;
}

static inline void list_replace(list_t * current, list_t * new) {
    new->prev = current->prev;
    new->next = current->next;
    current->prev->next = new;
    current->next->prev = new;
}

#endif
