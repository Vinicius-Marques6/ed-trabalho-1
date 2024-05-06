#ifndef __HEAP__
#define __HEAP__
#include <stdint.h>

typedef struct {
    void * reg;
    double dist;
} treg_dist;

typedef struct {
    treg_dist * vetor;
    int tam;
    int max;
} theap;

void heap_constroi(theap *h, int max);
int heap_insere(theap *h, void *reg, double dist);
void altera_prioridade(theap *h, int i, void *reg, double dist);
void heap_apaga(theap *h);
void heap_sort(theap * h);

#endif