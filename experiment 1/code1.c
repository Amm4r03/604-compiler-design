// write a C Program to scan and 
// count the number of characters, 
// words, and lines in a file 

#include <stdio.h>

int main(int argc, char* argv[]) {
    // init file pointer and vars
    FILE *file;
    char filename[100], ch;
    int lines = 1, words = 0, characters = 0;

    // assume that the first argument is the filename
    if (argc < 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    // open the file in read mode 
    file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("Could not open file %s\n", argv[1]);
        return 1;
    }

    // read the file character by character
    while ((ch = fgetc(file)) != EOF) {
        characters++; // count characters

        if (ch == '\n') {
            lines++; // count lines
        }

        if (ch == ' ' || ch == '\n' || ch == '\t') {
            words++; // count words
        }
    }

    // if the last character is not a space, count the last word
    if (characters > 0 && ch != ' ' && ch != '\n' && ch != '\t') {
        words++;
    }

    // close the file
    fclose(file);

    // print the counts
    printf("counter stats for %s : \n", argv[1]);
    printf("-----------------------\n");
    printf("Lines: %d\n", lines);
    printf("Words: %d\n", words);
    printf("Characters: %d\n", characters);

    return 0;
}