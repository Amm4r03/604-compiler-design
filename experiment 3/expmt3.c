#include <stdio.h>
#include <string.h>
#include <ctype.h>

typedef enum {
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_KEYWORD,
    TOKEN_STRING,
    TOKEN_COMMENT,
    TOKEN_PREPROCESSOR,
    TOKEN_UNKNOWN,
    TOKEN_INCLUDE_FILE,
} TokenType;

typedef struct {
    TokenType type;
    char value[100];
} Token;

const char* keywords[] = {
    "if", "else", "while", "for", "return",
    "int", "float", "char", "void", "include"
};

const char* operators[] = {
    "+", "-", "*", "/", "=", "==", "!=", "<", ">", "<=", ">="
};

const char* comments[] = {
    "//", "/*", "*/"
};

int main(int argc, char* argv[]) {

    // print the command line arguments
    for (int i = 0; i < argc; i++) {
        printf("Argument %d: %s\n", i, argv[i]);
    }

    FILE *input_file, *output_file;

    if (argc < 3) {
        printf("Usage: %s <input_file> [output_file]\n", argv[0]);
        return 1;
    }

    input_file = fopen(argv[1], "r");
    if (input_file == NULL) {
        printf("Error opening input file - %s\n", argv[1]);
        return 1;
    }

    if (argc == 3) {
        output_file = fopen(argv[2], "w");
        if (output_file == NULL) {
            printf("Error opening output file - %s\n", argv[2]);
            fclose(input_file);
            return 1;
        }
    } 

    char line[512];
    while (fgets(line, sizeof(line), input_file) != NULL) {
        char *token = strtok(line, " \t\n");
        while (token != NULL) {
            int is_keyword = 0;
            for (int i = 0; i < sizeof(keywords) / sizeof(keywords[0]); i++) {
                if (strcmp(token, keywords[i]) == 0) {
                    fprintf(output_file, "Keyword: %s\n", token);
                    is_keyword = 1;
                    break;
                }
            }
            if (!is_keyword) {
                int is_operator = 0;
                for (int i = 0; i < sizeof(operators) / sizeof(operators[0]); i++) {
                    if (strcmp(token, operators[i]) == 0) {
                        fprintf(output_file, "Operator: %s\n", token);
                        is_operator = 1;
                        break;
                    }
                }
                if (!is_operator) {
                    int is_comment = 0;
                    for (int i = 0; i < sizeof(comments) / sizeof(comments[0]); i++) {
                        if (strstr(token, comments[i]) != NULL) {
                            fprintf(output_file, "Comment: %s\n", token);
                            is_comment = 1;
                            break;
                        }
                    }
                    if (!is_comment) {
                        if (isdigit(token[0])) {
                            fprintf(output_file, "Number: %s\n", token);
                        } else if (isalpha(token[0])) {
                            fprintf(output_file, "Identifier: %s\n", token);
                        } else {
                            fprintf(output_file, "Unknown: %s\n", token);
                        }
                    }
                }
            }
            token = strtok(NULL, " \t\n");
        }
    }

    fclose(input_file);
    if (output_file != stdout) {
        fclose(output_file);
    }

    return 0;
}
