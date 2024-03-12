#ifndef GREP_H
#define GREP_H

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

enum { MAX_AMOUNT_CHAR = 65535, MAX_AMOUNT_PAT = 100 };

struct opt {
    int E;
    int I;
    int V;
    int C;
    int L;
    int N;
    int H;
    int S;
    int F;
    int O;
};

char *reg_str(char **pattern, size_t amount_pattern);
void parse_opt(int size_buf, char **buf, struct opt *args, char **pattern, size_t *amount_pattern);
void parse_files(int size_buf, char **buf, char **file_names, size_t *amount_files);
char **add_pattern_from_string(char *str, char ***pattern, size_t *amount_pattern);
void add_pattern_from_file(size_t *amount_pattern, char *file_name, char ***pattern);
void free_pattern(char **pattern, size_t amount_pattern);
void add_patterns(size_t *amount_pattern, char **buf, char ***pattern, struct opt *args);
void output_error_message(char *error, char *str, struct opt *args, int flag_file);
void output_str(char *str, char *file_name, size_t num, struct opt *args, int amount_char, int start,
                int flag);
int search_reg_match_str(char *str, char *print_str, char *file_name, regex_t pattern, struct opt *args,
                         size_t num_str, size_t *num_find, int flag_file);
void search_reg_match_file(char *file_name, char *pattern, struct opt *args, char **match_files,
                           size_t *amount_m_files, int flag_file);
int search_str_match_str(char *str, char *print_str, char *file_name, char *pattern, char *print_pattern,
                         struct opt *args, size_t num, size_t *num_find, int flag_file);
void search_str_match_file(char *file_name, char *pattern, struct opt *args, char **match_files,
                           size_t *amount_m_files, int flag_file);
void add_pattern_from_file(size_t *amount_pattern, char *file_name, char ***pattern);

#endif