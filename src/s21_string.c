#include "s21_string.h"

void *s21_memchr(const void *str, int c, s21_size_t n) {
  unsigned char *result = s21_NULL;
  for (s21_size_t i = 0; i < n && !result; i++) {
    if (*((unsigned char *)str + i) == (unsigned char)c) {
      result = (unsigned char *)str + i;
    }
  }

  return (void *)result;
}

int s21_memcmp(const void *str1, const void *str2, s21_size_t n) {
  int result = 0;
  for (s21_size_t i = 0; i < n && !result; i++) {
    result = *((unsigned char *)str1 + i) - *((unsigned char *)str2 + i);
  }

  return result;
}

void *s21_memcpy(void *dest, const void *src, s21_size_t n) {
  void *start = dest;
  for (s21_size_t i = 0; i < n; i++) {
    *((unsigned char *)dest + i) = *((unsigned char *)src + i);
  }

  return start;
}

void *s21_memset(void *str, int c, s21_size_t n) {
  void *start = str;
  for (s21_size_t i = 0; i < n; i++) {
    *((unsigned char *)str + i) = (unsigned char)c;
  }

  return start;
}

char *s21_strncat(char *dest, const char *src, s21_size_t n) {
  s21_size_t len_dest = s21_strlen(dest);
  s21_strncpy(dest + len_dest, src, n);
  if (s21_strlen(src) >= n) {
    dest[len_dest + n] = '\0';
  }
  return dest;
}

char *s21_strchr(const char *str, int c) {
  while (*str && *str != (char)c) {
    str++;
  }
  return (*str == (char)c) ? (char *)str : s21_NULL;
}

int s21_strncmp(const char *str1, const char *str2, s21_size_t n) {
  int result = 0;
  s21_size_t i = 0;
  for (; i < n && str1[i] && str1[i] == str2[i]; i++) {
  }
  if (i < n) {
    result = (unsigned char)str1[i] - (unsigned char)str2[i];
  }
  return result;
}

char *s21_strncpy(char *dest, const char *src, s21_size_t n) {
  s21_size_t i = 0;
  for (; i < n && src[i]; i++) {
    dest[i] = src[i];
  }

  s21_memset(dest + i, '\0', n - i);

  return dest;
}

s21_size_t s21_strcspn(const char *str1, const char *str2) {
  s21_size_t len = s21_strlen(str1);
  const char *first_ch = s21_strpbrk(str1, str2);
  if (first_ch) {
    len = (s21_size_t)(first_ch - str1);
  }

  return len;
}

s21_size_t s21_strlen(const char *str) {
  s21_size_t len = 0;
  while (str[len]) {
    len++;
  }

  return len;
}

char *s21_strpbrk(const char *str1, const char *str2) {
  char *result = s21_NULL;
  while (*str1 && !result) {
    if (s21_strchr(str2, *str1)) {
      result = (char *)str1;
    }
    str1++;
  }

  return result;
}

char *s21_strrchr(const char *str, int c) {
  const char *rev = str + s21_strlen(str);
  while (str <= rev && *rev != (char)c) {
    rev--;
  }

  return str <= rev ? (char *)rev : s21_NULL;
}

char *s21_strstr(const char *haystack, const char *needle) {
  char *result = s21_NULL;
  if (*needle == '\0') {
    return (char *)haystack;
  }

  while (*haystack && !result) {
    const char *h = haystack;
    const char *n = needle;

    while (*h && *n && *h == *n) {
      h++;
      n++;
    }

    if (*n == '\0') {
      result = (char *)haystack;
    }

    haystack++;
  }

  return result;
}

char *s21_strtok(char *str, const char *delim) {
  static char *last = s21_NULL;
  char *token = s21_NULL;

  if (!str && !(str = last)) {
    return s21_NULL;
  }

  while (*str && s21_strchr(delim, *str)) {
    str++;
  }

  if (*str) {
    token = str;
    str += s21_strcspn(str, delim);
    last = *str ? *str = '\0', str + 1 : s21_NULL;
  }
  return token;
}
