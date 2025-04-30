/*
 * NFA-based recognizer for identifiers, constants, and operators
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Token types
#define IDENTIFIER 1
#define CONSTANT 2
#define OPERATOR 3
#define UNKNOWN 4

// Function prototypes
int recognize_token(char *str);
int is_identifier(char *str);
int is_constant(char *str);
int is_operator(char *str);
void print_token_type(int token_type);

int main() {
    char input[100];
    
    printf("Enter a string to recognize (or 'exit' to quit): ");
    while (scanf("%s", input) && strcmp(input, "exit") != 0) {
        int token_type = recognize_token(input);
        printf("'%s' is a ", input);
        print_token_type(token_type);
        printf("\n\nEnter a string to recognize (or 'exit' to quit): ");
    }
    
    return 0;
}

int recognize_token(char *str) {
    if (is_identifier(str))
        return IDENTIFIER;
    else if (is_constant(str))
        return CONSTANT;
    else if (is_operator(str))
        return OPERATOR;
    else
        return UNKNOWN;
}

// NFA for identifiers: starts with a letter, followed by letters or digits
int is_identifier(char *str) {
    if (!isalpha(str[0]) && str[0] != '_')
        return 0;
    
    for (int i = 1; str[i] != '\0'; i++) {
        if (!isalnum(str[i]) && str[i] != '_')
            return 0;
    }
    
    return 1;
}

// NFA for constants: integer or floating point numbers
int is_constant(char *str) {
    int i = 0;
    int decimal_point = 0;
    
    // Optional sign
    if (str[i] == '+' || str[i] == '-')
        i++;
    
    // Must have at least one digit
    if (!isdigit(str[i]))
        return 0;
    
    // Process remaining characters
    while (str[i] != '\0') {
        if (str[i] == '.') {
            if (decimal_point) // More than one decimal point
                return 0;
            decimal_point = 1;
        } else if (!isdigit(str[i])) {
            return 0;
        }
        i++;
    }
    
    return 1;
}

// NFA for operators: arithmetic, relational, logical
int is_operator(char *str) {
    // Single character operators
    if (strlen(str) == 1) {
        char op = str[0];
        if (op == '+' || op == '-' || op == '*' || op == '/' || 
            op == '%' || op == '=' || op == '<' || op == '>' || 
            op == '&' || op == '|' || op == '!' || op == '^')
            return 1;
    }
    // Two character operators
    else if (strlen(str) == 2) {
        if (strcmp(str, "==") == 0 || strcmp(str, "!=") == 0 ||
            strcmp(str, "<=") == 0 || strcmp(str, ">=") == 0 ||
            strcmp(str, "&&") == 0 || strcmp(str, "||") == 0 ||
            strcmp(str, "++") == 0 || strcmp(str, "--") == 0)
            return 1;
    }
    
    return 0;
}

void print_token_type(int token_type) {
    switch (token_type) {
        case IDENTIFIER:
            printf("IDENTIFIER");
            break;
        case CONSTANT:
            printf("CONSTANT");
            break;
        case OPERATOR:
            printf("OPERATOR");
            break;
        case UNKNOWN:
            printf("UNKNOWN TOKEN");
            break;
    }
}