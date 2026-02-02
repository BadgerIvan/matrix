CC:=gcc
CFLAGS:=-Wall -Wextra -Werror -Wpedantic -std=c11
LIBFLAGS:=
TEST_FLAGS:=-lcheck -lm -lpthread -lsubunit
GCOV_FLAGS:=-fprofile-arcs -ftest-coverage

PROFILE:=
ifeq ($(PROFILE),debug)
	CFLAGS+=-g -O0
else
	CFLAGS+=-O3
endif

AVX:=$(shell lscpu | grep -e "\bavx\b" -o)
ifeq ($(AVX),avx)
	LIBFLAGS+=-mavx
endif

SRC_DIR:=.
TEST_DIR:=../tests
BUILD_DIR:=.
REPORT_DIR:=report

SOURCES:=$(shell find $(SRC_DIR) -name "*.c")
OBJECTS:=$(SOURCES:.c=.o)
GCOV_OBJECTS:=$(SOURCES:.c=.gcov.o)

TEST_SOURCES:=$(shell find $(TEST_DIR) -name "*.c")
TEST_OBJECTS:=$(TEST_SOURCES:.c=.o)

.PHONY: all rebuild clean test gcov_report clang-format

all: matrix.a

matrix.a: $(OBJECTS)
	ar rcs matrix.a $(OBJECTS)
	ranlib matrix.a

.c.o:
	$(CC) $(CFLAGS) $(LIBFLAGS) -c $< -o $@

%.gcov.o: %.c
	$(CC) $(CFLAGS) $(GCOV_FLAGS) -c $< -o $@

$(TEST_DIR)/%.o: $(TEST_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

test: $(TEST_OBJECTS) $(GCOV_OBJECTS)
	$(CC) $(CFLAGS) $(GCOV_FLAGS) $(TEST_OBJECTS) $(GCOV_OBJECTS) $(TEST_FLAGS) -o test

gcov_report: test
	mkdir -p $(REPORT_DIR)
	lcov -t "matrix_test" -o $(REPORT_DIR)/matrix_test.info -c -d .
	genhtml -o $(REPORT_DIR) $(REPORT_DIR)/matrix_test.info
	open $(REPORT_DIR)/index.html

clang-format:
	clang-format --style=file:../materials/linters/.clang-format -i *.c *.h ../tests/*.c

clean:
	rm -rf *.o *.gcov.o
	rm -rf matrix.a
	rm -rf test
	rm -rf *.gcda *.gcno
	rm -rf $(REPORT_DIR)
	rm -rf $(TEST_DIR)/*.o

rebuild: clean all