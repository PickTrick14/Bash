#include "cat.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        int c;
        while ((c = getchar()) != EOF) {
            putchar(c);
        }
        return 0;
    }

    int args[] = {0, 0, 0, 0, 0, 0};
    char **file_names = calloc(argc - 1, sizeof(*file_names));
    int amount_files = 0;

    parse_flag(argc, argv, args);
    parse_files(argc, argv, file_names, &amount_files);

    for (int i = 0; i < amount_files; i++) {
        if (print_file(file_names[i], args) < 0) {
            printf("No such file or directory, access denied or close error\n");
        }
    }

    free(file_names);

    return 0;
}