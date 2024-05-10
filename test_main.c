#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#include "./lib/include/hash.h"
#include "./lib/include/abb.h"
#include "./lib/include/json.h"

typedef struct {
    char codigo_ibge[8];
    char nome[100];
    double latitude;
    double longitude;
    int capital;
    int codigo_uf;
    int siafi_id;
    int ddd;
    char fuso_horario[100];
} tmunicipio;

char *get_cod(void *reg) {
    return ((tmunicipio * )reg)->codigo_ibge;
}

char *get_nome(void *reg) {
    return ((tmunicipio * )reg)->nome;
}

double cmp(void *reg1, void *reg2, int nivel) {
    if (nivel % 2 == 0) {
        return ((tmunicipio *) reg1)->latitude - ((tmunicipio *) reg2)->latitude;
    } else {
        return ((tmunicipio *) reg1)->longitude - ((tmunicipio *) reg2)->longitude;
    }
}

double distancia(void *p1, void *p2) {
    double dx = cmp(p1, p2, 0);
    double dy = cmp(p1, p2, 1);
    return dx * dx + dy * dy;
}

int isEqual(void *reg, const char *key) {
    return strcmp(get_cod(reg), key) == 0;
}

int utf8_strlen(const char *s) {
    int i = 0, j = 0;
    while (s[i]) {
        if ((s[i] & 0xc0) != 0x80)
            j++;
        i++;
    }
    return j;
}

void aloca(void **municipio) {
    *municipio = (tmunicipio *) malloc(sizeof(tmunicipio));
    if (*municipio == NULL) {
        printf("Erro ao alocar memória\n");
        exit(EXIT_FAILURE);
    }
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

void salva(void *reg, void *dest[]) {
    hash_insere((thash *) dest[0], reg);
    hash_insere((thash *) dest[1], reg);
    abb_insere((tarv *) dest[2], reg);
}

int int_len(int n) {
    if (n == 0) {
        return 1;
    }

    int len = 0;
    while (n > 0) {
        n /= 10;
        len++;
    }
    return len;
}

int main() {
    FILE *fmunicipios;
    thash hash_cod, hash_nome;
    tarv arv;
    hash_constroi(&hash_cod, 10006, get_cod);
    hash_constroi(&hash_nome, 10006, get_nome);
    abb_constroi(&arv, cmp, distancia);

    fmunicipios = fopen("municipios.json", "r");
    if (fmunicipios == NULL) {
        return EXIT_FAILURE;
    }

    void *dest[] = {&hash_cod, &hash_nome, &arv};
    char *keys[] = {"codigo_ibge", "nome", "latitude", "longitude", "capital", "codigo_uf", "siafi_id", "ddd", "fuso_horario"};
    int r = json_parse(fmunicipios, keys, 9, aloca, insere, salva, dest);

    if (r == EXIT_FAILURE) {
        hash_apaga(&hash_cod);
        fclose(fmunicipios);

        return EXIT_FAILURE;
    }
    fclose(fmunicipios);

    printf("Municípios carregados: %d\n", hash_cod.size);
    // Teste de busca por código IBGE
    tmunicipio *m = (tmunicipio *) hash_busca(hash_cod, "5003306");
    assert(strcmp(m->nome, "Coxim") == 0);
    m = (tmunicipio *) hash_busca(hash_cod, "123");
    assert(m == NULL);

    printf("Busca por código IBGE: OK\n");

    // Teste de busca dos municipios próximos, usando código IBGE
    m = (tmunicipio *) hash_busca(hash_cod, "5002704");
    int qtd_vizinhos = 5;
    assert(strcmp(m->nome, "Campo Grande") == 0);
    tmunicipio **proximos = (tmunicipio **) abb_busca_prox(&arv, m, &qtd_vizinhos);

    assert(strcmp(proximos[0]->codigo_ibge, "5008008") == 0); // Terenos
    assert(strcmp(proximos[1]->codigo_ibge, "5004908") == 0); // Jaraguari
    assert(strcmp(proximos[2]->codigo_ibge, "5007505") == 0); // Rochedo
    assert(strcmp(proximos[3]->codigo_ibge, "5001508") == 0); // Bandeirantes
    assert(strcmp(proximos[4]->codigo_ibge, "5007901") == 0); // Sidrolândia
    free(proximos);

    printf("Busca de vizinhos por código IBGE: OK\n");

    // Teste de busca dos municipios próximos, usando nome
    tmunicipio ** m2 = (tmunicipio **) hash_busca_todos(hash_nome, "Campo Grande");
    assert(strcmp(m2[0]->codigo_ibge, "2701506") == 0);
    assert(strcmp(m2[1]->codigo_ibge, "5002704") == 0);
    assert(m2[2] == NULL);
    
    proximos = (tmunicipio **) abb_busca_prox(&arv, m2[1], &qtd_vizinhos);

    assert(strcmp(proximos[0]->codigo_ibge, "5008008") == 0); // Terenos
    assert(strcmp(proximos[1]->codigo_ibge, "5004908") == 0); // Jaraguari
    assert(strcmp(proximos[2]->codigo_ibge, "5007505") == 0); // Rochedo
    assert(strcmp(proximos[3]->codigo_ibge, "5001508") == 0); // Bandeirantes
    assert(strcmp(proximos[4]->codigo_ibge, "5007901") == 0); // Sidrolândia
    free(proximos);

    printf("Busca de vizinhos por nome: OK\n");

    hash_apaga(&hash_cod);
    free(hash_nome.table);
    abb_apaga(&arv);
    
    return EXIT_SUCCESS;
}