#include "s21_string.h"

void *s21_to_upper(const char *str) {
  if (str == s21_NULL) {
    return s21_NULL;
  }
  char *res = s21_NULL;

  res = malloc((s21_strlen(str) + 1) * sizeof(char));
  if (res != s21_NULL) {
    s21_size_t i = 0;

    for (i = 0; str[i] != '\0'; i++) {
      if (str[i] >= 'a' && str[i] <= 'z')
        res[i] = str[i] - 32;
      else
        res[i] = str[i];
    }

    res[i] = '\0';
  }

  return res;
}

void *s21_to_lower(const char *str) {
  if (str == s21_NULL) {
    return s21_NULL;
  }
  char *res = s21_NULL;
  res = malloc((s21_strlen(str) + 1) * sizeof(char));
  if (res != s21_NULL) {
    s21_size_t i = 0;

    for (i = 0; str[i] != '\0'; i++) {
      if (str[i] >= 'A' && str[i] <= 'Z')
        res[i] = str[i] + 32;
      else
        res[i] = str[i];
    }

    res[i] = '\0';
  }

  return res;
}

void *s21_insert(const char *src, const char *str, s21_size_t start_index) {
  if (src == s21_NULL || str == s21_NULL) {
    return s21_NULL;
  }
  char *res = s21_NULL;

  s21_size_t src_len = s21_strlen(src);
  s21_size_t str_len = s21_strlen(str);
  int len_diff = str_len + src_len - start_index;

  if (len_diff >= 0 && len_diff <= (int)(src_len + str_len)) {
    res = malloc((str_len + src_len + 1) * sizeof(char));
    if (res != s21_NULL) {
      s21_size_t curr = 0;
      s21_size_t str_i = 0;
      s21_size_t src_i = 0;

      while (curr < start_index) {
        res[curr++] = src[src_i++];
      }

      for (s21_size_t i = start_index; str[str_i] != '\0'; i++) {
        res[i] = str[str_i++];
        curr++;
      }

      while (src[src_i] != '\0') {
        res[curr++] = src[src_i++];
      }

      res[curr] = '\0';
    }
  }

  return res;
}

void *s21_trim(const char *src, const char *trim_chars) {
  if (src == s21_NULL) {
    return s21_NULL;
  }
  char *res = s21_NULL;
  s21_size_t len;
  if (trim_chars == s21_NULL || *trim_chars == '\0') {
    len = s21_strlen(src);
    res = malloc((len + 1) * sizeof(char));

    if (res != s21_NULL) {
      s21_memcpy(res, src, len + 1);
    }

  } else {
    s21_size_t start = 0;
    s21_size_t end = s21_strlen(src);

    while (start < end && (s21_strchr(trim_chars, src[start]) != s21_NULL)) {
      start++;
    }

    while (end > start && (s21_strchr(trim_chars, src[end - 1]) != s21_NULL)) {
      end--;
    }

    len = end - start;
    res = malloc((len + 1) * sizeof(char));

    if (res != s21_NULL) {
      s21_memcpy(res, src + start, len);
      res[len] = '\0';
    }
  }

  return res;
}
