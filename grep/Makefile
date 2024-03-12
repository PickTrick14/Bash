CC=gcc
CFLAGS=-Wall -Wextra -Werror -std=c11 -g
WORK_FILES_DIR=work_files
CLEAN_FILES=*_grep *.txt $(WORK_FILES_DIR)/* *.log
CLANG_FILE=./work_files/.my_clang-format

all: grep

grep_funcs.o:
	$(CC) $(CFLAGS) -c grep_funcs.c -o $(WORK_FILES_DIR)/grep_funcs.o

grep.o:
	$(CC) $(CFLAGS) -c grep.c -o $(WORK_FILES_DIR)/grep.o

grep: grep_funcs.o grep.o
	$(CC) $(CFLAGS) $(WORK_FILES_DIR)/grep_funcs.o $(WORK_FILES_DIR)/grep.o -o my_grep

clang:
	dos2unix *.c *.h
	clang-format -style=file:$(CLANG_FILE) -i *.c *.h

clean:
	rm -rf $(CLEAN_FILES)

rebuild: clean all