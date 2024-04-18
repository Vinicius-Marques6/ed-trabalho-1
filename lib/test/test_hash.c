#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../include/hash.h"

typedef struct pessoa {
    char nome[50];
    int idade;
} tpessoa;

char * get_nome(void * p) {
    return (*((tpessoa *) p)).nome;
}

void * aloca_pessoa(char *nome, int idade) {
    tpessoa * p = malloc(sizeof(tpessoa));
    strcpy(p->nome, nome);
    p->idade = idade;
    return p;
}

void test_constroi() {
    thash h;
    hash_constroi(&h, 10, get_nome);
    assert(h.size == 0);
    assert(h.TABLE_SIZE == 11);
    assert(h.table != NULL);
    assert(h.get_key == get_nome);
    hash_apaga(&h);
}

void test_insere() {
    thash h;
    hash_constroi(&h, 3, get_nome);
    assert(hash_insere(&h, aloca_pessoa("João", 20)) == EXIT_SUCCESS);
    assert(hash_insere(&h, aloca_pessoa("Maria", 30)) == EXIT_SUCCESS);
    assert(hash_insere(&h, aloca_pessoa("José", 40)) == EXIT_SUCCESS);
    assert(hash_insere(&h, aloca_pessoa("Ana", 50)) == EXIT_FAILURE);
    hash_apaga(&h);
}

void test_busca() {
    thash h;
    hash_constroi(&h, 3, get_nome);
    assert(hash_insere(&h, aloca_pessoa("João", 20)) == EXIT_SUCCESS);
    assert(hash_insere(&h, aloca_pessoa("Maria", 30)) == EXIT_SUCCESS);
    assert(hash_insere(&h, aloca_pessoa("José", 40)) == EXIT_SUCCESS);

    assert(((tpessoa *) hash_busca(h, "Maria"))->idade == 30);
    assert(hash_busca(h, "Ana") == NULL);
    hash_apaga(&h);
}

void test_remove() {
    thash h;
    hash_constroi(&h, 10, get_nome);
    assert(hash_insere(&h, aloca_pessoa("João", 20)) == EXIT_SUCCESS);
    assert(hash_insere(&h, aloca_pessoa("Maria", 30)) == EXIT_SUCCESS);
    assert(hash_insere(&h, aloca_pessoa("José", 40)) == EXIT_SUCCESS);

    assert(hash_remove(&h, "Maria") == EXIT_SUCCESS);
    assert(hash_busca(h, "Maria") == NULL);
    
    assert(hash_remove(&h, "Ana") == EXIT_FAILURE);
    hash_apaga(&h);
}

void test_apaga() {
    thash h;
    hash_constroi(&h, 3, get_nome);
    assert(hash_insere(&h, aloca_pessoa("João", 20)) == EXIT_SUCCESS);
    assert(hash_insere(&h, aloca_pessoa("Maria", 30)) == EXIT_SUCCESS);
    assert(hash_insere(&h, aloca_pessoa("José", 40)) == EXIT_SUCCESS);

    hash_apaga(&h);
    assert(h.table == NULL);
}

int main() {
    test_constroi();
    test_insere();
    test_busca();
    test_remove();
    test_apaga();
    return EXIT_SUCCESS;
}