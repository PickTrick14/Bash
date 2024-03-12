CC=gcc
CFLAGS=-Wall -Wextra -Werror -std=c11
WORK_FILES_DIR=work_files
CLEAN_FILES=*_cat *.o *.out *.txt test/*.txt make_tests test/cmp_files/* $(WORK_FILES_DIR)/*

all: cat

cat_funcs.o: cat_funcs.c
	$(CC) $(CFLAGS) -c cat_funcs.c -o $(WORK_FILES_DIR)/cat_funcs.o

cat.o: cat.c
	$(CC) $(CFLAGS) -c cat.c -o $(WORK_FILES_DIR)/cat.o

cat: cat_funcs.o cat.o
	$(CC) $(CFLAGS) $(WORK_FILES_DIR)/cat_funcs.o $(WORK_FILES_DIR)/cat.o -o my_cat

test: rebuild
	$(CC) make_tests.c -o $(WORK_FILES_DIR)/make_tests
	./$(WORK_FILES_DIR)/make_tests
clang:
	dos2unix *.c *.h
	clang-format -i *.c *.h

clean:
	rm -rf $(CLEAN_FILES)

rebuild: clean all