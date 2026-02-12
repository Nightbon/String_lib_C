#ifndef MY_STRING_H
#define MY_STRING_H

#include <ctype.h>
#include <locale.h>
#include <math.h>
#include <stdarg.h>
#include <stdlib.h>
#include <wchar.h>

#if defined(_WIN64) || defined(__x86_64__) || defined(__ppc64__)
typedef unsigned long long s21_size_t;
#else
typedef unsigned int s21_size_t;
#endif

#define s21_NULL ((void *)0)
#define MAX_DIGITS 1024

typedef struct {
  int minus;
  int plus;
  int space;
  int dot;
  int null;
  int hashtag;
  s21_size_t width;
  s21_size_t precision;
  int length_short;
  int length_long;
  int length_long_double;
  int remove_zeros;
  int suppress;
  int n;
  int num;
  int upper;
  int is_unsigned;
  int error;
} Options;

int s21_sscanf(const char *str, const char *format, ...)
    __attribute__((format(scanf, 2, 3))) __attribute__((nonnull(1, 2)));
int s21_sprintf(char *str, const char *format, ...)
    __attribute__((format(printf, 2, 3))) __attribute__((nonnull(1, 2)));
int s21_strncmp(const char *str1, const char *str2, s21_size_t n);
int s21_memcmp(const void *str1, const void *str2, s21_size_t n);
char *s21_strncat(char *dest, const char *src, s21_size_t n);
char *s21_strncpy(char *dest, const char *src, s21_size_t n);
void *s21_memcpy(void *dest, const void *src, s21_size_t n);
s21_size_t s21_strcspn(const char *str1, const char *str2);
char *s21_strstr(const char *haystack, const char *needle);
void *s21_memchr(const void *str, int c, s21_size_t n);
char *s21_strpbrk(const char *str1, const char *str2);
void *s21_memset(void *str, int c, s21_size_t n);
char *s21_strtok(char *str, const char *delim);
char *s21_strrchr(const char *str, int c);
char *s21_strchr(const char *str, int c);
s21_size_t s21_strlen(const char *str);
char *s21_strerror(int errnum);
void *s21_insert(const char *src, const char *str, s21_size_t start_index);
void *s21_trim(const char *src, const char *trim_chars);
void *s21_to_upper(const char *str);
void *s21_to_lower(const char *str);

#endif  // MY_STRING_H