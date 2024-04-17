#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../include/hash.h"
#define SEED    0x12345678

uint32_t murmur_hash(const char* str, uint32_t h){
    /* One-byte-at-a-time Murmur hash 
    Source: https://github.com/aappleby/smhasher/blob/master/src/Hashes.cpp */
    for (; *str; ++str) {
        h ^= *str;
        h *= 0x5bd1e995;
        h ^= h >> 15;
    }
    return h;
}

uint32_t valorString(const char * key){
    int i, valor = 7;
    int tam = strlen(key);
    for (i = 0; i < tam; i++){
        valor = 31 * valor + key[i];
    }
    return valor;
}

int chaveDivisao(const char * key, int TABLE_SIZE){
    return valorString(key) % TABLE_SIZE;
}

int hash_duplo(const char * key, int i, int TABLE_SIZE){
    int h1 = murmur_hash(key ,SEED) % TABLE_SIZE; 
    int h2 = chaveDivisao(key, TABLE_SIZE - 2) + 1;
    return (h1 + i*h2) % TABLE_SIZE;
}

int hash_insere(thash * h, void * bucket){
    int tentativas = 0;
    int pos = hash_duplo(h->get_key(bucket), tentativas, h->TABLE_SIZE);
    /*se esta cheio*/
    if (h->TABLE_SIZE == (h->size+1)){
        free(bucket);
        return EXIT_FAILURE;
    }
    /*fazer a insercao*/
    while(h->table[pos] != 0){
        printf("Colisão na posição %d\n", pos);
        if (h->table[pos] == h->deleted)
            break;
        pos = hash_duplo(h->get_key(bucket), ++tentativas, h->TABLE_SIZE);
    }
    printf("Inserindo na posição %d\n", pos);
    h->table[pos] = (uintptr_t) bucket;
    h->size += 1;

    return EXIT_SUCCESS;
}

int hash_constroi(thash * h,int nbuckets, char * (*get_key)(void *) ){
    h->table = calloc(sizeof(void *),nbuckets + 1);
    if (h->table == NULL){
        return EXIT_FAILURE;
    }
    h->TABLE_SIZE = nbuckets + 1;
    h->size = 0;
    h->deleted = (uintptr_t) &(h->size);
    h->get_key = get_key;
    return EXIT_SUCCESS;
}

void * hash_busca(thash h, const char * key){
    int tentativas = 0;
    int pos = hash_duplo(key, tentativas, h.TABLE_SIZE);
    void * ret = NULL;
    while(h.table[pos] != 0 && ret == NULL){
        if (strcmp(h.get_key((void*)h.table[pos]),key) == 0){
            ret = (void *) h.table[pos];
        }else{
            pos = hash_duplo(key, ++tentativas, h.TABLE_SIZE);
        }
    }
    return ret;
}

int hash_remove(thash * h, const char * key){
    int tentativas = 0;
    int pos = hash_duplo(key, tentativas, h->TABLE_SIZE);
    while(h->table[pos]!=0){
        if (strcmp(h->get_key((void*)h->table[pos]),key) == 0){ /* se achei remove*/
            free((void *)h->table[pos]);
            h->table[pos] = h->deleted;
            h->size -= 1;
            return EXIT_SUCCESS;
        }else{
            pos = hash_duplo(key, ++tentativas, h->TABLE_SIZE);
        }
    }
    return EXIT_FAILURE;
}

void hash_apaga(thash *h){
    int pos;
    for (pos = 0; pos < h->TABLE_SIZE; pos++){
        if (h->table[pos] != 0){
            if (h->table[pos]!= h->deleted){
                free((void*) h->table[pos]);
            }
        }
    }
    free(h->table);
}