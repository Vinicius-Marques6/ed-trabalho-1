#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "./lib/include/hash.h"
#include "./lib/include/json.h"

typedef struct {
    char codigo_ibge[8];
    char nome[100];
    float latitude;
    float longitude;
    int capital;
    int codigo_uf;
    int siafi_id;
    int ddd;
    char fuso_horario[100];
} tmunicipio;

char *get_key(void *reg) {
    return (*((tmunicipio * )reg)).codigo_ibge;
}

int isEqual(void *reg, const char *key) {
    return strcmp(get_key(reg), key) == 0;
}

void aloca(void **municipio) {
    *municipio = (tmunicipio *) malloc(sizeof(tmunicipio));
    assert(*municipio != NULL);
}

void insere(void *municipio, char *key, char *value) {
    tmunicipio *m = (tmunicipio *) municipio;
    if (isEqual(key, "codigo_ibge")) {
        strcpy(m->codigo_ibge, value);
    } else if (isEqual(key, "nome")) {
        strcpy(m->nome, value);
    } else if (isEqual(key, "latitude")) {
        m->latitude = atof(value);
    } else if (isEqual(key, "longitude")) {
        m->longitude = atof(value);
    } else if (isEqual(key, "capital")) {
        m->capital = atoi(value);
    } else if (isEqual(key, "codigo_uf")) {
        m->codigo_uf = atoi(value);
    } else if (isEqual(key, "siafi_id")) {
        m->siafi_id = atoi(value);
    } else if (isEqual(key, "ddd")) {
        m->ddd = atoi(value);
    } else if (isEqual(key, "fuso_horario")) {
        strcpy(m->fuso_horario, value);
    }
}

void salva(void *reg, void *dest) {
    hash_insere((thash *) dest, reg);
}

int main() {
    FILE *fmunicipios;
    thash hash;
    hash_constroi(&hash, 5623, get_key);

    fmunicipios = fopen("municipios2.json", "r");

    if (fmunicipios == NULL) {
        printf("Erro! Não foi possível abrir o arquivo municipios.json\n");
        return EXIT_FAILURE;
    }

    char *keys[] = {"codigo_ibge", "nome", "latitude", "longitude", "capital", "codigo_uf", "siafi_id", "ddd", "fuso_horario"};
    int r = json_parse(fmunicipios, keys, 9, aloca, insere, salva, &hash);
    if (r == EXIT_FAILURE) {
        hash_apaga(&hash);
        fclose(fmunicipios);

        printf("Erro ao fazer o parse do arquivo municipios.json\n");
        return EXIT_FAILURE;
    }

    for (;;) {
        printf("Digite o código IBGE do município que deseja buscar: ");
        char codigo_ibge[8];
        scanf("%s", codigo_ibge);
        if (strcmp(codigo_ibge, "0") == 0) {
            break;
        }
        tmunicipio *m = (tmunicipio *) hash_busca(hash, codigo_ibge);
        if (m != NULL) {
            printf("Código IBGE: %s\n", m->codigo_ibge);
            printf("Nome: %s\n", m->nome);
            printf("Latitude: %f\n", m->latitude);
            printf("Longitude: %f\n", m->longitude);
            printf("Capital: %d\n", m->capital);
            printf("Código UF: %d\n", m->codigo_uf);
            printf("Siafi ID: %d\n", m->siafi_id);
            printf("DDD: %d\n", m->ddd);
            printf("Fuso Horário: %s\n", m->fuso_horario);
        } else {
            printf("Município não encontrado\n");
        }
    }

    hash_apaga(&hash);
    fclose(fmunicipios);

    return EXIT_SUCCESS;
}
