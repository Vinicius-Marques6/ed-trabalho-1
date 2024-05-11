#!/bin/sh

echo "Compilando..."

if [ "$1" = "-v" ]; then
    gcc -Wall -pedantic -g -o ./main.exe ./lib/src/hash.c ./lib/src/json.c ./lib/src/abb.c ./lib/src/heap.c ./main.c -DVERBOSE -lm
else
    gcc -Wall -pedantic -g -o ./main.exe ./lib/src/hash.c ./lib/src/json.c ./lib/src/abb.c ./lib/src/heap.c ./main.c -lm
fi

./main.exe municipios.json