#ifndef __HEAP__
#define __HEAP__

typedef struct {
    void **vetor;
    int tam;
    int max;
    int (*cmp)(void *, void *);
} theap;

#endif