#ifndef __HASH__
#define __HASH__
#include <stdint.h>
typedef struct {
    uintptr_t * table;
    int size;
    int TABLE_SIZE;
    uintptr_t deleted;
    #ifdef VERBOSE
    int qtd_colisoes;
    int max_tentativas;
    #endif
    char * (*get_key)(void *);
} thash;

int hash_insere(thash * h, void * bucket);
int hash_constroi(thash * h,int nbuckets, char * (*get_key)(void *) );
void * hash_busca(thash h, const char * key);
void ** hash_busca_todos(thash h, const char * key);
int hash_remove(thash * h, const char * key);
void hash_apaga(thash *h);

#endif