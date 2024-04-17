#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "../include/json.h"

#define BUFFER_SIZE 1024

char *ltrim(char *s) {
    while(*s == ' ') s++;
    return s;
}

int json_parse(FILE * fjson, char *keys[], int n, void (*aloca)(void **reg), void (*insere)(void * reg,char* key, char* value), void (*salva)(void *reg, void *dest), void *dest) {
    char linha[BUFFER_SIZE];
    while (fgets(linha, BUFFER_SIZE, fjson) != NULL)
    {
        if(strpbrk(linha, "[]") != NULL) {
            continue;
        }

        if (strpbrk(linha, "{") != NULL) {
            void *reg = NULL;
            aloca(&reg);
            if (reg == NULL) {  
                printf("Erro ao alocar registro\n");        
                return EXIT_FAILURE;
            }
            printf("Alocando registro\n");
            for (int i = 0; i < n; i++) {
                fgets(linha, BUFFER_SIZE, fjson);
                if (strstr(linha, keys[i]) == NULL)
                    return EXIT_FAILURE;

                char *value = strchr(linha, ':');
                if (value != NULL) {
                    value++;
                    value = ltrim(value);
                    char *end = strrchr(value, ',');
                    if (end != NULL && *(end+1) == '\n') {
                        *end = '\0';
                    }

                    insere(reg, keys[i], value);
                }               
            }
            salva(reg, dest);
        }
    }
    return EXIT_SUCCESS;
}