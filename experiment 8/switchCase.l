%{
#include <stdio.h>
#include <ctype.h>
%}

%%

[a-z]    { putchar(toupper(yytext[0])); }  // Convert lowercase to uppercase
.        { putchar(yytext[0]); }           // Copy other characters as is

%%

int main() {
    yylex();  // Call the lexer to process the input
    return 0;
}
