%{
#include <stdio.h>
#include <ctype.h>

int vowel_count = 0;   // To store the count of vowels
int consonant_count = 0; // To store the count of consonants
%}

%%
[aeiouAEIOU]     { vowel_count++; }    // Rule for vowels
[bcdfghjklmnpqrstvwxyzBCDFGHJKLMNPQRSTVWXYZ] { consonant_count++; } // Rule for consonants
.|\n              { /* Ignore other characters, including spaces and punctuation */ }

%%

int main() {
    printf("Enter a string: ");
    yylex();  // Start lexical analysis

    // After lexical analysis, print the counts
    printf("Number of vowels: %d\n", vowel_count);
    printf("Number of consonants: %d\n", consonant_count);
    return 0;
}
