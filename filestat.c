#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#define OUT 0
#define IN 1

#define SPLITER_STRING "-------------------------------------\n"

bool count_words = false;
bool show_table = false;
bool ignore_register = false;
bool show_all = false;
bool file_passed = false;

char c;

int word_count = 0;
int latin_lowercase[26] = {0};
int latin_uppercase[26] = {0};
int ascii_symbols[256] = {0};

bool word_state = OUT;

char* filepath = NULL;
int file_index;

int main(int argc, char** argv) {
    int _argc = 0;
    
    for (int i = 1;i < argc;i++) {
        _argc++;
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
        else {
            if (filepath != NULL) {
                printf("Too much arguments passed.\n");
                free(filepath);
                return 1;
            }
            filepath = malloc(sizeof(argv[i]));
            strcpy(filepath, argv[i]);
            _argc--;
        }
    }

    if (filepath == NULL) {
        printf("No file provided.\n");
        free(filepath);
        return 1;
    }

    file_index = open(filepath, O_RDONLY);

    if (file_index == -1) {
        printf("Error when opening file.\n");
        free(filepath);
        return 1;
    }

    if (_argc == 0) {
        show_all = true;
    }

    char* buf = malloc(sizeof(char));
    int err_code;

    while ((err_code = read(file_index, buf, 1)) > 0) {
        c = *buf;
        if (count_words) {
            if ((c != ' ' && c != '\n') && (word_state == OUT)) {
                word_count++;
                word_state = IN;
            }
            else if (c == ' ' || c == '\n') {
                word_state = OUT;
            }
        }
        if (show_table) {
            if (ignore_register) {
                if (c >= 'a' && c <= 'z') {
                    latin_lowercase[c - 'a']++;
                }
                if (c >= 'A' && c <= 'Z') {
                    latin_lowercase[c - 'A']++;
                }
            }
            else {
                if (c >= 'a' && c <= 'z') {
                    latin_lowercase[c - 'a']++;
                }
                if (c >= 'A' && c <= 'Z') {
                    latin_uppercase[c - 'A']++;
                }
            }
        }
        if (show_all) {
            ascii_symbols[c]++;
        }
    }

    free(buf);
    free(filepath);
    close(file_index);

    if (err_code == -1) {
        printf("Error when reading file.\n");
        return 1;
    }

    if (show_all) {
        printf(SPLITER_STRING);
        printf("Latin characters table.\n");
        for (int i = 0;i < 256;i++) {
            if (ascii_symbols[i]) {
                if (i <= 32 || i == 127)
                    printf("%d - %d\n", i, ascii_symbols[i]);
                else
                    printf("%c - %d\n", i, ascii_symbols[i]);
            }
        }
    }
    if (count_words) {
        printf(SPLITER_STRING);
        printf("Total word count - %d\n", word_count);
    }
    if (show_table) {
        printf(SPLITER_STRING);
        printf("Symbol table.\n");
        if (ignore_register) {
            for (int i = 0;i < 26;i++) {
                if (latin_lowercase[i] > 0)
                    printf("%c - %d\n", i + 'a', latin_lowercase[i]);
            }
        }
        else {
            for (int i = 0;i < 26;i++) {
                if (latin_lowercase[i] > 0)
                    printf("%c - %d\n", i + 'a', latin_lowercase[i]);
            }
            for (int i = 0;i < 26;i++) {
                if (latin_uppercase[i])
                    printf("%c - %d\n", i + 'A', latin_uppercase[i]);
            }
        }
    }

    return 0;
}