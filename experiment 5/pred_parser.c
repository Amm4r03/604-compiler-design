#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

char input[100];
int pos = 0;
char lookahead;

// Function to advance the input and set the next lookahead character
void nextToken() {
    lookahead = input[pos++];
}

// Function to match a terminal symbol
void match(char expected) {
    if (lookahead == expected)
        nextToken();
    else {
        printf("Syntax Error: Expected '%c', Found '%c'\n", expected, lookahead);
        exit(1);
    }
}

// Forward declarations of parsing functions
void E();
void E_prime();
void T();
void T_prime();
void F();

// Grammar rules

void E() {
    // E → T E'
    T();
    E_prime();
}

void E_prime() {
    // E' → + T E' | ε
    if (lookahead == '+') {
        match('+');
        T();
        E_prime();
    }
    // else epsilon (do nothing)
}

void T() {
    // T → F T'
    F();
    T_prime();
}

void T_prime() {
    // T' → * F T' | ε
    if (lookahead == '*') {
        match('*');
        F();
        T_prime();
    }
    // else epsilon (do nothing)
}

void F() {
    // F → ( E ) | id
    if (lookahead == '(') {
        match('(');
        E();
        match(')');
    } else if (isalnum(lookahead)) {
        match(lookahead);  // match any single letter/digit as id
    } else {
        printf("Syntax Error: Unexpected symbol '%c'\n", lookahead);
        exit(1);
    }
}

int main() {
    printf("Enter expression to parse: ");
    scanf("%s", input);

    nextToken(); // initialize lookahead with first token

    E();

    if (lookahead == '\0') {
        printf("Input parsed successfully: Valid expression.\n");
    } else {
        printf("Syntax Error: Extra input remaining at '%c'\n", lookahead);
    }

    return 0;
}
