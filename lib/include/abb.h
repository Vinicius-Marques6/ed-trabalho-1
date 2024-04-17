#ifndef __ABB__
#define __ABB__
#include <stdint.h>

typedef struct _tnode{
    void * reg;
    struct _tnode *esq;
    struct _tnode *dir;
} tnode;

typedef struct _abb{
    tnode * raiz;
    int (*cmp)(void* , void *);
}tarv;

#endif