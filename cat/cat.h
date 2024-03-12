#ifndef CAT_H

void parse_flag(int size_buf, char **buf, int *flags);
void parse_files(int size_buf, char **buf, char **file_names, int *amount_files);
void print_char(int c, int *flags);
int print_file(char *file_name, int *flags);

#endif