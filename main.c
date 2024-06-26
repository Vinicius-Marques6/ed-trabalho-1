#ifdef VERBOSE
#include <time.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "./lib/include/hash.h"
#include "./lib/include/abb.h"
#include "./lib/include/json.h"

/* ANSI escape codes
RED     "\x1b[31m"
GREEN   "\x1b[32m"
YELLOW  "\x1b[33m"
BLUE    "\x1b[34m"
MAGENTA "\x1b[35m"
CYAN    "\x1b[36m"
RESET   "\x1b[0m"
*/

#define BOLD "\x1b[1m"
#define NO_BOLD "\x1b[22m"
#define COR_BARRA "\x1b[33m"
#define COR_TITULO "\x1b[34m"
#define COR_AVISO "\x1b[33m"
#define COR_ERRO "\x1b[31m"
#define COR_RESET "\x1b[0m"

#define Coluna(...) printf("%s|%s", COR_BARRA, COR_RESET);\
printf(__VA_ARGS__);

#define Error(x) printf("%s%s%s", COR_ERRO, x, COR_RESET);

#define NAO_ENCONTRADO "Município não encontrado\n"

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

void imprime_cabecalho(int nome_len, int fuso_len) {
    Coluna("%s Cód. IBGE ", COR_TITULO);
    Coluna("%s %*s ", COR_TITULO, nome_len, "Nome");
    Coluna("%s   Latitude ", COR_TITULO);
    Coluna("%s  Longitude ", COR_TITULO);
    Coluna("%s Capital ", COR_TITULO);
    Coluna("%s Cód. UF ", COR_TITULO);
    Coluna("%s Siafi ID ", COR_TITULO);
    Coluna("%s DDD ", COR_TITULO);
    Coluna("%s  %*s ", COR_TITULO, fuso_len, "Fuso Horário");
    Coluna("\n");
}

void imprime_linha(tmunicipio * m, int nome_len, int fuso_len) {
    Coluna(" %*s ", 9, m->codigo_ibge);
    Coluna(" %*s ", nome_len, m->nome);
    Coluna(" %*f ", 10, m->latitude);
    Coluna(" %*f ", 10, m->longitude);
    Coluna(" %*d ", 7, m->capital);
    Coluna(" %*d ", 7, m->codigo_uf);
    Coluna(" %*d ", 8, m->siafi_id);
    Coluna(" %*d ", 3, m->ddd);
    Coluna(" %*s ", fuso_len, m->fuso_horario);
    Coluna("\n");
}

void imprime_municipio(tmunicipio *m) {
    if (m == NULL) {
        Error(NAO_ENCONTRADO);
        return;
    }

    int nome_len = utf8_strlen(m->nome);
    int fuso_len = utf8_strlen(m->fuso_horario);

    imprime_cabecalho(nome_len, fuso_len);
    imprime_linha(m, nome_len, fuso_len);
}

void imprime_municipios(tmunicipio * ref, tmunicipio *m[], int i) {
    if (m == NULL) {
        Error(NAO_ENCONTRADO);
        return;
    }

    int nome_len = 0;
    int fuso_len = 0;

    for (int j = 0; j < i; j++) {
        int len = utf8_strlen(m[j]->nome);
        if (len > nome_len) {
            nome_len = len;
        }

        len = utf8_strlen(m[j]->fuso_horario);
        if (len > fuso_len) {
            fuso_len = len;
        }
    }

    int i_len = int_len(i);
    for(int j = 0; j <= i_len; j++) {
        printf(" ");
    }
    if (ref != NULL) {
        Coluna("%s Distância ", COR_TITULO);
    }
    imprime_cabecalho(nome_len, fuso_len);

    for (int j = 0; j < i; j++) {
        int nome_diff = strlen(m[j]->nome) - utf8_strlen(m[j]->nome);
        int fuso_diff = strlen(m[j]->fuso_horario) - utf8_strlen(m[j]->fuso_horario);

        printf("%*d ", i_len,j + 1);
        if (ref != NULL) {
            Coluna(" %*.2fkm ", 7, sqrt(distancia(ref, m[j])) * 100);
        }
        imprime_linha(m[j], nome_len + nome_diff, fuso_len + fuso_diff);
    }
}

#ifdef VERBOSE
void imprime_relatorio(thash hash, char *nome) {
    printf("\n%s--- %s ---%s\n", COR_AVISO, nome, COR_RESET);
    printf("Foram inseridos %d municípios na tabela hash\n", hash.size);
    printf("Foram realizadas %d colisões\n", hash.qtd_colisoes);
    printf("A maior sequência de tentativas foi de %d\n", hash.max_tentativas);
}
#endif

int main(int argc, char *argv[]) {
    char *filename = "./municipios.json";
    if (argc != 1) {
        filename = argv[1];
    }

    FILE *fmunicipios;
    thash hash_cod, hash_nome;
    tarv arv;
    hash_constroi(&hash_cod, 10006, get_cod);
    hash_constroi(&hash_nome, 10006, get_nome);
    abb_constroi(&arv, cmp, distancia);

    fmunicipios = fopen(filename, "r");

    if (fmunicipios == NULL) {
        printf("%sErro! Não foi possível abrir o arquivo %s\n", COR_ERRO, filename);
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

        printf("Erro ao fazer o parse do arquivo %s\n", filename);
        return EXIT_FAILURE;
    }
    fclose(fmunicipios);

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
                continue;
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

            if (qtd_vizinhos <= 0) {
                printf("Quantidade de vizinhos inválida\n");
                continue;
            }

            tmunicipio *m = (tmunicipio *) hash_busca(hash_cod, codigo_ibge);
            if (m != NULL) {
                tmunicipio **melhor = (tmunicipio **) abb_busca_prox(&arv, m, &qtd_vizinhos);

                int i_len = int_len(qtd_vizinhos);
                for(int j = 0; j <= i_len; j++) {
                    printf(" ");
                }
                Coluna("%s Cód. IBGE %s", COR_TITULO, COR_RESET);
                Coluna("\n");
                for (int i = 0; i < qtd_vizinhos; i++) {
                    printf("%*d ", i_len, i + 1);
                    Coluna(" %*s ", 9, melhor[i]->codigo_ibge);
                    Coluna("\n");
                }
                
                free(melhor);
            } else {
                printf(NAO_ENCONTRADO);
            }
        } else if (opcao == 3) {
            printf("Digite o nome do município que deseja buscar: ");
            char nome[100];
            scanf(" %[^\n]", nome);

            printf("Digite a quantidade de vizinhos: ");
            int qtd_vizinhos;
            scanf("%d", &qtd_vizinhos);

            if (qtd_vizinhos <= 0) {
                printf("Quantidade de vizinhos inválida\n");
                continue;
            }

            tmunicipio **m = (tmunicipio **) hash_busca_todos(hash_nome, nome);
            if (m[0] != NULL) {
                int selecionado = 0;
                if (m[1] != NULL) {
                    printf("Foram encontrados vários municípios com o nome %s%s%s%s%s\n", BOLD,COR_AVISO, nome, COR_RESET, NO_BOLD);
                    printf("Selecione o município desejado:\n");
                    int i;
                    for (i = 0; m[i] != NULL; i++);

                    imprime_municipios(NULL, m, i);

                    scanf("%d", &selecionado);
                    selecionado--;
                    if (selecionado < 0 || selecionado >= i) {
                        printf("Opção inválida\n");
                        free(m);
                        continue;
                    }
                }

                tmunicipio **melhor = (tmunicipio **) abb_busca_prox(&arv, m[selecionado], &qtd_vizinhos);

                imprime_municipios(m[selecionado], melhor, qtd_vizinhos);

                free(melhor);
            } else {
                Error(NAO_ENCONTRADO);
            }
            free(m);
        } else if (opcao == 4) {
            FILE *snoopy = fopen("snoopy.txt", "r");
            char linha[1024];

            while (fgets(linha, 1024, snoopy)) {
                printf("%s", linha);
            }
            fclose(snoopy);
        } else if (opcao == 5) {
            FILE *takashi = fopen("takashi.txt", "r");
            char linha[1024];

            while (fgets(linha, 1024, takashi)) {
                printf("%s", linha);
            }
            fclose(takashi);
        } else {
            printf("Opção inválida\n");
        }
    }
    
    hash_apaga(&hash_cod);
    free(hash_nome.table);
    abb_apaga(&arv);
    printf("Saindo...\n");

    return EXIT_SUCCESS;
}