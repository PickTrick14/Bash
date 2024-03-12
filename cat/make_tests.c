#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

enum { AMOUNT_ASCII_SYMB = 256 };

int main(void) {
    unsigned long long n = 0;
    printf("Введите кол-во создаваемых файлов для теста: ");
    if (scanf("%llu", &n) < 0) {
        printf("Это не число. Перезапускай заново!\n");
        return 0;
    }

    for (unsigned long long i = 0; i < n; i++) {
        char file_name[PATH_MAX];
        snprintf(file_name, PATH_MAX, "./test/test%llu.txt", i);

        FILE *file = fopen(file_name, "w");
        for (int i = 0; i < 1000; i++) {
            fprintf(file, "%c", rand() % AMOUNT_ASCII_SYMB);
        }
        fclose(file);
    }
    return 0;
}