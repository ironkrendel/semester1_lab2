#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define OUT 0
#define IN 1

#define SPLITER_STRING "-------------------------------------\n"

bool count_words = false;
bool show_table = false;
bool ignore_register = false;
bool show_all = false;

char c;

int word_count = 0;
int latin_lowercase[26] = {0};
int latin_uppercase[26] = {0};
int ascii_symbols[256] = {0};

bool word_state = OUT;

int main(int argc, char** argv) {
    if (argc <= 1) {
        show_all = true;
    }

    for (int i = 1;i < argc;i++) {
        if (strcmp(argv[i], "-w") == 0 || strcmp(argv[i], "--word") == 0) {
            count_words = true;
        }
        else if (strcmp(argv[i], "--en") == 0) {
            show_table = true;
        }
        else if (strcmp(argv[i], "--ir") == 0) {
            ignore_register = true;
        }
        else if (strcmp(argv[i], "--all") == 0) {
            show_all = true;
        }
    }

    while ((c = getchar()) != EOF) {
        if (count_words) {
            if (c != ' ' && word_state == OUT) {
                word_count++;
                word_state = IN;
            }
            else {
                word_state = OUT;
            }
        }
        if (show_table) {
            if (ignore_register) {

            }
            else {

            }
        }
        if (show_all) {
            ascii_symbols[c]++;
        }
    }

    if (count_words) {
        printf(SPLITER_STRING);
        printf("Total word count - %d\n", word_count);
    }
    if (show_all) {
        printf(SPLITER_STRING);
        for (int i = 0;i < 256;i++) {
            if (ascii_symbols[i]) {
                printf("%d - %d\n", i, ascii_symbols[i]);
            }
        }
    }

    return 0;
}