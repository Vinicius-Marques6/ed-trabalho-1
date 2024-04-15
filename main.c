#include<stdio.h>
#include<stdlib.h>

int main() {
    FILE *fptr;
    char line[1000];

    fptr = fopen("municipios.json", "r");

    if (fptr == NULL) {
        printf("Erro! Não foi possível abrir o arquivo municipios.json\n");
        return EXIT_FAILURE;
    }

    while (fgets(line, sizeof(line), fptr)) {
        printf("%s", line);
    }

    fclose(fptr);

    return EXIT_SUCCESS;
}