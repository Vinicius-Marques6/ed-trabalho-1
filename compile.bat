@echo off

gcc -Wall -pedantic -o main.exe ./lib/src/hash.c ./lib/src/json.c ./lib/src/abb.c ./main.c -DVERBOSE && main.exe 