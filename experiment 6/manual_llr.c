#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define STACK_SIZE 100

enum TokenType { ID = 0, ASSIGN, PLUS, SEMICOLON, DOLLAR };

const char* token_str[] = { "ID", "=", "+", ";", "$" };

// Rule: lhs, number of RHS symbols
typedef struct {
    const char *lhs;
    int rhs_len;
} Rule;

Rule rules[] = {
    {"S'", 1},    // 0: S' → S
    {"S", 4},     // 1: S → ID = E ;
    {"E", 3},     // 2: E → E + ID
    {"E", 1},     // 3: E → ID
};

// ACTION TABLE: [state][token]
const char *action[8][5] = {
/* ID    =     +     ;     $  */
/* 0 */ {"s5", "",   "",   "",   ""   },
/* 1 */ {""  , "",   "",   "",   "acc"},
/* 2 */ {""  , "",   "s6", "r3", "r3" },
/* 3 */ {"s5", "",   "",   "",   ""   },
/* 4 */ {""  , "",   "r2", "r2", "r2" },
/* 5 */ {""  , "s3", "",   "",   ""   },
/* 6 */ {"s5", "",   "",   "",   ""   },
/* 7 */ {""  , "",   "",   "r1", "r1" }
};

// GOTO TABLE: [state][non-terminal S=0, E=1]
int go_to[8][2] = {
/* S  E */
/* 0 */ {1, 2},
/* 1 */ {-1, -1},
/* 2 */ {-1, -1},
/* 3 */ {-1, 4},
/* 4 */ {-1, -1},
/* 5 */ {-1, 6},
/* 6 */ {-1, 4},
/* 7 */ {-1, -1}
};

int stack[STACK_SIZE];
int top = -1;

void push(int x) {
    stack[++top] = x;
}
int pop() {
    return stack[top--];
}
int peek() {
    return stack[top];
}

// Tokenizer
int tokenize(const char *input, int tokens[]) {
    int i = 0, k = 0;
    while (input[i]) {
        if (isspace(input[i])) {
            i++;
            continue;
        }
        if (isalpha(input[i])) {
            while (isalnum(input[i])) i++;
            tokens[k++] = ID;
        } else if (input[i] == '=') {
            tokens[k++] = ASSIGN;
            i++;
        } else if (input[i] == '+') {
            tokens[k++] = PLUS;
            i++;
        } else if (input[i] == ';') {
            tokens[k++] = SEMICOLON;
            i++;
        } else {
            printf("Unknown character: %c\n", input[i]);
            exit(1);
        }
    }
    tokens[k++] = DOLLAR;
    return k;
}

// Parser
void parse(int tokens[], int len) {
    int ip = 0;
    top = -1;
    push(0);

    while (1) {
        int state = peek();
        int current_token = tokens[ip];
        const char *act = action[state][current_token];

        printf("State: %d, Token: %s, Action: %s\n", state, token_str[current_token], act);

        if (act[0] == 's') {
            int next_state = atoi(&act[1]);
            push(current_token);  // push token
            push(next_state);     // push state
            ip++;
        } else if (act[0] == 'r') {
            int rule_no = atoi(&act[1]);
            int pop_count = rules[rule_no].rhs_len * 2;
            while (pop_count--) pop();

            int prev_state = peek();
            int goto_state = -1;
            int nt_index = -1;

            if (strcmp(rules[rule_no].lhs, "S") == 0) nt_index = 0;
            else if (strcmp(rules[rule_no].lhs, "E") == 0) nt_index = 1;

            if (nt_index >= 0)
                goto_state = go_to[prev_state][nt_index];

            if (goto_state == -1) {
                printf("Goto error\n");
                return;
            }

            push(nt_index);      // Push non-terminal index for bookkeeping
            push(goto_state);    // Push goto state
            printf("Reduced by rule %d: %s → ...\n", rule_no, rules[rule_no].lhs);
        } else if (strcmp(act, "acc") == 0) {
            printf("Input accepted: valid statement.\n");
            return;
        } else {
            printf("Syntax error!\n");
            return;
        }
    }
}

int main() {
    char input[256];
    int tokens[MAX_TOKENS];

    printf("Enter a C-like expression (e.g., x = y + z;):\n> ");
    fgets(input, sizeof(input), stdin);

    int len = tokenize(input, tokens);
    printf("Tokens: ");
    for (int i = 0; i < len; i++) printf("%s ", token_str[tokens[i]]);
    printf("\n");

    parse(tokens, len);
    return 0;
}
