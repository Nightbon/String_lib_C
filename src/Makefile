CC = gcc
CFLAGS_BASE = -Wall -Wextra -Werror -std=c11 -pedantic
CFLAGS_COVERAGE = -fprofile-arcs -ftest-coverage -g

LDFLAGS_BASE = -lcheck -lm
LDFLAGS_COVERAGE = -fprofile-arcs -ftest-coverage -lcheck -lm

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
  CHECK_PREFIX := $(shell brew --prefix check 2>/dev/null)
  CFLAGS_BASE += -I$(CHECK_PREFIX)/include
  LDFLAGS_BASE += -L$(CHECK_PREFIX)/lib
  LDFLAGS_COVERAGE += -L$(CHECK_PREFIX)/lib
else
  LDFLAGS_BASE += -lsubunit
  LDFLAGS_COVERAGE += -lsubunit
endif

SRC = s21_string.c s21_strerror.c s21_sprintf.c s21_sscanf.c s21_sharp.c
OBJ = $(SRC:.c=.o)
LIB = s21_string.a
TEST_SRC = test_s21_string.c
TEST_OBJ = test_s21_string.o

.PHONY: all clean s21_string.a test gcov_report gcov_test

# all — только библиотека (без покрытия)
all: s21_string.a

s21_string.a: $(OBJ)
	ar rcs $@ $^

# Компиляция обычных .o без покрытия
%.o: %.c s21_string.h
	$(CC) $(CFLAGS_BASE) -c $<

# Сборка тестов без покрытия
test: s21_string.a test_string
	chmod +x test_string
	./test_string

test_string: s21_string.a $(TEST_OBJ)
	$(CC) $(CFLAGS_BASE) $(TEST_OBJ) s21_string.a -o $@ $(LDFLAGS_BASE)

$(TEST_OBJ): $(TEST_SRC) s21_string.h
	$(CC) $(CFLAGS_BASE) -c $<

# Сборка с покрытием
gcov_test: gcov_s21_string.a gcov_test_string
	chmod +x gcov_test_string
	./gcov_test_string

gcov_s21_string.a: $(SRC:.c=.gcov.o)
	ar rcs $@ $^

# Объекты с покрытием
%.gcov.o: %.c s21_string.h
	$(CC) $(CFLAGS_BASE) $(CFLAGS_COVERAGE) -c $< -o $@

gcov_test_string: gcov_s21_string.a test_s21_string.gcov.o
	$(CC) $(CFLAGS_BASE) $(CFLAGS_COVERAGE) test_s21_string.gcov.o gcov_s21_string.a -o $@ $(LDFLAGS_COVERAGE)

test_s21_string.gcov.o: test_s21_string.c s21_string.h
	$(CC) $(CFLAGS_BASE) $(CFLAGS_COVERAGE) -c $< -o $@

clean:
	rm -rf *.o *.a test_string *.gcda *.gcno coverage.info coverage_report filtered.info gcov_test_string

gcov_report: gcov_test
	lcov --capture --directory . --output-file coverage.info
	genhtml coverage.info --output-directory coverage_report