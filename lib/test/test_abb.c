#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "../include/abb.h"

typedef struct pessoa {
    char nome[50];
    int idade;
} tpessoa;

void * aloca_pessoa(char *nome, int idade) {
    tpessoa * p = malloc(sizeof(tpessoa));
    strcpy(p->nome, nome);
    p->idade = idade;
    return p;
}

double cmp(void *a, void *b, int nivel) {
    return strcmp(((tpessoa *) a)->nome, ((tpessoa *) b)->nome);
}

void test_constroi() {
    tarv arv;
    abb_constroi(&arv, cmp, NULL);
    assert(arv.raiz == NULL);
    assert(arv.cmp == cmp);
    assert(arv.tam == 0);
}

void test_insere() {
    tarv arv;
    abb_constroi(&arv, cmp, NULL);
    void *p = aloca_pessoa("Joao", 20);
    abb_insere(&arv, p);
    assert(arv.raiz != NULL);
    assert(arv.tam == 1);
    assert(strcmp(((tpessoa *) arv.raiz->reg)->nome, "Joao") == 0);
    assert(((tpessoa *) arv.raiz->reg)->idade == 20);

    p = aloca_pessoa("Maria", 30);
    abb_insere(&arv, p);
    assert(arv.tam == 2);
    assert(strcmp(((tpessoa *) arv.raiz->dir->reg)->nome, "Maria") == 0);
    assert(((tpessoa *) arv.raiz->dir->reg)->idade == 30);
}

int main() {
    test_constroi();
    test_insere();

    return EXIT_SUCCESS;
}