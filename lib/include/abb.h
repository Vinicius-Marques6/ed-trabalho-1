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
    int tam;
    double (*cmp)(void * , void *, int);
    double (*dist)(void *, void *);
}tarv;

void abb_constroi(tarv *parv, double (*cmp)(void *, void *, int), double (*dist)(void *, void *));
int abb_insere(tarv * parv, void * reg);
void * abb_busca(tarv * parv,  void * reg);
void ** abb_busca_prox(tarv * parv, void * reg, int * i);
void abb_apaga(tarv * parv);

#endif