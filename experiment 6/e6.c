/*
 * LALR parser implementation for a subset of C
 * This implements a table-driven LALR parser for basic expressions and statements
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Token types
#define ID 1      // identifier
#define NUM 2     // number
#define PLUS 3    // +
#define MINUS 4   // -
#define MULT 5    // *
#define DIV 6     // /
#define ASSIGN 7  // =
#define LPAREN 8  // (
#define RPAREN 9  // )
#define SEMI 10   // ;
#define END 11    // $

// Non-terminals
#define STMT 101
#define EXPR 102
#define TERM 103
#define FACTOR 104

// Actions
#define SHIFT 201
#define REDUCE 202
#define ACCEPT 203
#define ERROR 204

// Max sizes
#define MAX_STACK 100
#define MAX_TOKEN_LEN 100
#define MAX_INPUT 1000

// Token structure
typedef struct {
    int type;
    char lexeme[MAX_TOKEN_LEN];
} Token;

// Action table entry structure
typedef struct {
    int action;
    int value;
} ActionEntry;

// Reduction rule structure
typedef struct {
    int lhs;       // Left-hand side non-terminal
    int num_rhs;   // Number of symbols on right-hand side
} Rule;

// Function prototypes
void parse();
Token get_next_token();
void error(const char *msg);
void print_stack();

// Global variables
Token tokens[MAX_INPUT];      // Input tokens
int token_index = 0;         // Current token index
int stack[MAX_STACK];        // Parser stack for states
int top = -1;                // Stack top index
int token_count = 0;         // Total token count

// LALR parsing tables
// These are simplified and only handle a subset of C expressions
// 0: Error, >0: Shift to state, <0: Reduce by rule

// Action table [state][token] -> (action, value)
ActionEntry action_table[12][12] = {
    // State 0: ID SHIFT 5, NUM SHIFT 6, LPAREN SHIFT 4
    {{SHIFT, 5}, {SHIFT, 6}, {ERROR, 0}, {ERROR, 0}, {ERROR, 0}, {ERROR, 0}, {ERROR, 0}, {SHIFT, 4}, {ERROR, 0}, {ERROR, 0}, {ERROR, 0}},
    
    // State 1: END ACCEPT
    {{ERROR, 0}, {ERROR, 0}, {ERROR, 0}, {ERROR, 0}, {ERROR, 0}, {ERROR, 0}, {ERROR, 0}, {ERROR, 0}, {ERROR, 0}, {ERROR, 0}, {ACCEPT, 0}},
    
    // State 2: SEMI SHIFT 7, PLUS SHIFT 8, MINUS SHIFT 9
    {{ERROR, 0}, {ERROR, 0}, {SHIFT, 8}, {SHIFT, 9}, {ERROR, 0}, {ERROR, 0}, {ERROR, 0}, {ERROR, 0}, {ERROR, 0}, {SHIFT, 7}, {ERROR, 0}},
    
    // State 3: PLUS SHIFT 8, MINUS SHIFT 9, MULT SHIFT 10, DIV SHIFT 11, RPAREN REDUCE 2, SEMI REDUCE 2
    {{ERROR, 0}, {ERROR, 0}, {SHIFT, 8}, {SHIFT, 9}, {SHIFT, 10}, {SHIFT, 11}, {ERROR, 0}, {ERROR, 0}, {REDUCE, 2}, {REDUCE, 2}, {ERROR, 0}},
    
    // State 4: ID SHIFT 5, NUM SHIFT 6, LPAREN SHIFT 4
    {{SHIFT, 5}, {SHIFT, 6}, {ERROR, 0}, {ERROR, 0}, {ERROR, 0}, {ERROR, 0}, {ERROR, 0}, {SHIFT, 4}, {ERROR, 0}, {ERROR, 0}, {ERROR, 0}},
    
    // State 5: PLUS REDUCE 6, MINUS REDUCE 6, MULT REDUCE 6, DIV REDUCE 6, RPAREN REDUCE 6, SEMI REDUCE 6, ASSIGN SHIFT 15
    {{ERROR, 0}, {ERROR, 0}, {REDUCE, 6}, {REDUCE, 6}, {REDUCE, 6}, {REDUCE, 6}, {SHIFT, 15}, {ERROR, 0}, {REDUCE, 6}, {REDUCE, 6}, {ERROR, 0}},
    
    // State 6: PLUS REDUCE 7, MINUS REDUCE 7, MULT REDUCE 7, DIV REDUCE 7, RPAREN REDUCE 7, SEMI REDUCE 7
    {{ERROR, 0}, {ERROR, 0}, {REDUCE, 7}, {REDUCE, 7}, {REDUCE, 7}, {REDUCE, 7}, {ERROR, 0}, {ERROR, 0}, {REDUCE, 7}, {REDUCE, 7}, {ERROR, 0}},
    
    // State 7: END REDUCE 1
    {{ERROR, 0}, {ERROR, 0}, {ERROR, 0}, {ERROR, 0}, {ERROR, 0}, {ERROR, 0}, {ERROR, 0}, {ERROR, 0}, {ERROR, 0}, {ERROR, 0}, {REDUCE, 1}},
    
    // State 8: ID SHIFT 5, NUM SHIFT 6, LPAREN SHIFT 4
    {{SHIFT, 5}, {SHIFT, 6}, {ERROR, 0}, {ERROR, 0}, {ERROR, 0}, {ERROR, 0}, {ERROR, 0}, {SHIFT, 4}, {ERROR, 0}, {ERROR, 0}, {ERROR, 0}},
    
    // State 9: ID SHIFT 5, NUM SHIFT 6, LPAREN SHIFT 4
    {{SHIFT, 5}, {SHIFT, 6}, {ERROR, 0}, {ERROR, 0}, {ERROR, 0}, {ERROR, 0}, {ERROR, 0}, {SHIFT, 4}, {ERROR, 0}, {ERROR, 0}, {ERROR, 0}},
    
    // State 10: ID SHIFT 5, NUM SHIFT 6, LPAREN SHIFT 4
    {{SHIFT, 5}, {SHIFT, 6}, {ERROR, 0}, {ERROR, 0}, {ERROR, 0}, {ERROR, 0}, {ERROR, 0}, {SHIFT, 4}, {ERROR, 0}, {ERROR, 0}, {ERROR, 0}},
    
    // State 11: ID SHIFT 5, NUM SHIFT 6, LPAREN SHIFT 4
    {{SHIFT, 5}, {SHIFT, 6}, {ERROR, 0}, {ERROR, 0}, {ERROR, 0}, {ERROR, 0}, {ERROR, 0}, {SHIFT, 4}, {ERROR, 0}, {ERROR, 0}, {ERROR, 0}}
};

// Goto table [state][non-terminal] -> next state
int goto_table[12][4] = {
    // STMT, EXPR, TERM, FACTOR
    {1, 2, 3, 0},  // State 0
    {0, 0, 0, 0},  // State 1
    {0, 0, 0, 0},  // State 2
    {0, 0, 0, 0},  // State 3
    {0, 12, 3, 0}, // State 4
    {0, 0, 0, 0},  // State 5
    {0, 0, 0, 0},  // State 6
    {0, 0, 0, 0},  // State 7
    {0, 0, 13, 0}, // State 8
    {0, 0, 14, 0}, // State 9
    {0, 0, 0, 16}, // State 10
    {0, 0, 0, 17}  // State 11
};

// Grammar rules
Rule rules[] = {
    {0, 0},          // Rule 0 (dummy)
    {STMT, 2},       // Rule 1: STMT -> EXPR ;
    {EXPR, 1},       // Rule 2: EXPR -> TERM
    {EXPR, 3},       // Rule 3: EXPR -> EXPR + TERM
    {EXPR, 3},       // Rule 4: EXPR -> EXPR - TERM
    {TERM, 1},       // Rule 5: TERM -> FACTOR
    {FACTOR, 1},     // Rule 6: FACTOR -> ID
    {FACTOR, 1},     // Rule 7: FACTOR -> NUM
    {FACTOR, 3},     // Rule 8: FACTOR -> ( EXPR )
    {TERM, 3},       // Rule 9: TERM -> TERM * FACTOR
    {TERM, 3},       // Rule 10: TERM -> TERM / FACTOR
    {STMT, 3}        // Rule 11: STMT -> ID = EXPR
};

int main() {
    char line[200];
    char input[MAX_INPUT] = "";
    
    printf("Enter C expressions (end with 'end'):\n");
    
    // Read input line by line
    while (fgets(line, sizeof(line), stdin)) {
        if (strncmp(line, "end", 3) == 0) break;
        strcat(input, line);
    }
    
    // Tokenize input
    char *p = input;
    while (*p) {
        // Skip whitespace
        while (isspace(*p)) p++;
        if (!*p) break;
        
        // Extract token
        if (isalpha(*p) || *p == '_') {  // Identifier
            int i = 0;
            while (isalnum(*p) || *p == '_') {
                tokens[token_count].lexeme[i++] = *p++;
            }
            tokens[token_count].lexeme[i] = '\0';
            tokens[token_count].type = ID;
        }
        else if (isdigit(*p)) {  // Number
            int i = 0;
            while (isdigit(*p)) {
                tokens[token_count].lexeme[i++] = *p++;
            }
            tokens[token_count].lexeme[i] = '\0';
            tokens[token_count].type = NUM;
        }
        else {  // Special character
            switch (*p) {
                case '+': tokens[token_count].type = PLUS; break;
                case '-': tokens[token_count].type = MINUS; break;
                case '*': tokens[token_count].type = MULT; break;
                case '/': tokens[token_count].type = DIV; break;
                case '=': tokens[token_count].type = ASSIGN; break;
                case '(': tokens[token_count].type = LPAREN; break;
                case ')': tokens[token_count].type = RPAREN; break;
                case ';': tokens[token_count].type = SEMI; break;
                default: p++; continue;  // Skip unrecognized characters
            }
            tokens[token_count].lexeme[0] = *p++;
            tokens[token_count].lexeme[1] = '\0';
        }
        token_count++;
    }
    
    // Add end token
    tokens[token_count].type = END;
    strcpy(tokens[token_count].lexeme, "$");
    token_count++;
    
    // Parse the tokens
    parse();
    
    return 0;
}

void parse() {
    // Initialize the stack with state 0
    stack[++top] = 0;
    
    Token token = tokens[token_index++];
    int state, next_state, rule_index;
    ActionEntry entry;
    
    printf("LALR Parsing Steps:\n");
    printf("-----------------\n");
    
    while (1) {
        state = stack[top];
        entry = action_table[state][token.type - 1];  // Adjust for 0-based indexing
        
        switch (entry.action) {
            case SHIFT:
                printf("SHIFT: Token '%s' to state %d\n", token.lexeme, entry.value);
                stack[++top] = entry.value;
                token = tokens[token_index++];
                break;
                
            case REDUCE:
                rule_index = entry.value;
                printf("REDUCE: By rule %d\n", rule_index);
                
                // Pop right-hand side symbols from stack
                top -= rules[rule_index].num_rhs;
                
                // Push the new state from goto table
                state = stack[top];
                next_state = goto_table[state][rules[rule_index].lhs - 101];  // Adjust non-terminal index
                stack[++top] = next_state;
                
                printf("GOTO: State %d on non-terminal %d\n", next_state, rules[rule_index].lhs);
                break;
                
            case ACCEPT:
                printf("ACCEPT: Parsing completed successfully\n");
                return;
                
            case ERROR:
            default:
                error("Syntax error");
                return;
        }
        
        print_stack();
    }
}

void print_stack() {
    printf("Stack: ");
    for (int i = 0; i <= top; i++) {
        printf("%d ", stack[i]);
    }
    printf("\n");
}

void error(const char *msg) {
    printf("Error: %s\n", msg);
    printf("At token: %s\n", tokens[token_index - 1].lexeme);
    exit(1);
}