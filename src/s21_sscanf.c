#include "s21_string.h"

const char *s21_parser_length(const char *format, Options *opts);
static const char *s21_processing_argument(const char *str, const char format,
                                           va_list *args, Options *opts,
                                           int *count);
static const char *s21_parse_int(const char *str, va_list *args, Options *opts,
                                 int base, int *count);
static const char *s21_parse_pointer(const char *str, va_list *args,
                                     const Options *opts, int *count);
static const char *s21_parse_string(const char *str, va_list *args,
                                    const Options *opts, int *count);
static const char *s21_parse_float(const char *str, va_list *args,
                                   const Options *opts, int *count);
static const char *s21_processing_string_long(const char *str, wchar_t *dest,
                                              s21_size_t width);
static long long s21_strtoll(const char *str, const char **endptr, int base,
                             s21_size_t width);
static const char *s21_parse_char(const char *str, va_list *args, Options *opts,
                                  int *count);
static const char *s21_processing_char_long(const char *str, wchar_t *wc,
                                            s21_size_t width);
static const char *s21_processing_string(const char *str, char *dest,
                                         s21_size_t width);
static long double s21_strtold(const char *str, const char **endptr,
                               s21_size_t width);
static const char *s21_processing_char(const char *str, char *c,
                                       s21_size_t width);
static void s21_record_num(long long value, va_list *args, const Options *opts);
static const char *s21_processing_exp(const char *string, long double *value);
static const char *s21_handle_inf_nan(const char *str, long double *out_value);
static const char *s21_parser_width_sscanf(const char *format, Options *opts);
static const char *s21_check_sign(const char *str, int *sign);
static const char *s21_check_base(const char *str, int *base);
static int s21_check_format(const char **str, const char **format);
static int s21_search_char(const char ch);

int s21_sscanf(const char *str, const char *format, ...) {
  if (*str == '\0' || *format == '\0') {
    return -1;
  }
  setlocale(LC_ALL, "");
  va_list args;
  va_start(args, format);

  const char *start = str;
  int count = 0;

  while (s21_check_format(&str, &format) && count >= 0) {
    Options opts = {0};
    opts.num = (str - start);
    format = s21_parser_width_sscanf(format, &opts);
    format = s21_parser_length(format, &opts);
    str = s21_processing_argument(str, *format, &args, &opts, &count);
    format++;
  }

  va_end(args);
  return count;
}

int s21_check_format(const char **str, const char **format) {
  if (**format == '\0') {
    return 0;
  }
  while (**format && isspace((unsigned char)**format)) {
    (*str) += (**str == **format);
    (*format)++;
  }
  int result = 0;
  if (**format != '\0') {
    if (**format == '%' && *((*format) + 1) != '%') {
      result = 1;
    } else {
      result = (**str == **format);
      *str += result;
      *format += result;
    }
    if (result) {
      result = (**format != '\0');
      (*format) += result;
    }
  }

  return result;
}

const char *s21_parser_width_sscanf(const char *format, Options *opts) {
  if (*format == '*') {
    opts->suppress = 1;
    format++;
  }
  while (*format >= '0' && *format <= '9') {
    opts->width = opts->width * 10 + (*format++ - '0');
  }
  return format;
}

const char *s21_parser_length(const char *format, Options *opts) {
  if (*format == 'h') {
    opts->length_short = 1;
    format++;
  } else if (*format == 'l') {
    opts->length_long = 1;
    format++;
    if (*format == 'l') {
      format++;
    }
  } else if (*format == 'L') {
    opts->length_long_double = 1;
    format++;
  }
  return format;
}

const char *s21_processing_argument(const char *str, const char format,
                                    va_list *args, Options *opts, int *count) {
  if (format != 'c') {
    while (isspace(*str)) str++;
  }

  int base = 10;
  if (format == 'x' || format == 'X') {
    base = 16;
  } else if (format == 'o') {
    base = 8;
  } else if (format == 'i') {
    base = 0;
  }

  if (s21_strchr("uoxX", format)) {
    opts->is_unsigned = 1;
  } else if (format == 'n') {
    opts->n = 1;
  }

  if (s21_strchr("uoxXdni", format)) {
    str = s21_parse_int(str, args, opts, base, count);
  } else if (s21_strchr("eEgGf", format)) {
    str = s21_parse_float(str, args, opts, count);
  } else if (format == 'c') {
    str = s21_parse_char(str, args, opts, count);
  } else if (format == 's') {
    str = s21_parse_string(str, args, opts, count);
  } else if (format == 'p') {
    str = s21_parse_pointer(str, args, opts, count);
  }
  return str;
}

const char *s21_parse_char(const char *str, va_list *args, Options *opts,
                           int *count) {
  if (!opts->width) opts->width = 1;

  if (opts->suppress) {
    str += opts->width;
  } else {
    if (opts->length_long) {
      wchar_t *wc = va_arg(*args, wchar_t *);
      str = s21_processing_char_long(str, wc, opts->width);
    } else {
      char *c = va_arg(*args, char *);
      str = s21_processing_char(str, c, opts->width);
    }
    (*count)++;
  }
  return str;
}

const char *s21_processing_char_long(const char *str, wchar_t *wc,
                                     s21_size_t width) {
  s21_size_t i = 0;
  while (i < width && *str != '\0') {
    mbstate_t state = {0};
    int ret = mbrtowc(wc + i++, str, MB_CUR_MAX, &state);
    str += ret > 0 ? ret : 0;
  }
  return str;
}

const char *s21_processing_char(const char *str, char *c, s21_size_t width) {
  s21_size_t i = 0;
  while (i < width && *str != '\0') {
    c[i++] = *str++;
  }
  return str;
}

const char *s21_parse_string(const char *str, va_list *args,
                             const Options *opts, int *count) {
  if (opts->suppress) {
    while (*str && !isspace(*str)) str++;
  } else {
    if (opts->length_long) {
      wchar_t *dest = va_arg(*args, wchar_t *);
      str = s21_processing_string_long(str, dest, opts->width);
    } else {
      char *dest = va_arg(*args, char *);
      str = s21_processing_string(str, dest, opts->width);
    }
    (*count)++;
  }
  return str;
}

const char *s21_processing_string_long(const char *str, wchar_t *dest,
                                       s21_size_t width) {
  s21_size_t i = 0;
  while (*str && !isspace(*str) && i < width) {
    mbstate_t state = {0};
    int ret = mbrtowc(dest + i++, str, MB_CUR_MAX, &state);
    str += ret > 0 ? ret : 0;
  }
  dest[i] = L'\0';
  return str;
}

const char *s21_processing_string(const char *str, char *dest,
                                  s21_size_t width) {
  s21_size_t i = 0;
  while (*str && !isspace(*str) && i < width) {
    dest[i++] = *str++;
  }
  dest[i] = '\0';
  return str;
}

const char *s21_parse_int(const char *str, va_list *args, Options *opts,
                          int base, int *count) {
  const char *endptr = str;
  long long value =
      opts->n ? opts->num : s21_strtoll(str, &endptr, base, opts->width);
  opts->num = 0;
  if ((endptr > str || opts->n) && !opts->suppress) {
    s21_record_num(value, args, opts);
    if (opts->n) {
      opts->n = 0;
    } else {
      (*count)++;
    }
  }
  return endptr;
}

void s21_record_num(long long value, va_list *args, const Options *opts) {
  if (opts->length_short) {
    if (opts->is_unsigned) {
      unsigned short *ptr = va_arg(*args, unsigned short *);
      *ptr = (unsigned short)value;
    } else {
      short *ptr = va_arg(*args, short *);
      *ptr = (short)value;
    }
  } else if (opts->length_long) {
    if (opts->is_unsigned) {
      unsigned long *ptr = va_arg(*args, unsigned long *);
      *ptr = (unsigned long)value;
    } else {
      long *ptr = va_arg(*args, long *);
      *ptr = (long)value;
    }
  } else {
    if (opts->is_unsigned) {
      unsigned int *ptr = va_arg(*args, unsigned int *);
      *ptr = (unsigned int)value;
    } else {
      int *ptr = va_arg(*args, int *);
      *ptr = (int)value;
    }
  }
}

long long s21_strtoll(const char *str, const char **endptr, int base,
                      s21_size_t width) {
  const char *string = str;
  long long value = 0;
  int sign = 1;
  char buf[MAX_DIGITS + 1] = {0};
  width = width >= MAX_DIGITS ? MAX_DIGITS : width;
  if (width) {
    string = buf;
    s21_strncpy(buf, str, width);
  }

  string = s21_check_sign(string, &sign);
  string = s21_check_base(string, &base);

  while (*string && (s21_search_char(*string) != -1) &&
         (s21_search_char(*string) < base)) {
    value = value * base + s21_search_char(*string);
    string++;
  }

  for (s21_size_t i = 0; *str && *str != ' ' && i < width; i++) str++;
  *endptr = width ? str : string;

  return sign * value;
}

int s21_search_char(char ch) {
  int result = -1;
  if ('0' <= ch && ch <= '9')
    result = ch - '0';
  else if ('a' <= ch && ch <= 'f')
    result = ch - 'a' + 10;
  else if ('A' <= ch && ch <= 'F')
    result = ch - 'A' + 10;
  return result;
}

const char *s21_check_sign(const char *str, int *sign) {
  if (*str == '-') {
    *sign = -1;
    str++;
  } else if (*str == '+') {
    str++;
  }
  return str;
}

const char *s21_check_base(const char *str, int *base) {
  int basis = 0;
  if (*str == '0' && *base != 10) {
    if (str[1] == 'x' || str[1] == 'X') {
      basis = 16;
      str += 2;
    } else {
      basis = 8;
      str++;
    }
  } else {
    basis = 10;
  }
  *base = *base ? *base : basis;
  return str;
}

const char *s21_parse_float(const char *str, va_list *args, const Options *opts,
                            int *count) {
  long double value = 0;
  const char *endptr = s21_handle_inf_nan(str, &value);
  if (!endptr) value = s21_strtold(str, &endptr, opts->width);

  if (endptr > str && !opts->suppress) {
    if (opts->length_long_double) {
      long double *ptr = va_arg(*args, long double *);
      *ptr = (long double)value;
    } else {
      float *ptr = va_arg(*args, float *);
      *ptr = (float)value;
    }
    (*count)++;
  }
  return endptr;
}

const char *s21_handle_inf_nan(const char *str, long double *out_value) {
  int sign = 1;
  int recognized = 0;
  if (*str == '+') {
    str++;
  } else if (*str == '-') {
    sign = -1;
    str++;
  }

  if ((s21_strncmp(str, "inf", 3) == 0) || (s21_strncmp(str, "INF", 3) == 0)) {
    str += 3;
    if ((s21_strncmp(str, "inity", 5) == 0) ||
        (s21_strncmp(str, "INITY", 5) == 0)) {
      str += 5;
    }
    *out_value = sign * INFINITY;
    recognized = 1;
  }

  if ((s21_strncmp(str, "nan", 3) == 0) || (s21_strncmp(str, "NAN", 3) == 0)) {
    str += 3;
    *out_value = NAN;
    recognized = 1;
  }

  if ((s21_strncmp(str, "(nil)", 5) == 0)) {
    str += 5;
    recognized = 1;
  }

  if ((s21_strncmp(str, "0x0", 3) == 0) || (s21_strncmp(str, "0X0", 3) == 0)) {
    str += 3;
    *out_value = 0.0;
    recognized = 1;
  }

  return recognized ? str : s21_NULL;
}

long double s21_strtold(const char *str, const char **endptr,
                        s21_size_t width) {
  const char *string = str;
  long double value = 0.0;
  int sign = 1;
  long double fraction_factor = 1.0;
  char buf[MAX_DIGITS + 1] = {0};
  width = width >= MAX_DIGITS ? MAX_DIGITS : width;
  if (width) {
    string = buf;
    s21_strncpy(buf, str, width);
  }

  string = s21_check_sign(string, &sign);

  while (isdigit(*string)) {
    value = value * 10.0 + (*string - '0');
    string++;
  }

  if (*string == '.') {
    string++;
    while (isdigit(*string)) {
      value = value * 10.0 + (*string - '0');
      fraction_factor *= 0.1;
      string++;
    }
  }

  value = sign * value * fraction_factor;
  string = s21_processing_exp(string, &value);

  for (s21_size_t i = 0; *str && *str != ' ' && i < width; i++) str++;
  *endptr = width ? str : string;

  return value;
}

const char *s21_processing_exp(const char *string, long double *value) {
  int exponent_sign = 1;
  int exponent = 0;
  if (*string == 'e' || *string == 'E') {
    string++;
    string = s21_check_sign(string, &exponent_sign);

    while (isdigit(*string)) {
      exponent = exponent * 10 + (*string - '0');
      string++;
    }
  }

  if (exponent != 0) {
    long double power = powl(10.0, exponent);
    *value *= (exponent_sign == 1) ? power : 1.0 / power;
  }
  return string;
}

const char *s21_parse_pointer(const char *str, va_list *args,
                              const Options *opts, int *count) {
  long double recognized = 0.0;
  unsigned long long value = 0;
  const char *endptr = s21_handle_inf_nan(str, &recognized);
  if (!endptr) value = s21_strtoll(str, &endptr, 16, opts->width);

  if (endptr > str && !opts->suppress) {
    void **ptr = va_arg(*args, void **);
    *ptr = (void *)value;
    (*count)++;
  }

  return endptr;
}