#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

char *input;
int current_pos = 0;

void error() {
    printf("Syntax error!\n");
    exit(1);
}

void match(char expected) {
    if (input[current_pos] == expected) {
        current_pos++;
    } else {
        error();
    }
}

void E();
void E_prime();
void T();

void E() {
    T();      // Start with a term
    E_prime(); // Followed by E'
}

void E_prime() {
    if (input[current_pos] == '+') {  // Handle addition
        match('+');
        T();
        E_prime();
    }
    // epsilon (empty production) is implicitly handled here
}

void T() {
    if (isdigit(input[current_pos])) {  // If the current token is a digit
        while (isdigit(input[current_pos])) {  // Skip over the entire integer
            current_pos++;
        }
    } else if (input[current_pos] == '(') {  // If the current token is '('
        match('(');
        E();
        match(')');
    } else {
        error();  // Invalid token
    }
}

int main() {
    char buffer[100];
    printf("Enter the expression: ");
    scanf("%s", buffer);
    input = buffer;
    printf("Parsing the input: %s\n", input);

    E();  // Start parsing from the start symbol
    printf("Input successfully parsed!\n");
    
    return 0;
}
