#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int sti(const char *s) {
    char * resto;
    int n = strtol(s, &resto, 10);
    if (*resto != '\0') {
        int i, valor = 7;
        int tam = strlen(s);
        for (i = 0; i < tam; i++){
            valor = 31 * valor + s[i];
        }
        return valor;
    }

    return n;
}

int main() {
    int a = 11;
    char b[4] = "!";

    printf("%d\n", a);
    printf("%d\n", sti(b));
    return 0;
}