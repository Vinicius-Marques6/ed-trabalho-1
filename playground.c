// C program to illustrate the regexec() function
#include <regex.h>
#include <stdio.h>

void print_result(int value)
{
 
    // If pattern found
    if (value == 0) {
        printf("Pattern found.\n");
    }
 
    // If pattern not found
    else if (value == REG_NOMATCH) {
        printf("Pattern not found.\n");
    }
 
    // If error occurred during Pattern
    // matching
    else {
        printf("An error occurred.\n");
    }
}

int main() {
    // Variable to store initial regex()
    regex_t reegex;
 
    // Variable for return type
    int value;
 
    // Creation of regEx
    value = regcomp( &reegex, "[", 0);

 
    value = regexec( &reegex, "fmunicipio[s", 0, NULL, 0);
 
    // Print the results
    print_result(value);
    return 0;
}