#include "grep.h"

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: grep [OPTION]... PATTERN FILE...\n");
        return 0;
    }

    struct opt args = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    char **file_name = calloc(argc - 1, sizeof(*file_name));
    char **pattern = calloc(MAX_AMOUNT_PAT, sizeof(*pattern));
    char **match_files = calloc(argc - 1, sizeof(*match_files));
    size_t amount_files = 0;
    size_t amount_pattern = 0;
    size_t amount_match_files = 0;
    int flag_not_one_file = 0;

    parse_opt(argc, argv, &args, pattern, &amount_pattern);
    add_patterns(&amount_pattern, argv, &pattern, &args);
    parse_files(argc, argv, file_name, &amount_files);

    if (amount_pattern == 1 && !args.E) {
        amount_files--;
    }

    if (amount_files > 1) {
        flag_not_one_file = 1;
    }

    char *e_str = NULL;
    if (args.E) {
        e_str = reg_str(pattern, amount_pattern);
    }

    for (long long i = amount_files - 1; i >= 0; i--) {
        for (size_t j = 0; j < amount_pattern; j++) {
            if (args.E) {
                search_reg_match_file(file_name[i], e_str, &args, match_files, &amount_match_files,
                                      flag_not_one_file);
                break;
            } else {
                search_str_match_file(file_name[i], pattern[j], &args, match_files, &amount_match_files,
                                      flag_not_one_file);
            }
        }
    }

    if (amount_match_files > 0) {
        for (size_t i = 0; i < amount_match_files; i++) {
            printf("%s\n", match_files[i]);
        }
    }

    free_pattern(pattern, amount_pattern);
    free(file_name);
    free(pattern);
    free(match_files);
    if (e_str != NULL) {
        free(e_str);
    }

    return 0;
}