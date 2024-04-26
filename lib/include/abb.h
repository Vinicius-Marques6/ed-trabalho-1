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
    float (*cmp)(void * , void *, int);
    float (*dist)(void *, void *);
}tarv;

void abb_constroi(tarv *parv, float (*cmp)(void *, void *, int), float (*dist)(void *, void *));
int abb_insere(tarv * parv, void * reg);
void * abb_busca(tarv * parv,  void * reg);
void * closest_point(tarv * parv, void * pivo);

#endif