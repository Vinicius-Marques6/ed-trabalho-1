#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../include/hash.h"

int chaveDivisao(int key, int TABLE_SIZE){
    return key % TABLE_SIZE;
}

uint32_t sti(const char *str) {
    char * resto;
    int n = strtol(str, &resto, 10);
    if (*resto != '\0') {
        uint32_t valor = 0;
        int tam = strlen(str);
        for (int i = 0; i < tam; i++){
            valor = 31 * valor + (int) str[i];
            // valor = ((valor << 5) + valor) + str[i];
        }
        return valor & 0x7FFFFFFF;
    }

    return n;
}

int hash_duplo(const char * key, int i, int TABLE_SIZE){
    int h1 = chaveDivisao(sti(key), TABLE_SIZE);
    int h2 = chaveDivisao(sti(key), TABLE_SIZE - 1) + 1;
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
        if (h->table[pos] == h->deleted)
            break;
        #ifdef VERBOSE
        h->qtd_colisoes += 1;
        #endif
        pos = hash_duplo(h->get_key(bucket), ++tentativas, h->TABLE_SIZE);
    }
    #ifdef VERBOSE
    if (h->max_tentativas < tentativas)
        h->max_tentativas = tentativas;
    #endif
    h->table[pos] = (uintptr_t) bucket;
    h->size += 1;

    return EXIT_SUCCESS;
}

int hash_constroi(thash * h,int nbuckets, char * (*get_key)(void *) ){
    h->table = calloc(nbuckets + 1, sizeof(void *));
    if (h->table == NULL){
        return EXIT_FAILURE;
    }
    h->TABLE_SIZE = nbuckets + 1;
    h->size = 0;
    h->deleted = (uintptr_t) &(h->size);
    #ifdef VERBOSE
    h->qtd_colisoes = 0;
    h->max_tentativas = 0;
    #endif
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

void ** hash_busca_todos(thash h, const char * key){
    int tentativas = 0;
    int pos = hash_duplo(key, tentativas, h.TABLE_SIZE);
    void ** ret = malloc(sizeof(void *));
    ret[0] = NULL;
    int i = 0;
    while(h.table[pos] != 0){
        if (strcmp(h.get_key((void*)h.table[pos]),key) == 0){
            ret[i] = (void *) h.table[pos];
            i++;
            ret = realloc(ret, (i+1)*sizeof(void *));
            ret[i] = NULL;
        }
        pos = hash_duplo(key, ++tentativas, h.TABLE_SIZE);
    }
    return ret;
}

int hash_remove(thash * h, const char * key){
    int tentativas = 0;
    int pos = hash_duplo(key, tentativas, h->TABLE_SIZE);
    while(h->table[pos]!=0){
        if (strcmp(h->get_key((void*)h->table[pos]),key) == 0){ // se achei remove
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
    h->table = NULL;
}