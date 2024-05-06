#include <stdlib.h>
#include <stdio.h>
#include "../include/heap.h"

void troca(treg_dist *a, treg_dist *b) {
	treg_dist aux = *a;
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

	if (esq < h->tam && h->vetor[esq].dist > h->vetor[maior].dist)
		maior = esq;
	if (dir < h->tam && h->vetor[dir].dist > h->vetor[maior].dist)
		maior = dir;

	if (maior != n) {
		troca(&h->vetor[n], &h->vetor[maior]);
		desce(h, maior);
	}
}

void sobe(theap *h, int n) {
	int p = pai(n);
    if (h->vetor[p].dist < h->vetor[n].dist) {
		troca(&h->vetor[p], &h->vetor[n]);
		sobe(h, p);
	}
}

void heap_constroi(theap * h, int max){
    h->vetor = (treg_dist *) malloc(max * sizeof(treg_dist));
	if (h->vetor == NULL) {
		printf("Erro ao alocar vetor\n");
		exit(EXIT_FAILURE);
	}
    h->tam = 0;
    h->max = max;
}

int heap_insere(theap * h, void *reg, double dist){
    if (h->tam == h->max)
        return EXIT_FAILURE;
    
    h->vetor[h->tam].reg = reg;
	h->vetor[h->tam].dist = dist;
    sobe(h, h->tam);
    h->tam += 1;

    return EXIT_SUCCESS;
}

void altera_prioridade(theap * h, int n, void * valor, double dist) {
	treg_dist anterior = h->vetor[n];
	h->vetor[n].reg = valor;
	h->vetor[n].dist = dist;

	if (dist > anterior.dist)
		sobe(h, n);
	if (dist < anterior.dist)
		desce(h, n);
}

void heap_apaga(theap * h){
    free(h->vetor);
    h->vetor = NULL;
    h->tam = 0;
    h->max = 0;
}

void heap_sort(theap * h) {
	int n = h->tam;
	for (int i = h->tam - 1; i > 0; i--) {
		troca(&h->vetor[0], &h->vetor[i]);
		h->tam -= 1;
		desce(h, 0);
	}
	h->tam = n;
}