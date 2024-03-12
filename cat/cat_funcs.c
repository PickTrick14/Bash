#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

#include "cat.h"

enum {
    ERROR_CODE = -1,
    ASCII_LAST_CHAR = 127,
    MASK_7_BITS = 0x7F,
    ASCII_CTRL_END = 32,
    ASCII_DEL_CHAR = 127,
    FLAG_B_POS = 0,
    FLAG_E_POS,
    FLAG_N_POS,
    FLAG_S_POS,
    FLAG_T_POS,
    FLAG_V_POS
};

void parse_flag(int size_buf, char **buf, int *flags) {
    int cur_arg = 0;
    const struct option options[] = {{"number-nonblank", no_argument, NULL, 'b'},
                                     {"number", no_argument, NULL, 'n'},
                                     {"squeeze-blank", no_argument, NULL, 's'},
                                     {"show-tabs", no_argument, NULL, 'T'},
                                     {"show-nonprinting", no_argument, NULL, 'v'},
                                     {"show-ends", no_argument, NULL, 'E'},
                                     {NULL, no_argument, NULL, -2}};

    while ((cur_arg = getopt_long(size_buf, buf, "benstvET", options, NULL)) != -1) {
        switch (cur_arg) {
            case 'b':
                flags[FLAG_B_POS] = 1;
                break;
            case 'e':
                flags[FLAG_E_POS] = 1;
                flags[FLAG_V_POS] = 1;
                break;
            case 'n':
                flags[FLAG_N_POS] = 1;
                break;
            case 's':
                flags[FLAG_S_POS] = 1;
                break;
            case 't':
                flags[FLAG_T_POS] = 1;
                flags[FLAG_V_POS] = 1;
                break;
            case 'v':
                flags[FLAG_V_POS] = 1;
                break;
            case 'E':
                flags[FLAG_E_POS] = 1;
                break;
            case 'T':
                flags[FLAG_T_POS] = 1;
                break;
            default:
                break;
        }
    }
    return;
}

void parse_files(int size_buf, char **buf, char **file_names, int *amount_files) {
    for (int i = 1; i < size_buf; i++) {
        if (buf[i][0] != '-') {
            file_names[*amount_files] = buf[i];
            (*amount_files)++;
        }
    }
    return;
}

void print_char(int c, int *flags) {
    if (flags[FLAG_T_POS]) {
        if (c == '\t') {
            printf("^I");
            return;
        }
    }

    if (flags[FLAG_V_POS]) {
        if (c == '\t' || c == '\n') {
            putchar(c);
            return;
        }
        if (c > ASCII_LAST_CHAR) {
            printf("M-");
            c &= MASK_7_BITS;
        }
        if (c < ASCII_CTRL_END) {
            printf("^");
            c = c + '@';
        }
        if (c == ASCII_DEL_CHAR) {
            printf("^?");
            c = 0;
        }
        if (c != 0) {
            putchar(c);
        }
        return;
    }

    putchar(c);
    return;
}

int print_file(char *file_name, int *flags) {
    FILE *file = fopen(file_name, "r");
    if (file == NULL) {
        return ERROR_CODE;
    }
    int c = 0;
    unsigned long long count_str = 0;
    unsigned long long count_empty_str_row = 0;
    int flag_new_str = 1;
    while ((c = fgetc(file)) != EOF) {
        if (c == '\n') {
            if (flags[FLAG_E_POS]) {
                printf("$");
            }
            if (flags[FLAG_S_POS] && flag_new_str && count_empty_str_row >= 1) {
                continue;
            }
            if (!flag_new_str) {
                count_str++;
            } else {
                count_empty_str_row++;
            }
            if (!flags[FLAG_B_POS] && flags[FLAG_N_POS] && flag_new_str) {
                printf("%6llu\t", count_str + 1);
                count_str++;
            }
            putchar('\n');
            flag_new_str = 1;
            continue;
        }

        if (flag_new_str) {
            if (flags[FLAG_B_POS] && c != '\n') {
                printf("%6llu\t", count_str + 1);
            } else if (flags[FLAG_N_POS]) {
                printf("%6llu\t", count_str + 1);
            }
            flag_new_str = 0;
        }

        count_empty_str_row = 0;

        print_char(c, flags);
    }
    if (fclose(file) < 0) {
        return ERROR_CODE;
    }
    return 0;
}