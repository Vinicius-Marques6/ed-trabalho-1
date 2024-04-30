#ifndef __HEAP__
#define __HEAP__
#include <stdint.h>

typedef struct {
    uintptr_t * vetor;
    int tam;
    int max;
    double (*cmp)(void *, void *);
} theap;

void heap_constroi(theap *h, int max, double (*cmp)(void *, void *));
int heap_insere(theap *h, void *reg);
void altera_prioridade(theap *h, int i, void *reg);
void heap_apaga(theap *h);

#endif