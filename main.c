#ifdef VERBOSE
#include <time.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "./lib/include/hash.h"
#include "./lib/include/abb.h"
#include "./lib/include/json.h"

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

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
    double dx = ((tmunicipio *) p1)->latitude - ((tmunicipio *) p2)->latitude;
    double dy = ((tmunicipio *) p1)->longitude - ((tmunicipio *) p2)->longitude;
    return dx * dx + dy * dy;
}

int isEqual(void *reg, const char *key) {
    return strcmp(get_cod(reg), key) == 0;
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

void salva(void *reg, void *dest[]) {
    hash_insere((thash *) dest[0], reg);
    hash_insere((thash *) dest[1], reg);
    abb_insere((tarv *) dest[2], reg);
}

void imprime_municipio(tmunicipio *m) {
    if (m == NULL) {
        printf("%sMunicípio não encontrado%s\n", ANSI_COLOR_RED, ANSI_COLOR_RESET);
        return;
    }

    printf("%sCódigo IBGE:%s %s\n",ANSI_COLOR_CYAN,ANSI_COLOR_RESET , m->codigo_ibge);
    printf("%sNome:%s %s\n", ANSI_COLOR_CYAN,ANSI_COLOR_RESET, m->nome);
    printf("%sLatitude:%s %f\n", ANSI_COLOR_CYAN,ANSI_COLOR_RESET, m->latitude);
    printf("%sLongitude:%s %f\n", ANSI_COLOR_CYAN,ANSI_COLOR_RESET, m->longitude);
    printf("%sCapital:%s %d\n", ANSI_COLOR_CYAN,ANSI_COLOR_RESET, m->capital);
    printf("%sCódigo UF:%s %d\n", ANSI_COLOR_CYAN,ANSI_COLOR_RESET, m->codigo_uf);
    printf("%sSiafi ID:%s %d\n", ANSI_COLOR_CYAN,ANSI_COLOR_RESET, m->siafi_id);
    printf("%sDDD:%s %d\n", ANSI_COLOR_CYAN,ANSI_COLOR_RESET, m->ddd);
    printf("%sFuso Horário:%s %s\n", ANSI_COLOR_CYAN,ANSI_COLOR_RESET, m->fuso_horario);
}

#ifdef VERBOSE
void imprime_relatorio(thash hash, char *nome) {
    printf("\n%s--- %s ---%s\n", ANSI_COLOR_YELLOW, nome, ANSI_COLOR_RESET);
    printf("Foram inseridos %d municípios na tabela hash\n", hash.size);
    printf("Foram realizadas %d colisões\n", hash.qtd_colisoes);
    printf("A maior sequência de tentativas foi de %d\n", hash.max_tentativas);
}
#endif

int main() {
    FILE *fmunicipios;
    thash hash_cod, hash_nome;
    tarv arv;
    hash_constroi(&hash_cod, 10006, get_cod);
    hash_constroi(&hash_nome, 10006, get_nome);
    abb_constroi(&arv, cmp, distancia);

    fmunicipios = fopen("municipios2.json", "r");

    if (fmunicipios == NULL) {
        printf("Erro! Não foi possível abrir o arquivo municipios.json\n");
        return EXIT_FAILURE;
    }

    void *dest[] = {&hash_cod, &hash_nome, &arv};
    char *keys[] = {"codigo_ibge", "nome", "latitude", "longitude", "capital", "codigo_uf", "siafi_id", "ddd", "fuso_horario"};
    #ifdef VERBOSE
    clock_t t = clock();
    #endif
    int r = json_parse(fmunicipios, keys, 9, aloca, insere, salva, dest);
    #ifdef VERBOSE
    t = clock() - t;
    double time = ((double) t) / CLOCKS_PER_SEC;
    printf("O hash demorou %f segundos para executar\n", time);
    #endif
    if (r == EXIT_FAILURE) {
        hash_apaga(&hash_cod);
        fclose(fmunicipios);

        printf("Erro ao fazer o parse do arquivo municipios.json\n");
        return EXIT_FAILURE;
    }

    #ifdef VERBOSE
        imprime_relatorio(hash_cod, "Código IBGE");
        imprime_relatorio(hash_nome, "Nome");
    #endif

    for (;;) {
        printf("\nBuscar por:\n"
                "1 - Código IBGE\n"
                "2 - Vizinhos\n"
                "3 - Nome\n"
                "0 - Sair\n");
        int opcao;
        scanf("%d", &opcao);
        if (opcao == 0) {
            break;

        } else if (opcao == 1) {
            printf("Digite o código IBGE do município que deseja buscar: ");
            char codigo_ibge[8];
            scanf("%s", codigo_ibge);
            if (strcmp(codigo_ibge, "0") == 0) {
                break;
            }
            tmunicipio *m = (tmunicipio *) hash_busca(hash_cod, codigo_ibge);
            imprime_municipio(m);
        } else if (opcao == 2) {
            printf("Digite o código IBGE do município que deseja buscar: ");
            char codigo_ibge[8];
            scanf("%s", codigo_ibge);
            printf("Digite a quantidade de vizinhos: ");
            int qtd_vizinhos;
            scanf("%d", &qtd_vizinhos);

            tmunicipio *m = (tmunicipio *) hash_busca(hash_cod, codigo_ibge);
            if (m != NULL) {
                tmunicipio *melhor = (tmunicipio *) abb_busca_prox(&arv, arv.raiz, m, qtd_vizinhos);

                printf("%sCódigo IBGE:%s %s\n",ANSI_COLOR_CYAN,ANSI_COLOR_RESET , melhor->codigo_ibge);
            } else {
                printf("Município não encontrado\n");
            }
            //imprime_municipio(melhor);

        } else if (opcao == 3) {
            printf("Digite o nome do município que deseja buscar: ");
            char nome[100];
            scanf(" %[^\n]", nome);
            printf("Digite a quantidade de vizinhos: ");
            int qtd_vizinhos;
            scanf("%d", &qtd_vizinhos);

            tmunicipio *m = (tmunicipio *) hash_busca(hash_nome, nome);
            if (m != NULL) {
                tmunicipio **melhor = (tmunicipio **) abb_busca_prox(&arv, arv.raiz, m, qtd_vizinhos);
                for (int i = 0; i < qtd_vizinhos; i++) {
                    imprime_municipio(melhor[i]);
                }

                free(melhor);
            } else {
                printf("Município não encontrado\n");
            }    
        } else {
            printf("Opção inválida\n");
        }
    }
    
    printf("Saindo...\n");
    hash_apaga(&hash_cod);
    free(hash_cod.table);
    fclose(fmunicipios);

    return EXIT_SUCCESS;
}
