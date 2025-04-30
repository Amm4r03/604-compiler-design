%{
/* Lexical analyzer for C using Flex */
#include <stdio.h>
#include <string.h>

int line_num = 1;
%}

/* Options */
%option noyywrap

/* Regular Expressions */
DIGIT       [0-9]
LETTER      [a-zA-Z]
ID          {LETTER}({LETTER}|{DIGIT}|_)*
INTEGER     {DIGIT}+
FLOAT       {DIGIT}+\.{DIGIT}+([eE][+-]?{DIGIT}+)?
CHAR        \'(\\.|[^'\\])\'
STRING      \"(\\.|[^"\\])*\"
WHITESPACE  [ \t]+
NEWLINE     \n
SINGLECOMM  "//".*
MULTICOMM   "/*"([^*]|"*"[^/])*"*/"

%%

{WHITESPACE}    { /* Ignore whitespace */ }
{NEWLINE}       { line_num++; /* Count lines */ }
{SINGLECOMM}    { /* Ignore single-line comments */ }
{MULTICOMM}     { /* Ignore multi-line comments */ 
                  /* Count newlines in comment */
                  for(int i = 0; i < yyleng; i++)
                      if(yytext[i] == '\n') line_num++;
                }

"auto"          { printf("KEYWORD: %s\n", yytext); }
"break"         { printf("KEYWORD: %s\n", yytext); }
"case"          { printf("KEYWORD: %s\n", yytext); }
"char"          { printf("KEYWORD: %s\n", yytext); }
"const"         { printf("KEYWORD: %s\n", yytext); }
"continue"      { printf("KEYWORD: %s\n", yytext); }
"default"       { printf("KEYWORD: %s\n", yytext); }
"do"            { printf("KEYWORD: %s\n", yytext); }
"double"        { printf("KEYWORD: %s\n", yytext); }
"else"          { printf("KEYWORD: %s\n", yytext); }
"enum"          { printf("KEYWORD: %s\n", yytext); }
"extern"        { printf("KEYWORD: %s\n", yytext); }
"float"         { printf("KEYWORD: %s\n", yytext); }
"for"           { printf("KEYWORD: %s\n", yytext); }
"goto"          { printf("KEYWORD: %s\n", yytext); }
"if"            { printf("KEYWORD: %s\n", yytext); }
"int"           { printf("KEYWORD: %s\n", yytext); }
"long"          { printf("KEYWORD: %s\n", yytext); }
"register"      { printf("KEYWORD: %s\n", yytext); }
"return"        { printf("KEYWORD: %s\n", yytext); }
"short"         { printf("KEYWORD: %s\n", yytext); }
"signed"        { printf("KEYWORD: %s\n", yytext); }
"sizeof"        { printf("KEYWORD: %s\n", yytext); }
"static"        { printf("KEYWORD: %s\n", yytext); }
"struct"        { printf("KEYWORD: %s\n", yytext); }
"switch"        { printf("KEYWORD: %s\n", yytext); }
"typedef"       { printf("KEYWORD: %s\n", yytext); }
"union"         { printf("KEYWORD: %s\n", yytext); }
"unsigned"      { printf("KEYWORD: %s\n", yytext); }
"void"          { printf("KEYWORD: %s\n", yytext); }
"volatile"      { printf("KEYWORD: %s\n", yytext); }
"while"         { printf("KEYWORD: %s\n", yytext); }

{ID}            { printf("IDENTIFIER: %s\n", yytext); }
{INTEGER}       { printf("INTEGER: %s\n", yytext); }
{FLOAT}         { printf("FLOAT: %s\n", yytext); }
{CHAR}          { printf("CHAR: %s\n", yytext); }
{STRING}        { printf("STRING: %s\n", yytext); }

"+"             { printf("OPERATOR: %s\n", yytext); }
"-"             { printf("OPERATOR: %s\n", yytext); }
"*"             { printf("OPERATOR: %s\n", yytext); }
"/"             { printf("OPERATOR: %s\n", yytext); }
"%"             { printf("OPERATOR: %s\n", yytext); }
"++"            { printf("OPERATOR: %s\n", yytext); }
"--"            { printf("OPERATOR: %s\n", yytext); }
"="             { printf("OPERATOR: %s\n", yytext); }
"+="            { printf("OPERATOR: %s\n", yytext); }
"-="            { printf("OPERATOR: %s\n", yytext); }
"*="            { printf("OPERATOR: %s\n", yytext); }
"/="            { printf("OPERATOR: %s\n", yytext); }
"%="            { printf("OPERATOR: %s\n", yytext); }
"=="            { printf("OPERATOR: %s\n", yytext); }
"!="            { printf("OPERATOR: %s\n", yytext); }
">"             { printf("OPERATOR: %s\n", yytext); }
"<"             { printf("OPERATOR: %s\n", yytext); }
">="            { printf("OPERATOR: %s\n", yytext); }
"<="            { printf("OPERATOR: %s\n", yytext); }
"&&"            { printf("OPERATOR: %s\n", yytext); }
"||"            { printf("OPERATOR: %s\n", yytext); }
"!"             { printf("OPERATOR: %s\n", yytext); }
"&"             { printf("OPERATOR: %s\n", yytext); }
"|"             { printf("OPERATOR: %s\n", yytext); }
"^"             { printf("OPERATOR: %s\n", yytext); }
"<<"            { printf("OPERATOR: %s\n", yytext); }
">>"            { printf("OPERATOR: %s\n", yytext); }

"("             { printf("PUNCTUATOR: %s\n", yytext); }
")"             { printf("PUNCTUATOR: %s\n", yytext); }
"{"             { printf("PUNCTUATOR: %s\n", yytext); }
"}"             { printf("PUNCTUATOR: %s\n", yytext); }
"["             { printf("PUNCTUATOR: %s\n", yytext); }
"]"             { printf("PUNCTUATOR: %s\n", yytext); }
","             { printf("PUNCTUATOR: %s\n", yytext); }
"."             { printf("PUNCTUATOR: %s\n", yytext); }
";"             { printf("PUNCTUATOR: %s\n", yytext); }
":"             { printf("PUNCTUATOR: %s\n", yytext); }

.               { printf("UNKNOWN: %s\n", yytext); }

%%

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <input_file>\n", argv[0]);
        return 1;
    }
    
    FILE *file = fopen(argv[1], "r");
    if (!file) {
        printf("Could not open file %s\n", argv[1]);
        return 1;
    }
    
    yyin = file;
    printf("LEXICAL ANALYSIS USING FLEX\n");
    printf("---------------------------\n");
    yylex();
    
    printf("\nTotal lines: %d\n", line_num);
    fclose(file);
    return 0;
}