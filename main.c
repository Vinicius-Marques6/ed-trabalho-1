#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct {
    int codigo_ibge;
    char nome[100];
    float latitude;
    float longitude;
    int capital;
    int codigo_uf;
    int siafi_id;
    int ddd;
    char fuso_horario[100];
} tmunicipio;

#define BUFFER_SIZE 1000

char *ltrim(char *s) {
    while(*s == ' ') s++;
    return s;
}

char *get_value(char *linha) {
    char *value = strchr(linha, ':');
    if (value != NULL) {
        value++;
        value = ltrim(value);
        char *end = strrchr(value, ',');
        if (end != NULL && *(end+1) == '\n') {
            *end = '\0';
        }
    }
    return value;
}

int aloca_municipio(char *linha, FILE *fmunicipios) {
    tmunicipio *municipio = (tmunicipio *) malloc(sizeof(tmunicipio));
    if (municipio == NULL) {
        printf("Erro! Não foi possível alocar memória para o município\n");
        return EXIT_FAILURE;
    }

    municipio->codigo_ibge = atoi(get_value(linha));

    printf("Código IBGE: %d\n", municipio->codigo_ibge);

    free(municipio);
    return EXIT_SUCCESS;
}

int main() {
    FILE *fmunicipios;
    char linha[BUFFER_SIZE];

    fmunicipios = fopen("municipios2.json", "r");

    if (fmunicipios == NULL) {
        printf("Erro! Não foi possível abrir o arquivo municipios.json\n");
        return EXIT_FAILURE;
    }

    while (fgets(linha, BUFFER_SIZE, fmunicipios) != NULL) {
        if(strpbrk(linha, "[]") != NULL) {
            continue;
        }

        if(strpbrk(linha, "{") != NULL) {
            aloca_municipio(linha, fmunicipios);
        }
    }

    fclose(fmunicipios);

    return EXIT_SUCCESS;
}
