// Write a C Program to implement NFAs that recognize identifiers, constants, and
// operators of the mini language

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STATES 100
#define MAX_TRANSITIONS 100
#define MAX_INPUT 100

typedef struct {
    int from;
    char input;
    int to[MAX_STATES];
    int to_count;
} Transition;

typedef struct {
    int states[MAX_STATES];
    int state_count;
    char inputs[MAX_INPUT];
    int input_count;
    int start_state;
    int accepting_states[MAX_STATES];
    int accepting_count;
    Transition transitions[MAX_TRANSITIONS];
    int transition_count;
} NFA;

void initialize_nfa(NFA *nfa) {
    nfa->state_count = 0;
    nfa->input_count = 0;
    nfa->transition_count = 0;
    nfa->accepting_count = 0;
}

void add_state(NFA *nfa, int state) {
    nfa->states[nfa->state_count++] = state;
}

void add_input(NFA *nfa, char input) {
    nfa->inputs[nfa->input_count++] = input;
}

void add_transition(NFA *nfa, int from, char input, int to) {
    for (int i = 0; i < nfa->transition_count; i++) {
        if (nfa->transitions[i].from == from && nfa->transitions[i].input == input) {
            nfa->transitions[i].to[nfa->transitions[i].to_count++] = to;
            return;
        }
    }
    Transition t;
    t.from = from;
    t.input = input;
    t.to[0] = to;
    t.to_count = 1;
    nfa->transitions[nfa->transition_count++] = t;
}

void set_start_state(NFA *nfa, int start_state) {
    nfa->start_state = start_state;
}

void add_accepting_state(NFA *nfa, int state) {
    nfa->accepting_states[nfa->accepting_count++] = state;
}

int simulate_nfa(NFA *nfa, char *input) {
    int current_states[MAX_STATES];
    int current_count = 1;
    current_states[0] = nfa->start_state;

    for (int i = 0; input[i] != '\0'; i++) {
        char c = input[i];
        int next_states[MAX_STATES];
        int next_count = 0;

        for (int j = 0; j < current_count; j++) {
            int state = current_states[j];
            for (int k = 0; k < nfa->transition_count; k++) {
                if (nfa->transitions[k].from == state && nfa->transitions[k].input == c) {
                    for (int l = 0; l < nfa->transitions[k].to_count; l++) {
                        next_states[next_count++] = nfa->transitions[k].to[l];
                    }
                }
            }
        }

        memcpy(current_states, next_states, sizeof(int) * next_count);
        current_count = next_count;
    }

    for (int i = 0; i < current_count; i++) {
        for (int j = 0; j < nfa->accepting_count; j++) {
            if (current_states[i] == nfa->accepting_states[j]) {
                return 1;
            }
        }
    }

    return 0;
}

int main() {
    NFA nfa;
    initialize_nfa(&nfa);

    int num_states;
    printf("Enter the number of states: ");
    scanf("%d", &num_states);
    for (int i = 0; i < num_states; i++) {
        add_state(&nfa, i);
    }

    int num_inputs;
    printf("Enter the number of input characters: ");
    scanf("%d", &num_inputs);
    for (int i = 0; i < num_inputs; i++) {
        char input;
        printf("Enter input character %d: ", i + 1);
        scanf(" %c", &input);
        add_input(&nfa, input);
    }

    int start_state;
    printf("Enter the start state: ");
    scanf("%d", &start_state);
    set_start_state(&nfa, start_state);

    int num_accepting;
    printf("Enter the number of accepting states: ");
    scanf("%d", &num_accepting);
    for (int i = 0; i < num_accepting; i++) {
        int state;
        printf("Enter accepting state %d: ", i + 1);
        scanf("%d", &state);
        add_accepting_state(&nfa, state);
    }

    int num_transitions;
    printf("Enter the number of transitions: ");
    scanf("%d", &num_transitions);
    for (int i = 0; i < num_transitions; i++) {
        int from, to;
        char input;
        printf("Enter transition (from input to): ");
        scanf("%d %c %d", &from, &input, &to);
        add_transition(&nfa, from, input, to);
    }

    char test_string[100];
    printf("Enter the input string to test: ");
    scanf("%s", test_string);

    if (simulate_nfa(&nfa, test_string)) {
        printf("The input string is accepted by the NFA.\n");
    } else {
        printf("The input string is rejected by the NFA.\n");
    }

    return 0;
}