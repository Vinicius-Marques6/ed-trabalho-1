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

int json_parse(FILE * fjson, char *keys[], int n, void (*aloca)(void **reg), void (*insere)(void * reg,char* key, char* value), void (*salva)(void *reg, void *dest[]), void *dest) {
    char linha[BUFFER_SIZE];
    while (fgets(linha, BUFFER_SIZE, fjson) != NULL)
    {
        // Ignora linhas com comentários
        if(strpbrk(linha, "[]") != NULL) {
            continue;
        }

        // Verifica se a linha contém um objeto
        if (strpbrk(linha, "{") != NULL) {
            // Aloca um novo registro para o objeto
            void *reg = NULL;
            aloca(&reg);
            if (reg == NULL) { 
                return EXIT_FAILURE;
            }
            // Lê as linhas do objeto
            for (int i = 0; i < n; i++) {
                fgets(linha, BUFFER_SIZE, fjson);
                if (strstr(linha, keys[i]) == NULL)
                    return EXIT_FAILURE;

                char *value = strchr(linha, ':');
                if (value != NULL) {
                    value++;
                    value = ltrim(value);
                    char *squote = strchr(value, '"');
                    if (squote != NULL) {
                        value = squote + 1;
                        char *end = strchr(value, '"');
                        if (end != NULL) {
                            *end = '\0';
                        }
                    }
                    char *end = strrchr(value, ',');
                    if (end != NULL) {
                        *end = '\0';
                    }

                    char *barra_inv = strchr(value, '\\');
                    if (barra_inv != NULL) {
                        memmove(barra_inv, barra_inv + 1, strlen(barra_inv));
                    }

                    insere(reg, keys[i], value);
                }
            }
            salva(reg, dest);
        }
    }
    return EXIT_SUCCESS;
}