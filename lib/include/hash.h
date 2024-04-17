#ifndef __HASH__
#define __HASH__
#include <stdint.h>
    typedef struct {
         uintptr_t * table;
         int size;
         int TABLE_SIZE;
         uintptr_t deleted;
         char * (*get_key)(void *);
    } thash;

uint32_t murmur_hash(const char* str, uint32_t h);
uint32_t valorString(const char * key);
int chaveDivisao(const char * key, int TABLE_SIZE);
int hash_duplo(const char * key, int i, int TABLE_SIZE);
int hash_insere(thash * h, void * bucket);
int hash_constroi(thash * h,int nbuckets, char * (*get_key)(void *) );
void * hash_busca(thash h, const char * key);
int hash_remove(thash * h, const char * key);
void hash_apaga(thash *h);

#endif