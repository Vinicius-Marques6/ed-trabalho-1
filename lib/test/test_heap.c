#include <stdlib.h>
#include <string.h>
#include <stdio.h>  
#include <assert.h>

#include "../include/heap.h"

typedef struct pessoa {
    char nome[50];
    int idade;
} tpessoa;

void * aloca_pessoa(char *nome, int idade) {
    tpessoa * p = malloc(sizeof(tpessoa));
    strcpy(p->nome, nome);
    p->idade = idade;
    //printf("%s = %p\n", nome, (void *) p);
    return p;
}

int main() {
    theap h;
    heap_constroi(&h, 10);
    heap_insere(&h, aloca_pessoa("João", 20), 20);
    heap_insere(&h, aloca_pessoa("Maria", 30), 30);
    heap_insere(&h, aloca_pessoa("José", 40), 40);

    assert(((tpessoa *) h.vetor[0].reg)->idade == 40);
    assert(((tpessoa *) h.vetor[1].reg)->idade == 20);
    assert(((tpessoa *) h.vetor[2].reg)->idade == 30);
    assert(h.tam == 3);
    /*printf("\n");
    for (int i = 0; i < h.tam; i++) {
        printf("%s = %p\n", ((tpessoa *) h.vetor[i].reg)->nome, (void *) h.vetor[i].reg);
    }*/

    altera_prioridade(&h, 0, aloca_pessoa("Ana", 50), 50);

    assert(((tpessoa *) h.vetor[0].reg)->idade == 50);
    heap_sort(&h);

    assert(((tpessoa *) h.vetor[0].reg)->idade == 20);

    return EXIT_SUCCESS;
}
