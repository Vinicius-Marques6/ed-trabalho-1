#!/bin/sh

echo "Compilando..."
verbose="n"
if [ "$1" = "-v" ]; then
    verbose="s"
elif [ "$1" = "--verbose" ]; then
    verbose="s"
else 
    echo "Deseja compilar em modo verbose? (s/n): "
    read verbose
fi

if [ "$verbose" = "s" ]; then
    gcc -Wall -pedantic -g -o ./main.exe ./lib/src/hash.c ./lib/src/json.c ./lib/src/abb.c ./lib/src/heap.c ./main.c -DVERBOSE -lm
else
    gcc -Wall -pedantic -g -o ./main.exe ./lib/src/hash.c ./lib/src/json.c ./lib/src/abb.c ./lib/src/heap.c ./main.c -lm
fi

./main.exe municipios.json