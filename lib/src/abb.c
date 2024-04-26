#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../include/abb.h"

void abb_constroi(tarv *parv, float (*cmp)(void *, void *, int), float (*dist)(void *, void *)){
    parv->raiz = NULL;
    parv->cmp = cmp;
    parv->dist = dist;
}

int abb_insere_node(tarv * parv, tnode ** ppnode, void *reg, int nivel){
    if (*ppnode == NULL){ // insere
        *ppnode = malloc(sizeof(tnode));
        (*ppnode)->reg = reg;
        (*ppnode)->esq = NULL;
        (*ppnode)->dir = NULL;
        return EXIT_SUCCESS;
    } else if (parv->cmp((*ppnode)->reg, reg, nivel) > 0){ // esquerda
        return abb_insere_node(parv,&((*ppnode)->esq), reg, ++nivel);
    } else { // direita
        return abb_insere_node(parv,&((*ppnode)->dir), reg, ++nivel);
    }
    return EXIT_FAILURE;
}

int abb_insere(tarv * parv, void * reg){
    return abb_insere_node(parv,&parv->raiz,reg, 0);
}

void * abb_busca_node(tarv * parv, tnode * pnode, void *reg, int nivel){
    tnode * ret;
    if (pnode == NULL){ // não encontrou
        ret = NULL;
    }else if (parv->cmp(pnode->reg,reg, nivel) > 0){ // esquerda
        ret = abb_busca_node(parv,pnode->esq,reg, ++nivel);
    }else if (parv->cmp(pnode->reg,reg, nivel) < 0) { // direita
        ret = abb_busca_node(parv,pnode->dir,reg, ++nivel);
    }else{ // encontrou
        ret = pnode->reg;
    }
    return ret;
}

void * abb_busca(tarv * parv, void * reg){
    return abb_busca_node(parv,parv->raiz,reg, 0);
}