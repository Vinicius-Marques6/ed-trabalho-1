#ifndef __JSON__
#define __JSON__
#include <stdio.h>

char *ltrim(char *s);
int json_parse(FILE * fjson, char *keys[], int n, void (*aloca)(void **reg), void (*insere)(void * reg,char* key, char* value), void (*salva)(void *reg, void *dest[]), void *dest);

#endif