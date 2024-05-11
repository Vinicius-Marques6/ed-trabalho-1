@echo off

echo Compilando...
if "%~1" == "-v" (
    gcc -Wall -pedantic -g -o main.exe ./lib/src/hash.c ./lib/src/json.c ./lib/src/abb.c ./lib/src/heap.c ./main.c -DVERBOSE
) else (
    gcc -Wall -pedantic -g -o main.exe ./lib/src/hash.c ./lib/src/json.c ./lib/src/abb.c ./lib/src/heap.c ./main.c
)

if %ERRORLEVEL% neq 0 (
    echo Houve um erro na compilação.
    pause
    exit /B 1
)

START main.exe municipios.json