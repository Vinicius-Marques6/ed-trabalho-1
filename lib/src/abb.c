#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../include/abb.h"
#include "../include/heap.h"

void abb_constroi(tarv *parv, double (*cmp)(void *, void *, int), double (*dist)(void *, void *)){
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

typedef struct {
    char codigo_ibge[8];
    char nome[100];
    float latitude;
    float longitude;
    int capital;
    int codigo_uf;
    int siafi_id;
    int ddd;
    char fuso_horario[100];
} tmunicipio;

void abb_busca_prox_node(tarv * parv, tnode * pnode, void *reg, int nivel, double *melhor_dist, void **melhor_reg, int *qtd_visitados) {
    if (pnode == NULL) {
        return;
    }

    ++(*qtd_visitados);

    double dist = parv->dist(pnode->reg, reg);
    if (dist > 0 && dist < *melhor_dist) {
        *melhor_dist = dist;
        melhor_reg[0] = pnode->reg;
    }

    tnode * node_prox = NULL;
    tnode * node_contr = NULL;

    if (parv->cmp(pnode->reg, reg, nivel) > 0) {
        node_prox = pnode->esq;
        node_contr = pnode->dir;
    } else {
        node_prox = pnode->dir;
        node_contr = pnode->esq;
    }

    abb_busca_prox_node(parv, node_prox, reg, ++nivel, melhor_dist, melhor_reg, qtd_visitados);
    abb_busca_prox_node(parv, node_contr, reg, ++nivel, melhor_dist, melhor_reg, qtd_visitados);
}

void ** abb_busca_prox(tarv * parv, void * reg, int i) {
    void ** melhores_regs = calloc(i, sizeof(void *));
    double melhor_dist = parv->dist(parv->raiz->reg, reg);
    melhores_regs[0] = parv->raiz->reg;
    int qtd_visitados = 0;
    abb_busca_prox_node(parv, parv->raiz, reg, 0, &melhor_dist, melhores_regs, &qtd_visitados);
    printf("Visitados: %d\n", qtd_visitados);
    return melhores_regs;
}