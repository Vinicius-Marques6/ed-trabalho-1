#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    int a = 11;
    int  * ptr = &a;

    printf("a = %p\n", &a);
    printf("ptr = %p\n", ptr);
    return 0;
}