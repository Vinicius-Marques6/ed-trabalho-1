@echo off

gcc -Wall -pedantic -o main.exe ./lib/src/hash.c ./lib/src/json.c  ./main.c -DVERBOSE && main.exe 