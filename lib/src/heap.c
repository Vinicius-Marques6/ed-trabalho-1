#include <stdlib.h>
#include <stdio.h>
#include "../include/heap.h"

void troca(int *a, int *b) {
	int aux = *a;
	*a = *b;
	*b = aux;
}

int pai(int n) {
	return (n - 1) / 2;
}


int filho_esq(int n) {
	return (n * 2) + 1;
}

int filho_dir(int n) {
	return (n * 2) + 2;
}

void desce(theap * h, int n) {
	int maior = n;
	int esq = filho_esq(n);
	int dir = filho_dir(n);

	if (esq < h->tam && h->cmp(h->vetor[esq], h->vetor[n]) > 0)
		maior = esq;
	if (dir < h->tam && h->cmp(h->vetor[dir], h->vetor[maior]) > 0)
		maior = dir;

	if (maior != n) {
		troca(&h->vetor[n], &h->vetor[maior]);
		desce(h, maior);
	}
}

void sobe(theap *h, int n) {
	int p = pai(n);
    if (h->cmp(h->vetor[p], h->vetor[n]) < 0) {
		troca(&h->vetor[p], &h->vetor[n]);
		sobe(h, p);
	}
}

void heap_constroi(theap * h, int max, double (*cmp)(void *, void *)){
    h->vetor = calloc(max, sizeof(void *));
    h->tam = 0;
    h->max = max;
    h->cmp = cmp;
}

int heap_insere(theap * h, void *reg){
    if (h->tam == h->max)
        return EXIT_FAILURE;
    
    h->vetor[h->tam] = reg;
    sobe(h, h->tam);
    h->tam += 1;

    return EXIT_SUCCESS;
}

void altera_prioridade(theap * h, int n, void * valor) {
	int anterior = h->vetor[n];
	h->vetor[n] = valor;

	if (h->cmp(valor, anterior) > 0)
		sobe(h, n);
	if (h->cmp(valor, anterior) < 0)
		desce(h, n);
}

void heap_apaga(theap * h){
    free(h->vetor);
    h->vetor = NULL;
    h->tam = 0;
    h->max = 0;
    h->cmp = NULL;
}