#include <ctype.h>

#include "grep.h"

void parse_opt(int size_buf, char **buf, struct opt *args, char **pattern, size_t *amount_pattern) {
    const struct option options[] = {{"regexp", required_argument, NULL, 'e'},
                                     {"file", required_argument, NULL, 'f'},
                                     {"ignore-case", no_argument, NULL, 'i'},
                                     {"invert-match", no_argument, NULL, 'v'},
                                     {"count", no_argument, NULL, 'c'},
                                     {"file-with-matches", no_argument, NULL, 'l'},
                                     {"line-number", no_argument, NULL, 'n'},
                                     {"no-filename", no_argument, NULL, 'h'},
                                     {"no-messages", no_argument, NULL, 's'},
                                     {"only-matching", required_argument, NULL, 'o'},
                                     {NULL, no_argument, NULL, -2}};

    int cur_arg = 0;

    while ((cur_arg = getopt_long(size_buf, buf, "e:ivclnhsf:o", options, NULL)) != -1) {
        switch (cur_arg) {
            case 'e':
                args->E = 1;
                add_pattern_from_string(optarg, &pattern, amount_pattern);
                break;
            case 'f':
                args->F = 1;
                args->E = 1;
                add_pattern_from_file(amount_pattern, optarg, &pattern);
                break;
            case 'i':
                args->I = 1;
                break;
            case 'v':
                args->V = 1;
                break;
            case 'c':
                args->C = 1;
                break;
            case 'l':
                args->L = 1;
                break;
            case 'n':
                args->N = 1;
                break;
            case 'h':
                args->H = 1;
                break;
            case 's':
                args->S = 1;
                break;
            case 'o':
                args->O = 1;
                break;
            default:
                break;
        }
    }
    return;
}

void parse_files(int size_buf, char **buf, char **file_names,
                 size_t *amount_files) {  //поиск имен файлов
    char *tmp = NULL;
    for (int i = size_buf - 1; i > 0; i--) {
        if (buf[i][0] != '-') {
            file_names[(*amount_files)++] = buf[i];
        } else if (strchr(buf[i], 'f') != NULL || ((tmp = strchr(buf[i], 'e')) != NULL && !*(tmp + 1))) {
            (*amount_files)--;
        }
    }
    return;
}

char **add_pattern_from_string(char *str, char ***pattern,
                               size_t *amount_pattern) {  // добавление мультипаттерна в массив
    if (str == NULL || pattern == NULL) return NULL;

    int flag = 1;
    char *tmp;

    do {
        if (*amount_pattern == MAX_AMOUNT_PAT) {
            *pattern = (char **)realloc(*pattern, MAX_AMOUNT_PAT * MAX_AMOUNT_PAT * sizeof(**pattern));
        }

        if (flag) {
            flag = 0;
            tmp = strtok(str, "\n");
        } else
            tmp = strtok(NULL, "\n");

        if (tmp != NULL && tmp[0] != '\0') {
            char *new_str = calloc(strlen(tmp) + 1, sizeof(*new_str));
            strncpy(new_str, tmp, strlen(tmp) + 1);
            new_str[strlen(tmp)] = '\0';
            (*pattern)[(*amount_pattern)++] = new_str;
        }
    } while (tmp != NULL);
    return *pattern;
}

void add_pattern_from_file(size_t *amount_pattern, char *file_name, char ***pattern) {
    FILE *f = fopen(file_name, "r");
    if (f == NULL) {
        output_error_message("No such file or directory: ", file_name, NULL, 1);
        return;
    }

    char *str = calloc(MAX_AMOUNT_CHAR, sizeof(*str));
    while (fgets(str, MAX_AMOUNT_CHAR, f) != NULL) {
        if (str[0] != '\n' && str[strlen(str) - 1] == '\n') str[strlen(str) - 1] = '\0';

        (*pattern)[(*amount_pattern)++] = str;

        str = calloc(MAX_AMOUNT_CHAR, sizeof(*str));
    }

    free(str);
    fclose(f);
    return;
}

void free_pattern(char **pattern, size_t amount_pattern) {
    if (pattern == NULL) return;
    for (size_t i = 0; i < amount_pattern; i++) {
        free(pattern[i]);
    }
    return;
}

void add_patterns(size_t *amount_pattern, char **buf, char ***pattern,
                  struct opt *args) {  // добавление всех паттернов в массив

    if (buf == NULL || pattern == NULL || args == NULL) return;

    if (!args->E && !args->F) {
        int i = 1;
        while (buf[i][0] == '-') {
            i++;
        }
        add_pattern_from_string(buf[i], pattern, amount_pattern);
    }
    return;
}

char *reg_str(char **pattern, size_t amount_pattern) {
    if (pattern == NULL) return NULL;

    char *e_str = NULL;
    for (size_t i = 0; i < amount_pattern; i++) {
        if (e_str == NULL) {
            e_str = calloc(strlen(pattern[i]) + 1, sizeof(*e_str));
            strncpy(e_str, pattern[i], strlen(pattern[i]));
            e_str[strlen(pattern[i])] = '\0';
            continue;
        }

        e_str = realloc(e_str, (strlen(e_str) + strlen(pattern[i]) + 2) * sizeof(*e_str));
        strcat(e_str, "|");
        strncat(e_str, pattern[i], strlen(pattern[i]) + 1);
    }
    return e_str;
}

void lower_str(char *str) {
    if (str == NULL) return;

    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = tolower(str[i]);
    }
    return;
}

void output_error_message(char *error, char *str, struct opt *args, int flag_file) {
    if (args == NULL) {
        fprintf(stderr, error, str);
        return;
    }

    if (!args->S && flag_file) fprintf(stderr, error, str);  // возможное подавление ошибки
    return;
}

void output_str(char *str, char *file_name, size_t num, struct opt *args, int amount_char, int start,
                int flag) {
    if (flag && !args->H) printf("%s:", file_name);  // вывод названия файла

    if (args->N) printf("%lu:", num);  // вывод номера строки

    printf("%.*s", amount_char, str + start);
    if (start != 0 || str[amount_char - 1] != '\n') printf("\n");
    return;
}

int search_reg_match_str(char *str, char *print_str, char *file_name, regex_t pattern, struct opt *args,
                         size_t num, size_t *num_find,
                         int flag_file) {  // поиск по регулярному выражению в строке

    size_t size_match_shift = strlen(str) + 1;
    regmatch_t match_shift[size_match_shift];
    for (size_t i = 0; i < size_match_shift; i++) {
        match_shift[i].rm_so = -1;
    }
    int status = 0;
    size_t shift_str = 0;
    int flag_num_str = 1;

    do {
        status = regexec(&pattern, str + shift_str, size_match_shift, match_shift, 0);
        if ((status == 0 && !args->V) || (status != 0 && args->V)) {
            *num_find += flag_num_str;
            flag_num_str = 0;

            if (args->L) {
                return args->L;
            }
            if (!args->C) {
                if (!args->O && !args->V) {
                    output_str(print_str + shift_str, file_name, num, args, strlen(print_str + shift_str), 0,
                               flag_file);
                } else if (!args->V) {
                    for (size_t i = 0; i < size_match_shift && match_shift[i].rm_so != -1; i++) {
                        output_str(print_str + shift_str, file_name, num, args,
                                   match_shift[0].rm_eo - match_shift[0].rm_so, match_shift[0].rm_so,
                                   flag_file);
                    }
                }
            }
            shift_str += match_shift[0].rm_eo;
        } else {
            break;
        }
    } while (status == 0 && shift_str + 1 < size_match_shift && args->O);

    return 0;
}

void search_reg_match_file(char *file_name, char *pattern, struct opt *args, char **match_files,
                           size_t *amount_m_files,
                           int flag_file) {  // поиск по регулярному выражению в файле
    if (file_name == NULL || pattern == NULL || match_files == NULL || amount_m_files == NULL) {
        return;
    }
    regex_t regex;
    int reg_flag = REG_EXTENDED | REG_NEWLINE;
    if (args->I) reg_flag |= REG_ICASE;

    int error = 0;
    if ((error = regcomp(&regex, pattern, reg_flag)) != 0) {
        char s[MAX_AMOUNT_CHAR];
        regerror(error, &regex, s, MAX_AMOUNT_CHAR);
        printf("%s\n", s);
        output_error_message("Invalid regular expression: %s\n", pattern, args, 0);
        return;
    }

    FILE *f = fopen(file_name, "r");
    if (f == NULL) {
        regfree(&regex);
        output_error_message("./s21_grep: %s: No such file or directory\n", file_name, args, 1);
        return;
    }

    char *str = calloc(MAX_AMOUNT_CHAR, sizeof(*str));
    char *work_str = calloc(MAX_AMOUNT_CHAR, sizeof(*str));
    size_t num_find = 0;
    size_t str_num = 0;

    while (fgets(str, MAX_AMOUNT_CHAR, f) != NULL) {
        str_num++;
        strncpy(work_str, str, strlen(str));
        work_str[strlen(str)] = '\0';
        if (args->I) {  // если надо игнорировать регистр
            lower_str(work_str);
        }

        if (search_reg_match_str(work_str, str, file_name, regex, args, str_num, &num_find, flag_file)) {
            match_files[*amount_m_files] = file_name;
            *amount_m_files += 1;
            break;
        }
    }

    regfree(&regex);
    free(str);
    free(work_str);
    fclose(f);

    if (args->C && !args->L) {  // выводит кол-во совпадающих строк
        if (flag_file && !args->H) printf("%s:", file_name);
        printf("%lu\n", num_find);
    }

    return;
}

int search_str_match_str(char *str, char *print_str, char *file_name, char *pattern, char *print_pattern,
                         struct opt *args, size_t num, size_t *num_find,
                         int flag_file) {  // поиск совпадения в строке
    if (str == NULL || pattern == NULL) return 0;

    char *match = NULL;
    char *tmp = str;
    int flag_find = 1;
    size_t len_str = strlen(str);
    size_t len_pattern = strlen(pattern);

    while (*tmp && (((match = strstr(tmp, pattern)) != NULL && !args->V) || (match == NULL && args->V))) {
        *num_find += flag_find;
        flag_find = 0;

        if (args->L) {  // выводит совпадающий файл и заканчивает цикл
            return args->L;
        }
        if (!args->C) {  // выводит совпадающую найденную строку, если не надо
                         // только кол-во
            if (!args->V && args->O)  // выводит только совпадающую строку
                output_str(print_pattern, file_name, num, args, strlen(pattern), 0, flag_file);
            else if (!args->O) {
                output_str(print_str, file_name, num, args, strlen(str), 0, flag_file);
                break;
            }
        }

        if ((size_t)(match + len_pattern - str) > len_str)
            tmp = "\0";
        else
            tmp = match + len_pattern;
    }
    return 0;
}

void search_str_match_file(char *file_name, char *pattern, struct opt *args, char **match_files,
                           size_t *amount_m_files,
                           int flag_file) {  //поиск совпадения в файле
    if (pattern == NULL || file_name == NULL || match_files == NULL || amount_m_files == NULL) {
        return;
    }

    FILE *f = fopen(file_name, "r");
    if (f == NULL) {
        output_error_message("./s21_grep: %s: No such file or directory\n", file_name, args, 1);
        return;
    }

    char *str = calloc(MAX_AMOUNT_CHAR, sizeof(*str));
    char *work_str = calloc(MAX_AMOUNT_CHAR, sizeof(*str));
    char *work_pattern = calloc(MAX_AMOUNT_CHAR, sizeof(*str));

    strncpy(work_pattern, pattern, strlen(pattern));
    work_pattern[strlen(pattern)] = '\0';

    size_t num_find = 0;
    size_t str_num = 0;

    if (args->I) {
        lower_str(work_pattern);  // если надо игнорировать регистр
    }

    while (fgets(str, MAX_AMOUNT_CHAR, f) != NULL) {
        str_num++;
        strncpy(work_str, str, strlen(str));
        work_str[strlen(str)] = '\0';
        if (args->I) {  // если надо игнорировать регистр
            lower_str(work_str);
        }

        if (search_str_match_str(work_str, str, file_name, work_pattern, pattern, args, str_num, &num_find,
                                 flag_file)) {
            match_files[*amount_m_files] = file_name;
            *amount_m_files += 1;
            break;
        }
    }

    if (args->C && !args->L) {  // выводит кол-во совпадающих строк
        if (flag_file && !args->H) printf("%s:", file_name);
        printf("%lu\n", num_find);
    }

    free(str);
    free(work_pattern);
    free(work_str);
    fclose(f);
    return;
}