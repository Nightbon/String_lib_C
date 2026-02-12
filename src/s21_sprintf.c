#include "s21_string.h"

const char *s21_parser_length(const char *format, Options *opts);
static unsigned long long s21_processing_unsigned(va_list *args,
                                                  const Options *opts,
                                                  const char ch);
static char *s21_processing_arguments(char *res, const char *format,
                                      va_list *args, Options *opts);
static int s21_should_skip_zero(unsigned long long value, const Options *opts,
                                const char ch);
static int s21_add_hex_prefix(char *res, unsigned long long value,
                              Options *opts, char ch);
static char *s21_processing_string_long(char *start_string, va_list *args,
                                        Options *opts);
static void s21_remove_trailing_zeros(char *res, char *start_string,
                                      const Options *opts);
static char *s21_processing_float(char *res, long double value, Options *opts,
                                  char ch);
static char *s21_processing_string_width(char *res, char *start_string,
                                         Options *opts);
static char *s21_add_leading_zeros(char *res, char *start_string,
                                   const Options *opts);
static const char *s21_parser_precision(const char *format, va_list *args,
                                        Options *opts);
static char *s21_processing_number(char *res, va_list *args, Options *opts,
                                   const char ch);
static const char *s21_parser_width(const char *format, va_list *args,
                                    Options *opts);
static char *s21_uint_to_str(char *res, unsigned long long value,
                             const char ch);
static char *s21_float_to_str(char *res, long double value,
                              const Options *opts);
static long long int s21_processing_int(va_list *args, const Options *opts);
static char *check_sing_int(char *res, long long int *value, Options *opts);
static char *check_sing_float(char *res, long double *value, Options *opts);
static char *s21_processing_string(char *res, va_list *args, Options *opts);
static int s21_handle_special(long double value, char *res, Options *opts);
static char *s21_processing_char(char *res, va_list *args, Options *opts);
static char *s21_shift_right(char *res, char *start_string, int spaces);
static int s21_calculation_exp(long double *value, const Options *opts);
static const char *s21_parser_flags(const char *format, Options *opts);
static char *s21_add_exp_to_str(char *res, int exp, const char ch);
static int s21_check_format(char **res, const char **format);
static void s21_processing_n(va_list *args, const Options *opts);
static char *s21_reverse(char *res, char *ptr);

int s21_sprintf(char *str, const char *format, ...) {
  setlocale(LC_ALL, "");
  char *res = str;
  va_list args;
  va_start(args, format);

  while (s21_check_format(&res, &format)) {
    Options opts = {0};
    opts.num = (res - str);
    format = s21_parser_flags(format, &opts);
    format = s21_parser_width(format, &args, &opts);
    format = s21_parser_precision(format, &args, &opts);
    format = s21_parser_length(format, &opts);
    res = s21_processing_arguments(res, format, &args, &opts);
    format++;
  }
  *res = '\0';
  va_end(args);
  return (int)(res - str);
}

int s21_check_format(char **res, const char **format) {
  if (**format == '\0') {
    return 0;
  }
  int result = 0;
  while (**format != '%' && **format != '\0') {
    **res = **format;
    (*res)++;
    (*format)++;
  }
  (*format) += (**format != '\0');
  if (**format != '\0') {
    if (**format == '%') {
      **res = **format;
      (*res)++;
    }
    result = 1;
  }

  return result;
}

char *s21_processing_arguments(char *res, const char *format, va_list *args,
                               Options *opts) {
  if (s21_strchr("XEG", *format)) {
    opts->upper = 1;
  }
  if (*format == 'c') {
    res = s21_processing_char(res, args, opts);
  } else if (*format == 's') {
    res = s21_processing_string(res, args, opts);
  } else if (s21_strchr("dfeEgGxXpoui", *format)) {
    res = s21_processing_number(res, args, opts, *format);
  } else if (*format == 'n') {
    s21_processing_n(args, opts);
  }
  *res = '\0';
  return res;
}

void s21_processing_n(va_list *args, const Options *opts) {
  if (opts->length_short) {
    short *ptr = va_arg(*args, short *);
    if (ptr) {
      *ptr = (short)opts->num;
    }
  } else if (opts->length_long) {
    long *ptr = va_arg(*args, long *);
    if (ptr) {
      *ptr = (long)opts->num;
    }
  } else {
    int *ptr = va_arg(*args, int *);
    if (ptr) {
      *ptr = (int)opts->num;
    }
  }
}

char *s21_processing_char(char *res, va_list *args, Options *opts) {
  char *start_string = res;
  if (opts->length_long) {
    int bytes = wcrtomb(res, va_arg(*args, wchar_t), s21_NULL);
    res += bytes > 0 ? bytes : 0;
  } else {
    *res++ = (char)va_arg(*args, int);
  }
  return s21_processing_string_width(res, start_string, opts);
}

char *s21_processing_number(char *res, va_list *args, Options *opts,
                            const char ch) {
  char *start_string = res;

  if (s21_strchr("feEgG", ch)) {
    if (opts->length_long_double) {
      res = s21_processing_float(res, va_arg(*args, long double), opts, ch);
    } else {
      res = s21_processing_float(res, va_arg(*args, double), opts, ch);
    }
  }
  if (s21_strchr("diuxXpo", ch)) {
    int hex = 0;
    unsigned long long value;
    if (ch == 'd' || ch == 'i') {
      long long int value_int = s21_processing_int(args, opts);
      res = check_sing_int(res, &value_int, opts);
      value = (unsigned long long)value_int;
    } else {
      value = s21_processing_unsigned(args, opts, ch);
      hex = s21_add_hex_prefix(res, value, opts, ch);
      res += hex;
    }
    if (!s21_should_skip_zero(value, opts, ch)) {
      res = s21_uint_to_str(res, value, ch);
      res = s21_add_leading_zeros(res, start_string + hex, opts);
    }
  }
  return s21_processing_string_width(res, start_string, opts);
}

char *check_sing_int(char *res, long long int *value, Options *opts) {
  if (*value < 0) {
    *value = -(*value);
    *res++ = '-';
  } else if (opts->plus) {
    *res++ = '+';
  } else if (opts->space) {
    *res++ = ' ';
  }
  opts->space = 0;
  opts->plus = 0;
  return res;
}

int s21_add_hex_prefix(char *res, unsigned long long value, Options *opts,
                       char ch) {
  int hex = 0;
  const char *null_spec = (ch == 'X') ? "0X" : "0x";
  if (s21_strchr("xXop", ch) && (opts->hashtag || ch == 'p') && value) {
    hex = ch == 'o' ? 1 : 2;
    opts->precision -= (ch == 'o');
  }

  if (ch == 'p' && !value) {
#ifdef __linux__
    null_spec = "(nil)";
#else
    null_spec = "0x0";
#endif
    hex = s21_strlen(null_spec);
  }

  if (s21_strchr("xXop", ch)) {
    s21_strncpy(res, null_spec, hex);
  }

  return hex;
}

int s21_should_skip_zero(unsigned long long value, const Options *opts,
                         const char ch) {
  int precission_and_value_null = (opts->dot && !opts->precision && !value);
  int ptr_null = (ch == 'p' && !value);
  int specifier_o = (ch == 'o' && !opts->hashtag);
  int specifier_xXdi = (s21_strchr("xXdi", ch) != s21_NULL);
  int specifier = (specifier_xXdi || specifier_o);
  return ((precission_and_value_null && specifier) || ptr_null);
}

char *s21_processing_float(char *res, long double value, Options *opts,
                           char ch) {
  opts->precision = opts->dot ? opts->precision : 6;
  int spec = s21_handle_special(value, res, opts);
  if (!spec) {
    res = check_sing_float(res, &value, opts);
    long double mantissa = value;
    int exp = s21_calculation_exp(&mantissa, opts);
    int use_exp_format = 0;
    if (ch == 'g' || ch == 'G') {
      opts->remove_zeros = 1;
      int precision = opts->precision == 0 ? 1 : opts->precision;
      use_exp_format = (exp < -4) || (exp >= precision);
      opts->precision = use_exp_format ? precision - 1 : precision - (exp + 1);
      opts->precision = opts->precision > 18 ? 18 : opts->precision;
    }
    if (use_exp_format) {
      ch = ch == 'g' ? 'e' : 'E';
    }
    if (s21_strchr("eE", ch)) {
      res = s21_float_to_str(res, mantissa, opts);
      res = s21_add_exp_to_str(res, exp, ch);
    } else {
      res = s21_float_to_str(res, value, opts);
    }
  }
  return res + spec;
}

int s21_calculation_exp(long double *value, const Options *opts) {
  int exp = 0;
  if (*value) {
    exp = (int)floorl(log10l(*value));
    *value /= pow(10, exp);
    *value =
        roundl(*value * powl(10, opts->precision)) / powl(10, opts->precision);
    if (*value >= 10) {
      *value /= 10;
      exp += 1;
    }
  }
  return exp;
}

char *s21_float_to_str(char *res, long double value, const Options *opts) {
  value = roundl(value * powl(10, opts->precision)) / powl(10, opts->precision);
  long double int_part = 0;
  unsigned long long float_part =
      roundl(modfl(value, &int_part) * powl(10, opts->precision));

  res = s21_uint_to_str(res, int_part, 'u');

  if (opts->precision || float_part || opts->hashtag) {
    char buf[MAX_DIGITS] = {0};
    char *ptr = buf;
    *ptr++ = '.';
    for (s21_size_t i = 0; i < opts->precision; i++) {
      *ptr++ = (float_part % 10) + '0';
      float_part /= 10;
    }
    ptr = s21_reverse(buf + 1, ptr);
    s21_remove_trailing_zeros(ptr, buf, opts);
    s21_strncpy(res, buf, s21_strlen(buf));
    res += s21_strlen(buf);
  }
  return res;
}

static int s21_handle_special(long double value, char *res, Options *opts) {
  int result = 0;
  if (isnan(value) || isinf((double)value)) {
    const char *start = res;
    const char *nan_str = isnan(value) ? (opts->upper ? "NAN" : "nan")
                                       : (opts->upper ? "INF" : "inf");
#ifdef __linux__
    res = check_sing_float(res, &value, opts);
#else
    if (!isnan(value)) {
      res = check_sing_float(res, &value, opts);
    }
#endif
    s21_strncpy(res, nan_str, 4);
    result = (res - start) + 3;
  }
  return result;
}

char *check_sing_float(char *res, long double *value, Options *opts) {
  if (signbit(*value)) {
    *value = -(*value);
    *res++ = '-';
  } else if (opts->plus) {
    *res++ = '+';
  } else if (opts->space) {
    *res++ = ' ';
  }
  opts->space = 0;
  opts->plus = 0;
  return res;
}

long long int s21_processing_int(va_list *args, const Options *opts) {
  long long int value = 0;

  if (opts->length_long) {
    value = va_arg(*args, long int);
  } else {
    value = va_arg(*args, int);
  }

  return value;
}

unsigned long long s21_processing_unsigned(va_list *args, const Options *opts,
                                           const char ch) {
  unsigned long long value = 0;
  if (ch == 'p') {
    value = (long long int)va_arg(*args, void *);
  } else {
    if (opts->length_long) {
      value = va_arg(*args, unsigned long int);
    } else {
      value = va_arg(*args, unsigned int);
    }
  }
  return value;
}

char *s21_add_leading_zeros(char *res, char *start_string,
                            const Options *opts) {
  if (s21_strchr("-+ ", *start_string)) {
    start_string++;
  }
  s21_size_t len = res - start_string;
  int spaces = opts->precision - len;
  if (opts->dot && opts->precision > len) {
    res = s21_shift_right(res, start_string, spaces);
    s21_memset(start_string, '0', spaces);
  }
  *res = '\0';
  return res;
}

void s21_remove_trailing_zeros(char *res, char *start_string,
                               const Options *opts) {
  if (!opts->hashtag && opts->remove_zeros) {
    char *end = res - 1;
    while (end >= start_string && (*end == '0' || *end == '.')) {
      end--;
    }
    res = ++end;
    *res = '\0';
  }
}

char *s21_processing_string(char *res, va_list *args, Options *opts) {
  char *start_string = res;
  if (opts->length_long) {
    res = s21_processing_string_long(start_string, args, opts);
  } else {
    const char *ptr = va_arg(*args, char *);
    s21_size_t len = s21_strlen(ptr);
    if (opts->dot && opts->precision < len) {
      len = opts->precision;
    }
    s21_strncpy(res, ptr, len);
    res += len;
  }
  return s21_processing_string_width(res, start_string, opts);
}

char *s21_processing_string_long(char *start_string, va_list *args,
                                 Options *opts) {
  char *res = start_string;
  const wchar_t *ptr = va_arg(*args, wchar_t *);
  int precision = !opts->dot || opts->precision;
  char buf[MAX_DIGITS] = {0};
  for (s21_size_t i = 0; ptr[i] != L'\0' && precision; i++) {
    mbstate_t state = {0};
    int bytes = wcrtomb(buf, ptr[i], &state);
    bytes = bytes > 0 ? bytes : 0;
    if (opts->dot &&
        ((res + bytes) - start_string) > (long int)opts->precision) {
      precision = 0;
    } else {
      s21_strncpy(res, buf, bytes);
      res += bytes;
    }
  }
  *res = '\0';
  return res;
}

char *s21_processing_string_width(char *res, char *start_string,
                                  Options *opts) {
  s21_size_t len = res - start_string;
  if (opts->width > len) {
    int spaces = opts->width - len;
    if (opts->minus) {
      s21_memset(start_string + len, ' ', spaces);
      res += spaces;
    } else {
      start_string += (s21_strchr("-+ ", *start_string) && opts->null);
      res = s21_shift_right(res, start_string, spaces);
      char ch = opts->null ? '0' : ' ';
      s21_memset(start_string, ch, spaces);
    }
  }
  *res = '\0';
  return res;
}

char *s21_shift_right(char *res, char *start_string, int spaces) {
  for (int i = (res - start_string); i >= 0; i--) {
    start_string[i + spaces] = start_string[i];
  }
  return res + spaces;
}

char *s21_uint_to_str(char *res, unsigned long long value, char ch) {
  const char *digits = (ch == 'X') ? "0123456789ABCDEF" : "0123456789abcdef";
  int base = (ch == 'o') ? 8 : (s21_strchr("xXp", ch) ? 16 : 10);
  char *ptr = res;
  do {
    *ptr++ = digits[value % base];
    value /= base;
  } while (value);
  return s21_reverse(res, ptr);
}

char *s21_reverse(char *res, char *ptr) {
  char *start = res;
  char *end = ptr - 1;

  while (start < end) {
    char temp = *start;
    *start = *end;
    *end = temp;
    start++;
    end--;
  }
  return ptr;
}

char *s21_add_exp_to_str(char *res, int exp, const char ch) {
  *res++ = ch;
  *res++ = (exp >= 0) ? '+' : '-';
  int abs_exp = exp >= 0 ? exp : -exp;
  if (abs_exp < 10) {
    *res++ = '0';
    *res++ = abs_exp + '0';
  } else if (abs_exp < 100) {
    *res++ = (abs_exp / 10) + '0';
    *res++ = (abs_exp % 10) + '0';
  } else {
    *res++ = (abs_exp / 100) + '0';
    *res++ = ((abs_exp / 10) % 10) + '0';
    *res++ = (abs_exp % 10) + '0';
  }
  *res = '\0';

  return res;
}

const char *s21_parser_flags(const char *format, Options *opts) {
  while (s21_strchr("-+ 0#", *format)) {
    if (*format == '-') {
      opts->minus = 1;
    }
    if (*format == '+') {
      opts->plus = 1;
    }
    if (*format == ' ') {
      opts->space = 1;
    }
    if (*format == '0') {
      opts->null = 1;
    }
    if (*format == '#') {
      opts->hashtag = 1;
    }
    format++;
  }
  return format;
}

const char *s21_parser_width(const char *format, va_list *args, Options *opts) {
  int n = 0;
  if (*format == '*') {
    int width = va_arg(*args, int);
    opts->minus += (width < 0);
    opts->width = (width < 0) ? -width : width;
    format++;
  } else {
    s21_sscanf(format, "%llu%n", &opts->width, &n);
  }
  return format + n;
}

const char *s21_parser_precision(const char *format, va_list *args,
                                 Options *opts) {
  int n = 0;
  opts->dot += (*format == '.');
  format += opts->dot;
  if (*format == '*') {
    int star = va_arg(*args, int);
    opts->precision = star > 0 ? star : 0;
    opts->dot = (star >= 0);
    format++;
  } else {
    s21_sscanf(format, "%llu%n", &opts->precision, &n);
  }

  return format + n;
}