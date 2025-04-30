#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define KEYWORD_COUNT 32
const char *keywords[] = {"auto", "break", "case", "char", "const", "continue", "default", "do", "double", "else", "enum", "extern", "float", "for", "goto", "if", "int", "long", "register", "return", "short", "signed", "sizeof", "static", "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while"};

int isKeyword(char *word) {
    for (int i = 0; i < KEYWORD_COUNT; i++) {
        if (strcmp(word, keywords[i]) == 0) return 1;
    }
    return 0;
}

void skipComments(FILE *file) {
    int ch = fgetc(file);
    if (ch == '/') {
        while ((ch = fgetc(file)) != '\n' && ch != EOF);
    } else if (ch == '*') {
        int prev;
        while ((ch = fgetc(file)) != EOF) {
            if (prev == '*' && ch == '/') break;
            prev = ch;
        }
    } else {
        ungetc(ch, file);
        ungetc('/', file);
    }
}

int main() {
    FILE *file = fopen("helloworld.c", "r");
    if (!file) {
        perror("Error");
        return 1;
    }

    int ch;
    while ((ch = fgetc(file)) != EOF) {
        if (isspace(ch)) continue;

        if (ch == '/') {
            skipComments(file);
            continue;
        }

        if (ch == '"' || ch == '\'') {
            char delim = ch;
            putchar(delim);
            while ((ch = fgetc(file)) != delim && ch != EOF) {
                putchar(ch);
                if (ch == '\\') fgetc(file); // Skip escape
            }
            putchar(delim);
            printf(" (String/Char)\n");
            continue;
        }

        if (isalpha(ch) || ch == '_') {
            char buffer[100];
            int i = 0;
            buffer[i++] = ch;
            while ((ch = fgetc(file)) != EOF && (isalnum(ch) || ch == '_')) {
                buffer[i++] = ch;
            }
            buffer[i] = '\0';
            ungetc(ch, file);
            printf("%s: %s\n", isKeyword(buffer) ? "Keyword" : "Identifier", buffer);
            continue;
        }

        if (isdigit(ch)) {
            printf("Constant: %c", ch);
            while ((ch = fgetc(file)) != EOF && isdigit(ch)) putchar(ch);
            ungetc(ch, file);
            printf("\n");
            continue;
        }

        char operators[] = "+-*/%=&|<>!^~?:";
        if (strchr(operators, ch) != NULL) {
            printf("Operator: %c\n", ch);
        } else {
            printf("Punctuation: %c\n", ch);
        }
    }

    fclose(file);
    return 0;
}