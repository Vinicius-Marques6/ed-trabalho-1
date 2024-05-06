#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../include/abb.h"
#include "../include/heap.h"

void abb_constroi(tarv *parv, double (*cmp)(void *, void *, int), double (*dist)(void *, void *)){
    parv->raiz = NULL;
    parv->cmp = cmp;
    parv->dist = dist;
    parv->tam = 0;
}

int abb_insere_node(tarv * parv, tnode ** ppnode, void *reg, int nivel){
    if (*ppnode == NULL){ // insere
        *ppnode = malloc(sizeof(tnode));
        (*ppnode)->reg = reg;
        (*ppnode)->esq = NULL;
        (*ppnode)->dir = NULL;
        parv->tam++;
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

void abb_busca_prox_node(tarv * parv, tnode * pnode, void *reg, int nivel, theap * melhores_regs, int *qtd_visitados) {
    // Se o nó for nulo, retorna
    if (pnode == NULL) {
        return;
    }

    ++(*qtd_visitados);

    // Calcula a distância entre o nó atual e o nó de referência
    double dist = parv->dist(pnode->reg, reg);
    // Se a distância for maior que 0, insere no heap
    if (dist > 0) {
        int ret = heap_insere(melhores_regs, pnode->reg, dist);
        // Se o heap estiver cheio e a distância for menor que a maior distância do heap, altera a prioridade
        if (ret == EXIT_FAILURE && dist < melhores_regs->vetor[0].dist) {
            altera_prioridade(melhores_regs, 0, pnode->reg, dist);
        }
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

    abb_busca_prox_node(parv, node_prox, reg, ++nivel, melhores_regs, qtd_visitados);
    abb_busca_prox_node(parv, node_contr, reg, ++nivel, melhores_regs, qtd_visitados);
}

void ** abb_busca_prox(tarv * parv, void * reg, int * i) {
    if (*i >= parv->tam) {
        *i = parv->tam - 1;
    }

    void ** ret = malloc(*i * sizeof(void *));
    if (ret == NULL) {
        printf("Erro ao alocar vetor\n");
        exit(EXIT_FAILURE);
    }

    theap melhores_regs;
    heap_constroi(&melhores_regs, *i);
    melhores_regs.vetor[0].dist = INFINITY;
    int qtd_visitados = 0;

    abb_busca_prox_node(parv, parv->raiz, reg, 0, &melhores_regs, &qtd_visitados);

    heap_sort(&melhores_regs);
    for (int j = 0; j < *i; j++) {
        ret[j] = melhores_regs.vetor[j].reg;
    }

    heap_apaga(&melhores_regs);
    //printf("Visitados: %d\n", qtd_visitados);
    return ret;
}

void abb_apaga_node(tarv * parv, tnode * pnode) {
    if (pnode == NULL) {
        return;
    }

    abb_apaga_node(parv, pnode->esq);
    abb_apaga_node(parv, pnode->dir);
    free(pnode);
}

void abb_apaga(tarv * parv) {
    abb_apaga_node(parv, parv->raiz);
    parv->raiz = NULL;
    parv->tam = 0;
}