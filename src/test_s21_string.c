#include <check.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>

#include "s21_string.h"

#if defined(__APPLE__)
#define MAX_KNOWN_ERROR 106
#elif defined(__linux__)
#define MAX_KNOWN_ERROR 134
#else
#define MAX_KNOWN_ERROR 134
#endif

START_TEST(test_s21_memchr) {
  char dest[20] = "\r\t\0Hello";
  ck_assert_str_eq(s21_memchr(dest, 'l', 8), memchr(dest, 'l', 8));
  ck_assert_str_eq(s21_memchr(dest, 'l', 6), memchr(dest, 'l', 6));
  ck_assert_ptr_eq(s21_memchr(dest, '\0', 5), memchr(dest, '\0', 5));
}
END_TEST

START_TEST(test_s21_memcmp) {
  const char dest[20] = "\r\t\0Hello";
  ck_assert_int_eq(s21_memcmp(dest, "\r\t\0Hello", 8),
                   memcmp(dest, "\r\t\0Hello", 8));
  ck_assert_int_ge(s21_memcmp(dest, "", 1), memcmp(dest, "", 1));
  ck_assert_int_eq(s21_memcmp(dest, "\r\t\0Herrr", 5),
                   memcmp(dest, "\r\t\0Herrr", 5));
}
END_TEST

START_TEST(test_s21_memcpy) {
  char dest1[10] = {'\0'};
  char dest2[10] = {'\0'};
  ck_assert_str_eq(s21_memcpy(dest1, "\r\t\0Hello", 8),
                   memcpy(dest2, "\r\t\0Hello", 8));
  ck_assert_str_eq(s21_memcpy(dest1, "", 1), memcpy(dest2, "", 1));
  ck_assert_str_eq(s21_memcpy(dest1, "hello", 6), memcpy(dest2, "hello", 6));
}
END_TEST

START_TEST(test_s21_memset) {
  char dest1[10] = {'\0'};
  char dest2[10] = {'\0'};
  ck_assert_str_eq(s21_memset(dest1, 't', 8), memset(dest2, 't', 8));
  ck_assert_mem_eq(s21_memset(dest1, '\0', 10), memset(dest2, '\0', 10), 10);
  ck_assert_mem_eq(s21_memset(dest1, '\n', 6), memset(dest2, '\n', 6), 10);
}
END_TEST

START_TEST(test_s21_strncat) {
  char dest1[20] = "Hello";
  char dest2[20] = "Hello";
  ck_assert_str_eq(s21_strncat(dest1, " World", 7),
                   strncat(dest2, " World", 7));
  char empty1[10] = "";
  char empty2[10] = "";
  ck_assert_str_eq(s21_strncat(empty1, "test", 5), strncat(empty2, "test", 5));
  ck_assert_str_eq(s21_strncat(dest1, " Wo", 7), strncat(dest2, " Wo", 7));
  ck_assert_str_eq(s21_strncat(dest1, "ttt", 1), strncat(dest2, "ttt", 1));
}
END_TEST

START_TEST(test_s21_strchr) {
  const char dest[20] = "Hello";
  ck_assert_str_eq(s21_strchr(dest, 'l'), strchr(dest, 'l'));
  ck_assert_ptr_eq(s21_strchr(dest, 'l'), strchr(dest, 'l'));
  ck_assert_str_eq(s21_strchr(dest, '\0'), strchr(dest, '\0'));
}
END_TEST

START_TEST(test_s21_strncmp) {
  char dest[20];
  sprintf(dest, "%s %s", "hello", "privet");
  ck_assert_int_eq(s21_strncmp(dest + 12, dest + 12, 1),
                   strncmp(dest + 12, dest + 12, 1));
  ck_assert_int_eq(s21_strncmp(dest, "hello", 5), strncmp(dest, "hello", 5));
  ck_assert_int_le(s21_strncmp(dest, dest + 6, 6), strncmp(dest, dest + 6, 6));
  ck_assert_int_ge(s21_strncmp(dest + 6, dest, 6), strncmp(dest + 6, dest, 6));
  ck_assert_int_eq(s21_strncmp(dest + 6, dest, 0), strncmp(dest + 6, dest, 0));
  ck_assert_int_eq(s21_strncmp("hgllo", dest, 1), strncmp("hgllo", dest, 1));
}
END_TEST

START_TEST(test_s21_strncpy) {
  char dest1[10] = {'\0'};
  char dest2[10] = {'\0'};
  ck_assert_str_eq(s21_strncpy(dest1, "", 1), strncpy(dest2, "", 1));
  ck_assert_str_eq(s21_strncpy(dest1, "hello", 6), strncpy(dest2, "hello", 6));
}
END_TEST

START_TEST(test_s21_strcspn) {
  const char dest[20] = "Hello world";
  ck_assert_uint_eq(s21_strcspn(dest, ""), strcspn(dest, ""));
  ck_assert_uint_eq(s21_strcspn(dest, "hello"), strcspn(dest, "hello"));
}
END_TEST

START_TEST(test_s21_strerror) {
  for (int err = 0; err < MAX_KNOWN_ERROR; err++) {
    if (err != 41 && err != 58) {
      const char *expected = strerror(err);
      const char *actual = s21_strerror(err);
      ck_assert_str_eq(actual, expected);
    }
  }

  int err = -1;
#ifdef __linux__
  const char *expected_linux = "Unknown error -1";
#else
  const char *expected_linux = "Unknown error: -1";
#endif
  const char *actual = s21_strerror(err);
  ck_assert_str_eq(actual, expected_linux);
}
END_TEST

START_TEST(test_s21_strlen) {
  ck_assert_uint_eq(s21_strlen(""), strlen(""));
  ck_assert_uint_eq(s21_strlen("hello"), strlen("hello"));
}
END_TEST

START_TEST(test_s21_strpbrk) {
  const char dest[20] = "Hello";
  ck_assert_str_eq(s21_strpbrk(dest, "eot"), strpbrk(dest, "eot"));
  ck_assert_ptr_eq(s21_strpbrk(dest, "let"), strpbrk(dest, "let"));
  ck_assert_ptr_eq(s21_strpbrk(dest, ""), strpbrk(dest, ""));
}
END_TEST

START_TEST(test_s21_strrchr) {
  const char dest[20] = "Hello";
  ck_assert_str_eq(s21_strrchr(dest, 'l'), strrchr(dest, 'l'));
  ck_assert_ptr_eq(s21_strrchr(dest, 'l'), strrchr(dest, 'l'));
  ck_assert_str_eq(s21_strrchr(dest, '\0'), strrchr(dest, '\0'));
}
END_TEST

START_TEST(test_s21_strstr) {
  char dest[20] = "Hello world";
  ck_assert_str_eq(s21_strstr(dest, "llo"), strstr(dest, "llo"));
  ck_assert_ptr_eq(s21_strstr(dest, "ttt"), strstr(dest, "ttt"));
  ck_assert_str_eq(s21_strstr(dest, ""), strstr(dest, ""));
}
END_TEST

START_TEST(test_s21_strtok) {
  char dest1[20] = "..Hello,world.Hay\0";
  char dest2[20] = "..Hello,world.Hay\0";

  char *token = strtok(dest1, ".,");
  char *s21_token = s21_strtok(dest2, ".,");
  while (token != NULL && s21_token != s21_NULL) {
    ck_assert_str_eq(s21_token, token);
    token = strtok(NULL, ".,");
    s21_token = s21_strtok(s21_NULL, ".,");
  }
  ck_assert_ptr_eq(s21_token, token);
  ck_assert_ptr_eq(s21_strtok(dest2, ""), strtok(dest2, ""));
}
END_TEST

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
START_TEST(test_sscanf_min_max_values) {
  int res1, res2;
  int d1, d2;
  short hd1, hd2;
  long ld1, ld2;

  res1 = sscanf("-2147483648", "%d", &d1);
  res2 = s21_sscanf("-2147483648", "%d", &d2);
  ck_assert_int_eq(d1, d2);
  ck_assert_int_eq(res1, res2);

  res1 = sscanf("2147483647", "%d", &d1);
  res2 = s21_sscanf("2147483647", "%d", &d2);
  ck_assert_int_eq(d1, d2);
  ck_assert_int_eq(res1, res2);

  res1 = sscanf("-32768", "%hd", &hd1);
  res2 = s21_sscanf("-32768", "%hd", &hd2);
  ck_assert_int_eq(hd1, hd2);
  ck_assert_int_eq(res1, res2);

  res1 = sscanf("32767", "%hd", &hd1);
  res2 = s21_sscanf("32767", "%hd", &hd2);
  ck_assert_int_eq(hd1, hd2);
  ck_assert_int_eq(res1, res2);

  res1 = sscanf("-9223372036854775808", "%ld", &ld1);
  res2 = s21_sscanf("-9223372036854775808", "%ld", &ld2);
  ck_assert_int_eq(ld1, ld2);
  ck_assert_int_eq(res1, res2);

  res1 = sscanf("9223372036854775807", "%ld", &ld1);
  res2 = s21_sscanf("9223372036854775807", "%ld", &ld2);
  ck_assert_int_eq(ld1, ld2);
  ck_assert_int_eq(res1, res2);

  unsigned int u1, u2;
  unsigned short hu1, hu2;
  unsigned long lu1, lu2;
  unsigned int x1, x2;

  res1 = sscanf("0", "%u", &u1);
  res2 = s21_sscanf("0", "%u", &u2);
  ck_assert_uint_eq(u1, u2);
  ck_assert_int_eq(res1, res2);

  res1 = sscanf("4294967295", "%u", &u1);
  res2 = s21_sscanf("4294967295", "%u", &u2);
  ck_assert_uint_eq(u1, u2);
  ck_assert_int_eq(res1, res2);

  res1 = sscanf("0", "%ho", &hu1);
  res2 = s21_sscanf("0", "%ho", &hu2);
  ck_assert_uint_eq(hu1, hu2);
  ck_assert_int_eq(res1, res2);

  res1 = sscanf("65535", "%ho", &hu1);
  res2 = s21_sscanf("65535", "%ho", &hu2);
  ck_assert_uint_eq(hu1, hu2);
  ck_assert_int_eq(res1, res2);

  res1 = sscanf("0", "%lo", &lu1);
  res2 = s21_sscanf("0", "%lo", &lu2);
  ck_assert_uint_eq(lu1, lu2);
  ck_assert_int_eq(res1, res2);

  res1 = sscanf("18446744073709551615", "%lo", &lu1);
  res2 = s21_sscanf("18446744073709551615", "%lo", &lu2);
  ck_assert_uint_eq(lu1, lu2);
  ck_assert_int_eq(res1, res2);

  res1 = sscanf("0", "%x", &x1);
  res2 = s21_sscanf("0", "%x", &x2);
  ck_assert_uint_eq(x1, x2);
  ck_assert_int_eq(res1, res2);

  res1 = sscanf("ffffffff", "%x", &x1);
  res2 = s21_sscanf("ffffffff", "%x", &x2);
  ck_assert_uint_eq(x1, x2);
  ck_assert_int_eq(res1, res2);

  char c1 = 0, c2 = 0;
  char s1[10] = {0}, s2[10] = {0};
  wchar_t ws1[10] = {0}, ws2[10] = {0};

  res1 = sscanf("A", "%c", &c1);
  res2 = s21_sscanf("A", "%c", &c2);
  ck_assert_int_eq(c1, c2);
  ck_assert_int_eq(res1, res2);

  res1 = sscanf("Hello", "%9s", s1);
  res2 = s21_sscanf("Hello", "%9s", s2);
  ck_assert_str_eq(s1, s2);
  ck_assert_int_eq(res1, res2);

  res1 = sscanf("Привет", "%9ls", ws1);
  res2 = s21_sscanf("Привет", "%9ls", ws2);
  ck_assert(wcscmp(ws1, ws2) == 0);
  ck_assert_int_eq(res1, res2);

  float f1, f2;

  res1 = sscanf("-3.4e38", "%f", &f1);
  res2 = s21_sscanf("-3.4e38", "%f", &f2);
  ck_assert_float_eq(f1, f2);
  ck_assert_int_eq(res1, res2);

  long double val1, val2;
  int r1 = sscanf("-1.1e4932", "%Lf", &val1);
  int r2 = s21_sscanf("-1.1e4932", "%Lf", &val2);

  ck_assert_int_eq(r1, r2);

  void *p1 = NULL;
  void *p2 = NULL;

  res1 = sscanf("0x0", "%p", &p1);
  res2 = s21_sscanf("0x0", "%p", &p2);
  ck_assert_ptr_eq(p1, p2);
  ck_assert_int_eq(res1, res2);

  res1 = sscanf("0xffffffffffffffff", "%p", &p1);
  res2 = s21_sscanf("0xffffffffffffffff", "%p", &p2);
  ck_assert_ptr_eq(p1, p2);
  ck_assert_int_eq(res1, res2);

  int n1 = -1, n2 = -1;
  res1 = sscanf("12345", "%3d%n", &d1, &n1);
  res2 = s21_sscanf("12345", "%3d%n", &d2, &n2);
  ck_assert_int_eq(n1, n2);
  ck_assert_int_eq(res1, res2);
}
END_TEST

START_TEST(test_sscanf_d_i) {
  int a1, a2, a3, a4, a5, a6;
  int b1, b2, b3, b4, b5, b6;

  const char *input = "123 -456 +789 10 00012 34";
  int res1 =
      s21_sscanf(input, "%d %d %d %d %d %d", &a1, &a2, &a3, &a4, &a5, &a6);
  int res2 = sscanf(input, "%d %d %d %d %d %d", &b1, &b2, &b3, &b4, &b5, &b6);
  ck_assert_int_eq(a1, b1);
  ck_assert_int_eq(a2, b2);
  ck_assert_int_eq(a3, b3);
  ck_assert_int_eq(a4, b4);
  ck_assert_int_eq(a5, b5);
  ck_assert_int_eq(a6, b6);
  ck_assert_int_eq(res1, res2);

  const char *input2 = "100 +200 -300";
  res1 = s21_sscanf(input2, "%i %i %i", &a1, &a2, &a3);
  res2 = sscanf(input2, "%i %i %i", &b1, &b2, &b3);
  ck_assert_int_eq(res1, res2);
  ck_assert_int_eq(a1, b1);
  ck_assert_int_eq(a2, b2);
  ck_assert_int_eq(a3, b3);

  const char *edge_input = "2147483647 -2147483648 0";
  res1 = s21_sscanf(edge_input, "%d %d %d", &a1, &a2, &a3);
  res2 = sscanf(edge_input, "%d %d %d", &b1, &b2, &b3);
  ck_assert_int_eq(res1, res2);
  ck_assert_int_eq(a1, INT_MAX);
  ck_assert_int_eq(a2, INT_MIN);
  ck_assert_int_eq(a3, 0);
}
END_TEST

START_TEST(test_sscanf_length_modifiers) {
  long l1, l2;
  short s1, s2;
  unsigned long ul1, ul2;
  unsigned short us1, us2;

  const char *input = "1234567890 -12345 4294967295 65535";
  int res1 = s21_sscanf(input, "%ld %hd %lu %hu", &l1, &s1, &ul1, &us1);
  int res2 = sscanf(input, "%ld %hd %lu %hu", &l2, &s2, &ul2, &us2);
  ck_assert_int_eq(res1, res2);
  ck_assert_int_eq(l1, l2);
  ck_assert_int_eq(s1, s2);
  ck_assert_int_eq(ul1, ul2);
  ck_assert_int_eq(us1, us2);

  char edge_input[100];
  sprintf(edge_input, "%ld %hd", (long)LONG_MIN, (short)SHRT_MAX);
  res1 = s21_sscanf(edge_input, "%ld %hd", &l1, &s1);
  res2 = sscanf(edge_input, "%ld %hd", &l2, &s2);
  ck_assert_int_eq(res1, res2);
  ck_assert_int_eq(l1, LONG_MIN);
  ck_assert_int_eq(s1, SHRT_MAX);

  sprintf(edge_input, "%lu %hu", (unsigned long)ULONG_MAX,
          (unsigned short)USHRT_MAX);
  res1 = s21_sscanf(edge_input, "%lu %hu", &ul1, &us1);
  res2 = sscanf(edge_input, "%lu %hu", &ul2, &us2);
  ck_assert_int_eq(res1, res2);
  ck_assert_int_eq(ul1, ULONG_MAX);
  ck_assert_int_eq(us1, USHRT_MAX);
}
END_TEST

START_TEST(test_sscanf_uoxX) {
  unsigned u1, u2;
  unsigned o1, o2;
  unsigned x1, x2;
  unsigned X1, X2;

  const char *input = "4294967295 65 ff FF";
  int res1 = s21_sscanf(input, "%u %o %x %X", &u1, &o1, &x1, &X1);
  int res2 = sscanf(input, "%u %o %x %X", &u2, &o2, &x2, &X2);
  ck_assert_uint_eq(u1, u2);
  ck_assert_uint_eq(o1, o2);
  ck_assert_uint_eq(x1, x2);
  ck_assert_uint_eq(X1, X2);
  ck_assert_int_eq(res1, res2);

  const char *prefixed_input = "0x7f 0XFF 077";
  unsigned x_val1, x_val2, X_val1, X_val2, o_val1, o_val2;
  res1 = s21_sscanf(prefixed_input, "%x %X %o", &x_val1, &X_val1, &o_val1);
  res2 = sscanf(prefixed_input, "%x %X %o", &x_val2, &X_val2, &o_val2);
  ck_assert_int_eq(res1, res2);
  ck_assert_uint_eq(x_val1, x_val2);
  ck_assert_uint_eq(X_val1, X_val2);
  ck_assert_uint_eq(o_val1, o_val2);
}
END_TEST

START_TEST(test_sscanf_fegG) {
  float s21_f1, s21_f2, s21_f3, s21_f4, s21_f5;
  float std_f1, std_f2, std_f3, std_f4, std_f5;
  float s21_g1, s21_g2, s21_g3;
  float std_g1, std_g2, std_g3;

  const char *input = "3.14 -0.0 inf nan +infinity 1.23e-5 1.23e+8 12345.6789";

  int res1 = s21_sscanf(input, "%10f %f %f %f %f %f %f %f", &s21_f1, &s21_f2,
                        &s21_f3, &s21_f4, &s21_f5, &s21_g1, &s21_g2, &s21_g3);
  int res2 = sscanf(input, "%10f %f %f %f %f %f %f %f", &std_f1, &std_f2,
                    &std_f3, &std_f4, &std_f5, &std_g1, &std_g2, &std_g3);

  ck_assert_float_eq(s21_f1, std_f1);
  ck_assert_float_eq(s21_f2, std_f2);
  ck_assert(s21_f2 == std_f2);  // -0.0
  ck_assert_float_infinite(s21_f3);
  ck_assert_float_nan(s21_f4);
  ck_assert_float_infinite(s21_f5);
  ck_assert_float_eq(s21_g1, std_g1);
  ck_assert_float_eq(s21_g2, std_g2);
  ck_assert_float_eq(s21_g3, std_g3);
  ck_assert_int_eq(res1, res2);
}
END_TEST

START_TEST(test_sscanf_scientific_notation) {
  float s21_f1, s21_f2, s21_f3;
  float std_f1, std_f2, std_f3;

  const char *sci_input = "1.23e-10 1.23E+10 -2.5e5";

  int res1 = s21_sscanf(sci_input, "%e %E %g", &s21_f1, &s21_f2, &s21_f3);
  int res2 = sscanf(sci_input, "%e %E %g", &std_f1, &std_f2, &std_f3);

  ck_assert_int_eq(res1, res2);
  ck_assert_float_eq(s21_f1, std_f1);
  ck_assert_float_eq(s21_f2, std_f2);
  ck_assert_float_eq(s21_f3, std_f3);
}
END_TEST

static int ld_eq(long double a, long double b);
START_TEST(test_sscanf_long_double) {
  long double s21_ld1, s21_ld2;
  long double std_ld1, std_ld2;

  const char *ld_input = "3.14159265358979323846 2.71828182845904523536";

  int res1 = s21_sscanf(ld_input, "%20Lf %Lf", &s21_ld1, &s21_ld2);
  int res2 = sscanf(ld_input, "%20Lf %Lf", &std_ld1, &std_ld2);

  ck_assert_int_eq(res1, res2);
  ck_assert(ld_eq(s21_ld1, std_ld1));
  ck_assert(ld_eq(s21_ld2, std_ld2));
}

static int ld_eq(long double a, long double b) { return fabsl(a - b) < 1e-10L; }
END_TEST

START_TEST(test_sscanf_cs) {
  char c1, c2;
  char s1[100] = {0}, s2[100] = {0};
  char s3[100] = {0}, s4[100] = {0};

  const char *input = "A Hello World";
  int res1 = s21_sscanf(input, "%*c %99s %99s", s1, s2);
  int res2 = sscanf(input, "%*c %99s %99s", s3, s4);
  ck_assert_int_eq(res1, res2);
  ck_assert_str_eq(s1, s3);
  ck_assert_str_eq(s2, s4);

  const char *width_input = "1234567890";
  char buf1[5], buf2[5];
  res1 = s21_sscanf(width_input, "%4s", buf1);
  res2 = sscanf(width_input, "%4s", buf2);
  ck_assert_int_eq(res1, res2);
  ck_assert_str_eq(buf1, buf2);

  const char *empty_input = "";
  res1 = s21_sscanf(empty_input, "%c %s", &c1, s1);
  res2 = sscanf(empty_input, "%c %99s", &c2, s3);
  ck_assert_int_eq(res1, res2);
  ck_assert_int_eq(res1, -1);
}
END_TEST

START_TEST(test_sscanf_lc_ls) {
  wchar_t wc1, wc2, wc3;
  wchar_t ws1[100] = {0};
  wchar_t wc4, wc5, wc6;
  wchar_t ws2[100] = {0};

  const char *input = "Ж € 😊 ДлиннаяСтрока";
  int res1 = s21_sscanf(input, "%lc %lc %lc %99ls", &wc1, &wc2, &wc3, ws1);
  int res2 = sscanf(input, "%lc %lc %lc %99ls", &wc4, &wc5, &wc6, ws2);
  ck_assert_int_eq(res1, res2);
  ck_assert_int_eq(res1, 4);
}
END_TEST

START_TEST(test_sscanf_p) {
  char input[100];
  void *p1 = &input, *p2, *p3, *p4, *p5, *p6;
  sprintf(input, "%p %p %s", p1, s21_NULL, "0x0");

  int res1 = s21_sscanf(input, "%p %p %p", &p1, &p3, &p5);
  int res2 = sscanf(input, "%p %p %p", &p2, &p4, &p6);
  ck_assert_ptr_eq(p1, p2);
  ck_assert_ptr_eq(p3, p4);
  ck_assert_ptr_eq(p5, p6);
  ck_assert_int_eq(res1, res2);
}
END_TEST

START_TEST(test_sscanf_n) {
  int n1 = 0, n2 = 0;
  int a1, a2;

  const char *input = "12345";
  int res1 = s21_sscanf(input, "%d%n%n", &a1, &n1, &n1);
  int res2 = sscanf(input, "%d%n%n", &a2, &n2, &n2);
  ck_assert_int_eq(res1, res2);
  ck_assert_int_eq(a1, a2);
  ck_assert_int_eq(n1, n2);
  ck_assert_int_eq(n1, 5);

  long ln1 = 0, ln2 = 0;
  short sn1 = 0, sn2 = 0;
  const char *ln_input = "12345 67890";
  res1 = s21_sscanf(ln_input, "%*d%ln %*d%hn", &ln1, &sn1);
  res2 = sscanf(ln_input, "%*d%ln %*d%hn", &ln2, &sn2);
  ck_assert_int_eq(res1, res2);
  ck_assert_int_eq(ln1, ln2);
  ck_assert_int_eq(sn1, sn2);
}
END_TEST

START_TEST(test_sscanf_percent) {
  int a1, a2, b1, b2;
  char c1, c2;

  const char *input = "100% 50% X";
  int res1 = s21_sscanf(input, "%d%% %d%% %c", &a1, &b1, &c1);
  int res2 = sscanf(input, "%d%% %d%% %c", &a2, &b2, &c2);
  ck_assert_int_eq(a1, a2);
  ck_assert_int_eq(b1, b2);
  ck_assert_int_eq(c1, c2);
  ck_assert_int_eq(res1, res2);
}
END_TEST

START_TEST(test_sscanf_assignment_suppression) {
  int a1, a2;
  float f1, f2;
  char s1[100] = {0}, s2[100] = {0};

  const char *input = "123 3.14 Hello";
  int res1 = s21_sscanf(input, "%*d %f %*s", &f1);
  int res2 = sscanf(input, "%*d %f %*s", &f2);
  ck_assert_int_eq(res1, res2);
  ck_assert_float_eq(f1, f2);

  const char *complex_input = "1 2 3 4 5 6 7 8 ";
  res1 = s21_sscanf(complex_input, "%*d %d %*d %d %*d %f %*d %99s", &a1, &a1,
                    &f1, s1);
  res2 =
      sscanf(complex_input, "%*d %d %*d %d %*d %f %*d %99s", &a2, &a2, &f2, s2);
  ck_assert_int_eq(res1, res2);
}
END_TEST

START_TEST(test_sscanf_empty_input) {
  int a1, a2;
  char c1, c2;

  const char *empty = "";
  int res1 = s21_sscanf(empty, "%d", &a1);
  int res2 = sscanf(empty, "%d", &a2);
  ck_assert_int_eq(res1, res2);
  ck_assert_int_eq(res1, -1);

  const char *partial = "123abc";
  res1 = s21_sscanf(partial, "%d%c", &a1, &c1);
  res2 = sscanf(partial, "%d%c", &a2, &c2);
  ck_assert_int_eq(res1, res2);
  ck_assert_int_eq(a1, a2);
  ck_assert_int_eq(c1, c2);

  const char *no_match = "abc123";
  res1 = s21_sscanf(no_match, "%d", &a1);
  res2 = sscanf(no_match, "%d", &a2);
  ck_assert_int_eq(res1, res2);
  ck_assert_int_eq(res1, 0);
}
END_TEST

START_TEST(test_sscanf_overflow) {
  char buf1[5], buf2[5];
  short s1, s2;

  const char *str_input = "HelloWorld";
  int res1 = s21_sscanf(str_input, "%4s", buf1);
  int res2 = sscanf(str_input, "%4s", buf2);
  ck_assert_int_eq(res1, res2);
  ck_assert_str_eq(buf1, buf2);

  const char *num_input = "99999999999999999999";
  res1 = s21_sscanf(num_input, "%hd", &s1);
  res2 = sscanf(num_input, "%hd", &s2);
  ck_assert_int_eq(res1, res2);
}
END_TEST

START_TEST(test_sscanf_complex_format) {
  int a1, a2, a3, a4;
  float f1, f2;
  char s1[100], s2[100];
  char c1, c2;

  const char *input = "25 Dec 2023 3.14,X";
  int res1 = s21_sscanf(input, "%d %99s %d %f,%c", &a1, s1, &a3, &f1, &c1);
  int res2 = sscanf(input, "%d %99s %d %f,%c", &a2, s2, &a4, &f2, &c2);
  ck_assert_int_eq(res1, res2);
  ck_assert_int_eq(a1, a2);
  ck_assert_str_eq(s1, s2);
  ck_assert_int_eq(a3, a4);
  ck_assert_float_eq(f1, f2);
  ck_assert_int_eq(c1, c2);

  const char *mixed_input = "1234-56.78-ABC";
  res1 = s21_sscanf(mixed_input, "%2d-%4f-%99s", &a1, &f1, s1);
  res2 = sscanf(mixed_input, "%2d-%4f-%99s", &a2, &f2, s2);
  ck_assert_int_eq(res1, res2);
  ck_assert_int_eq(a1, a2);
  ck_assert_float_eq(f1, f2);
  ck_assert_str_eq(s1, s2);
}
END_TEST

START_TEST(test_sscanf_malformed_format) {
  int a1, a2;
  const char *input = "123 456";

  int res1 = s21_sscanf(input, "%d", &a1);
  int res2 = sscanf(input, "%d", &a2);
  ck_assert_int_eq(res1, res2);

  res1 = s21_sscanf(input, "%d %*d", &a1);
  res2 = sscanf(input, "%d %*d", &a2);
  ck_assert_int_eq(res1, res2);
}
END_TEST

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
START_TEST(test_to_upper_1) {
  const char str_src[30] = "Hello, world!";
  char str_res[30] = "HELLO, WORLD!";
  char *res = s21_to_upper(str_src);
  ck_assert_pstr_eq(res, str_res);
  if (res) free(res);
}
END_TEST

START_TEST(test_to_upper_2) {
  const char str_src[30] = "\nH\t\\G123123";
  char str_res[30] = "\nh\t\\g123123";
  char *res = s21_to_lower(str_src);
  ck_assert_pstr_eq(res, str_res);
  if (res) free(res);
}
END_TEST

START_TEST(test_to_upper_3) {
  const char str_src[30] = "ALREADY UPPER";
  char str_res[30] = "ALREADY UPPER";
  char *res = s21_to_upper(str_src);
  ck_assert_pstr_eq(res, str_res);
  if (res) free(res);
}
END_TEST

START_TEST(test_to_upper_4) {
  const char str_src[30] = "";
  char str_res[30] = "";
  char *res = s21_to_upper(str_src);
  ck_assert_pstr_eq(res, str_res);
  if (res) free(res);
}
END_TEST

START_TEST(test_to_upper_5) {
  const char str_src[50] = "abcdefghijklmnopqrstuvwxyz";
  char str_res[50] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  char *res = s21_to_upper(str_src);
  ck_assert_pstr_eq(res, str_res);
  if (res) free(res);
}
END_TEST

START_TEST(test_to_upper_6) {
  const char str_src[30] = "_?};!234";
  char str_res[30] = "_?};!234";
  char *res = s21_to_upper(str_src);
  ck_assert_pstr_eq(res, str_res);
  if (res) free(res);
}
END_TEST

START_TEST(test_to_upper_7) {
  const char *str_src = s21_NULL;
  char *str_res = s21_NULL;
  char *res = s21_to_upper(str_src);
  ck_assert_pstr_eq(res, str_res);
  if (res) free(res);
}
END_TEST

START_TEST(test_to_lower_1) {
  const char str_src[30] = "hELLO, WORLD!";
  char str_res[] = "hello, world!";
  char *res = s21_to_lower(str_src);
  ck_assert_pstr_eq(res, str_res);
  if (res) free(res);
}
END_TEST

START_TEST(test_to_lower_2) {
  const char str_src[30] = "\nH\t\\G123123";
  char str_res[30] = "\nh\t\\g123123";
  char *res = s21_to_lower(str_src);
  ck_assert_pstr_eq(res, str_res);
  if (res) free(res);
}
END_TEST

START_TEST(test_to_lower_3) {
  const char str_src[30] = "already lower";
  char str_res[30] = "already lower";
  char *res = s21_to_lower(str_src);
  ck_assert_pstr_eq(res, str_res);
  if (res) free(res);
}
END_TEST

START_TEST(test_to_lower_4) {
  const char str_src[30] = "";
  char str_res[30] = "";
  char *res = s21_to_lower(str_src);
  ck_assert_pstr_eq(res, str_res);
  if (res) free(res);
}
END_TEST

START_TEST(test_to_lower_5) {
  const char str_src[30] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  char str_res[30] = "abcdefghijklmnopqrstuvwxyz";
  char *res = s21_to_lower(str_src);
  ck_assert_pstr_eq(res, str_res);
  if (res) free(res);
}
END_TEST

START_TEST(test_to_lower_6) {
  const char str_src[30] = "_?};!234";
  char str_res[30] = "_?};!234";
  char *res = s21_to_lower(str_src);
  ck_assert_pstr_eq(res, str_res);
  if (res) free(res);
}
END_TEST

START_TEST(test_to_lower_7) {
  const char *str_src = s21_NULL;
  char *str_res = s21_NULL;
  char *res = s21_to_lower(str_src);
  ck_assert_pstr_eq(res, str_res);
  if (res) free(res);
}
END_TEST

START_TEST(test_insert_1) {
  const char str_src[30] = "hello, world!";
  const char str_ins[] = "hELLO, WORLD!";
  char str_res[] = "hello, hELLO, WORLD!world!";
  s21_size_t num = 7;
  char *res = s21_insert(str_src, str_ins, num);
  ck_assert_pstr_eq(str_res, res);
  if (res) free(res);
}
END_TEST

START_TEST(test_insert_2) {
  const char str_src[30] = "";
  const char str_ins[] = "";
  char *str_res = s21_NULL;
  s21_size_t num = 7;
  char *res = s21_insert(str_src, str_ins, num);
  ck_assert_pstr_eq(str_res, res);
  if (res) free(res);
}
END_TEST

START_TEST(test_insert_3) {
  const char *str_src = s21_NULL;
  const char str_ins[] = "";
  char *str_res = s21_NULL;
  s21_size_t num = 0;
  char *res = s21_insert(str_src, str_ins, num);
  ck_assert_pstr_eq(str_res, res);
  if (res) free(res);
}
END_TEST

START_TEST(test_insert_4) {
  const char str_src[30] = "abcdefghij";
  const char str_ins[] = "\'I WAS HERE\'";
  char str_res[] = "abc\'I WAS HERE\'defghij";
  s21_size_t num = 3;
  char *res = s21_insert(str_src, str_ins, num);
  ck_assert_pstr_eq(str_res, res);
  if (res) free(res);
}
END_TEST

START_TEST(test_insert_5) {
  const char str_src[30] = "abc";
  const char str_ins[] = "333";
  char *str_res = s21_NULL;
  s21_size_t num = 10;
  char *res = s21_insert(str_src, str_ins, num);
  ck_assert_pstr_eq(str_res, res);
  if (res) free(res);
}
END_TEST

START_TEST(test_insert_6) {
  const char str_src[30] = "hello, world!";
  const char str_ins[] = "hELLO, WORLD!";
  char *str_res = s21_NULL;
  s21_size_t num = -1;
  char *res = s21_insert(str_src, str_ins, num);
  ck_assert_pstr_eq(str_res, res);
  if (res) free(res);
}
END_TEST

START_TEST(test_insert_7) {
  const char *str_src = s21_NULL;
  const char *str_ins = s21_NULL;
  char *str_res = s21_NULL;
  s21_size_t num = 0;
  char *res = s21_insert(str_src, str_ins, num);
  ck_assert_pstr_eq(str_res, res);
  if (res) free(res);
}
END_TEST

START_TEST(test_insert_8) {
  const char str_src[30] = "";
  const char str_ins[] = "";
  char str_res[] = "";
  s21_size_t num = 0;
  char *res = s21_insert(str_src, str_ins, num);
  ck_assert_pstr_eq(str_res, res);
  if (res) free(res);
}
END_TEST

START_TEST(test_insert_9) {
  const char str_src[] = "wtf";
  const char *str_ins = s21_NULL;
  char *str_res = s21_NULL;
  s21_size_t num = 0;
  char *res = s21_insert(str_src, str_ins, num);
  ck_assert_pstr_eq(str_res, res);
  if (res) free(res);
}
END_TEST

START_TEST(test_insert_10) {
  const char str_src[] = "";
  const char *str_ins = s21_NULL;
  char *str_res = s21_NULL;
  s21_size_t num = 0;
  char *res = s21_insert(str_src, str_ins, num);
  ck_assert_pstr_eq(res, str_res);
  if (res) free(res);
}
END_TEST

START_TEST(test_trim_1) {
  const char str_src[30] = "-?hello, world!";
  const char str_strim[] = "!?-";
  char str_res[] = "hello, world";
  char *res = s21_trim(str_src, str_strim);
  ck_assert_pstr_eq(str_res, res);
  if (res) free(res);
}
END_TEST

START_TEST(test_trim_2) {
  const char str_src[30] = "MAF MAF";
  const char str_strim[] = "MAF MAF";
  char *str_res = "";
  char *res = s21_trim(str_src, str_strim);
  ck_assert_pstr_eq(str_res, res);
  if (res) free(res);
}
END_TEST

START_TEST(test_trim_3) {
  const char *str_src = s21_NULL;
  const char str_strim[] = "";
  char *str_res = s21_NULL;
  char *res = s21_trim(str_src, str_strim);
  ck_assert_pstr_eq(str_res, res);
  if (res) free(res);
}
END_TEST

START_TEST(test_trim_4) {
  const char str_src[30] = "!!!abcdefghij!?!";
  const char str_strim[] = "!?";
  char str_res[] = "abcdefghij";
  char *res = s21_trim(str_src, str_strim);
  ck_assert_pstr_eq(str_res, res);
  if (res) free(res);
}
END_TEST

START_TEST(test_trim_5) {
  const char str_src[30] = "abc";
  const char str_strim[] = "";
  char *str_res = "abc";
  char *res = s21_trim(str_src, str_strim);
  ck_assert_pstr_eq(str_res, res);
  if (res) free(res);
}
END_TEST

START_TEST(test_trim_6) {
  const char str_src[30] = "hello, world!";
  const char str_strim[] = "?!";
  char *str_res = "hello, world";
  char *res = s21_trim(str_src, str_strim);
  ck_assert_pstr_eq(str_res, res);
  if (res) free(res);
}
END_TEST

START_TEST(test_trim_7) {
  const char *str_src = s21_NULL;
  const char *str_strim = s21_NULL;
  char *str_res = s21_NULL;
  char *res = s21_trim(str_src, str_strim);
  ck_assert_pstr_eq(str_res, res);
  if (res) free(res);
}
END_TEST

START_TEST(test_trim_8) {
  const char str_src[30] = "";
  const char str_strim[] = "";
  char str_res[] = "";
  char *res = s21_trim(str_src, str_strim);
  ck_assert_pstr_eq(str_res, res);
  if (res) free(res);
}
END_TEST

START_TEST(test_trim_9) {
  const char str_src[] = " wtf ";
  const char *str_strim = s21_NULL;
  char *str_res = " wtf ";
  char *res = s21_trim(str_src, str_strim);
  ck_assert_pstr_eq(str_res, res);
  if (res) free(res);
}
END_TEST

START_TEST(test_trim_10) {
  const char str_src[] = " wtf ";
  const char *str_strim = "";
  char *str_res = " wtf ";
  char *res = s21_trim(str_src, str_strim);
  ck_assert_pstr_eq(str_res, res);
  if (res) free(res);
}
END_TEST

START_TEST(test_s21_sprintf_all_unique) {
  char dest1[500], dest2[500];
  int res1, res2;
  wchar_t wc = L'Ж';
  wchar_t wstr[] = L"Привет";
  char *str = "Hello";
  int x = 42;
  void *ptr = &x;
  short hd_val = -32768;
  long ld_val = 2147483647L;
  unsigned short hu_val = 65535U;
  unsigned long lu_val = 4294967295UL;
  double pi = 3.14159265358979323846;
  double lf_val = -0.000123;

  res1 = s21_sprintf(dest1, "%%%%%%");
  res2 = sprintf(dest2, "%%%%%%");
  ck_assert_int_eq(res1, res2);
  ck_assert_str_eq(dest1, dest2);

  res1 = s21_sprintf(dest1, "%c|%-5c|%3c|%c", 'A', 'B', 'C', 'D');
  res2 = sprintf(dest2, "%c|%-5c|%3c|%c", 'A', 'B', 'C', 'D');
  ck_assert_int_eq(res1, res2);
  ck_assert_str_eq(dest1, dest2);

  res1 = s21_sprintf(dest1, "Wide char: %lc", wc);
  res2 = sprintf(dest2, "Wide char: %lc", (wint_t)wc);
  ck_assert_int_eq(res1, res2);
  ck_assert_str_eq(dest1, dest2);

  res1 = s21_sprintf(dest1, "%lc|%-5lc|%3lc", wc, wc, wc);
  res2 = sprintf(dest2, "%lc|%-5lc|%3lc", (wint_t)wc, (wint_t)wc, (wint_t)wc);
  ck_assert_int_eq(res1, res2);
  ck_assert_str_eq(dest1, dest2);

  res1 = s21_sprintf(dest1, "Int: %*d, %i, %+d, % d, %0 5d, %-5d\n", -3, 123,
                     -456, 789, 10, 12, 34);
  res2 = sprintf(dest2, "Int: %*d, %i, %+d, % d, %0 5d, %-5d\n", -3, 123, -456,
                 789, 10, 12, 34);
  ck_assert_str_eq(dest1, dest2);
  ck_assert_int_eq(res1, res2);

  res1 = s21_sprintf(dest1, "%d|%+d|% d|%-8d|%5d|%.5d", -123, 456, 789, -987,
                     32, 7);
  res2 =
      sprintf(dest2, "%d|%+d|% d|%-8d|%5d|%.5d", -123, 456, 789, -987, 32, 7);
  ck_assert_str_eq(dest1, dest2);
  ck_assert_int_eq(res1, res2);

  res1 = s21_sprintf(dest1, "%hd|%ld|%+ld|% hd", hd_val, ld_val, ld_val, 27);
  res2 = sprintf(dest2, "%hd|%ld|%+ld|% hd", (short)hd_val, ld_val, ld_val,
                 (short)27);
  ck_assert_int_eq(res1, res2);
  ck_assert_str_eq(dest1, dest2);

  res1 = s21_sprintf(dest1, "%u|%5u|%-5u|%.5u|%hu|%lu", 123, 456, 789, 7,
                     hu_val, lu_val);
  res2 = sprintf(dest2, "%u|%5u|%-5u|%.5u|%hu|%lu", 123U, 456U, 789U, 7U,
                 hu_val, lu_val);
  ck_assert_str_eq(dest1, dest2);
  ck_assert_int_eq(res1, res2);

  res1 = s21_sprintf(dest1, "%p|%18p|%-16p|%p|%8p", ptr, ptr, ptr, s21_NULL,
                     s21_NULL);
  res2 =
      sprintf(dest2, "%p|%18p|%-16p|%p|%8p", ptr, ptr, ptr, s21_NULL, s21_NULL);
  ck_assert_str_eq(dest1, dest2);
  ck_assert_int_eq(res1, res2);

  res1 = s21_sprintf(dest1, "%o|%#8o|%8o|%08o|%.6o|%#8.6o|%8.0o|%#8.0o|%#8.8o",
                     1234, 1234, 1234, 1234, 1234, 1234, 0, 0, 0);
  res2 = sprintf(dest2, "%o|%#8o|%8o|%08o|%.6o|%#8.6o|%8.0o|%#8.0o|%#8.8o",
                 1234, 1234, 1234, 1234, 1234, 1234, 0, 0, 0);
  ck_assert_str_eq(dest1, dest2);
  ck_assert_int_eq(res1, res2);

  res1 = s21_sprintf(dest1, "%x|%X|%8x|%08x|%.6x|%#8.6x|%8.0x|%#8.0x|%#8.8x",
                     1234, 1234, 1234, 1234, 1234, 1234, 0, 0, 0);
  res2 = sprintf(dest2, "%x|%X|%8x|%08x|%.6x|%#8.6x|%8.0x|%#8.0x|%#8.8x", 1234,
                 1234, 1234, 1234, 1234, 1234, 0, 0, 0);
  ck_assert_str_eq(dest1, dest2);
  ck_assert_int_eq(res1, res2);

  res1 = s21_sprintf(dest1, "%f|% .2f|%-10.3f|%+8.2f|%.0f|%.4f", 3.14159,
                     -2.71828, 123.456, 78.9, 5.0, 0.9999);
  res2 = sprintf(dest2, "%f|% .2f|%-10.3f|%+8.2f|%.0f|%.4f", 3.14159, -2.71828,
                 123.456, 78.9, 5.0, 0.9999);
  ck_assert_str_eq(dest1, dest2);
  ck_assert_int_eq(res1, res2);

  res1 = s21_sprintf(dest1, "%lf|%15.10lf|%- .5lf", lf_val, lf_val, lf_val);
  res2 = sprintf(dest2, "%lf|%15.10lf|%- .5lf", lf_val, lf_val, lf_val);
  ck_assert_str_eq(dest1, dest2);
  ck_assert_int_eq(res1, res2);

  res1 = s21_sprintf(dest1,
                     "%.0Lg|%.15LG|%.6g|%.6g|%.6G|%.6g|%.6G|%.6g|%.15Lg|%LG",
                     3.1415926535897932384626433832795029L, 0.00000123L,
                     987654321.0, -2.718281828, 0.0, 1.23e100, 9.9999999,
                     0.000123456789, 0.9999999L, 1.23e+99L);
  res2 = sprintf(dest2, "%.0Lg|%.15LG|%g|%.6g|%.6G|%.6g|%.6G|%.6g|%.15Lg|%LG",
                 3.1415926535897932384626433832795029L, 0.00000123L,
                 987654321.0, -2.718281828, 0.0, 1.23e100, 9.9999999,
                 0.000123456789, 0.9999999L, 1.23e+99L);
  ck_assert_str_eq(dest1, dest2);
  ck_assert_int_eq(res1, res2);

  res1 = s21_sprintf(
      dest1,
      "%.6g|%.4g|%.6g|%.2g|%.6g|%.2g|%.6g|%.2g|%.6g|%.1g|%#.6g|%#.2g|%.5g",
      123.456, 123.456, 1234567.0, 1234567.0, 0.0000123456, 0.0000123456,
      0.000123456, 0.000123456, 9.9999999, 0.099999999, 100.0, 100.0, 1.20003);
  res2 = sprintf(
      dest2,
      "%.6g|%.4g|%.6g|%.2g|%.6g|%.2g|%.6g|%.2g|%.6g|%.1g|%#.6g|%#.2g|%.5g",
      123.456, 123.456, 1234567.0, 1234567.0, 0.0000123456, 0.0000123456,
      0.000123456, 0.000123456, 9.9999999, 0.099999999, 100.0, 100.0, 1.20003);
  ck_assert_str_eq(dest1, dest2);
  ck_assert_int_eq(res1, res2);

  res1 =
      s21_sprintf(dest1, "%s|%-8s|%5.2s|%.3s", str, "world", "test", "abcdef");
  res2 = sprintf(dest2, "%s|%-8s|%5.2s|%.3s", str, "world", "test", "abcdef");
  ck_assert_int_eq(res1, res2);
  ck_assert_str_eq(dest1, dest2);

  res1 = s21_sprintf(dest1, "%ls|%-18ls|%5.4ls|%15.3ls|%.1ls|%.0ls", wstr, wstr,
                     wstr, wstr, wstr, wstr);
  res2 = sprintf(dest2, "%ls|%-18ls|%5.4ls|%15.3ls|%.1ls|%.0ls", wstr, wstr,
                 wstr, wstr, wstr, wstr);
  ck_assert_str_eq(dest1, dest2);
  ck_assert_int_eq(res1, res2);

  res1 = s21_sprintf(dest1, "%d %+5d %-6.2f %.3s %lc %%", -42, 73, 9.8765,
                     "comb", wc);
  res2 = sprintf(dest2, "%d %+5d %-6.2f %.3s %lc %%", -42, 73, 9.8765, "comb",
                 (wint_t)wc);
  ck_assert_str_eq(dest1, dest2);
  ck_assert_int_eq(res1, res2);

  res1 = s21_sprintf(dest1, "%s|%c", "", 0);
  res2 = sprintf(dest2, "%s|%c", "", 0);
  ck_assert_str_eq(dest1, dest2);
  ck_assert_int_eq(res1, res2);

  res1 = s21_sprintf(dest1, "%10d|%15.8f|%-20s", 123, pi, "width_test");
  res2 = sprintf(dest2, "%10d|%15.8f|%-20s", 123, pi, "width_test");
  ck_assert_str_eq(dest1, dest2);
  ck_assert_int_eq(res1, res2);

  char buffer[100] = {0};
  char buffer2[100] = {0};
  int n1, n2, p1 = 0, p2 = 0;
  long n3, p3 = 0;
  short n5, p5 = 0;
  int r1 = s21_sprintf(buffer, "Test%n %d%n", &n1, 123, &n2);
  int r2 = sprintf(buffer2, "Test%n %d%n", &p1, 123, &p2);
  ck_assert_str_eq(buffer, buffer2);
  ck_assert_int_eq(n1, p1);
  ck_assert_int_eq(n2, p2);
  ck_assert_int_eq(r1, r2);

  r1 = s21_sprintf(buffer, "Long%ln string", &n3);
  r2 = sprintf(buffer2, "Long%ln string", &p3);
  ck_assert_str_eq(buffer, buffer2);
  ck_assert_int_eq(n3, p3);
  ck_assert_int_eq(r1, r2);

  r1 = s21_sprintf(buffer, "Short%hn value", &n5);
  r2 = sprintf(buffer2, "Short%hn value", &p5);
  ck_assert_str_eq(buffer, buffer2);
  ck_assert_int_eq(n5, p5);
  ck_assert_int_eq(r1, r2);
}
END_TEST

START_TEST(test_sprintf_nan_inf) {
  char s21_buffer[500];
  char std_buffer[500];
  int s21_len, std_len;
  double nan_val = NAN;
  double inf_val = INFINITY;

  s21_len = s21_sprintf(
      s21_buffer, "NaN: %f, -NaN: %f, Inf: %f, -Inf: %f|%*d| |%.*f| |%*.*s|\n",
      nan_val, -nan_val, inf_val, -inf_val, 0, 123, 3, 3.14159, 10, 5,
      "HelloWorld");
  std_len = sprintf(
      std_buffer, "NaN: %f, -NaN: %f, Inf: %f, -Inf: %f|%*d| |%.*f| |%*.*s|\n",
      nan_val, -nan_val, inf_val, -inf_val, 0, 123, 3, 3.14159, 10, 5,
      "HelloWorld");
  ck_assert_str_eq(s21_buffer, std_buffer);
  ck_assert_int_eq(s21_len, std_len);

  s21_len = s21_sprintf(s21_buffer, "0.0: %f, +0.0: %+f, -0.0: %f|% f|%f\n",
                        0.0, 0.0, -0.0, 0.0, +0.0);
  std_len = sprintf(std_buffer, "0.0: %f, +0.0: %+f, -0.0: %f|% f|%f\n", 0.0,
                    0.0, -0.0, 0.0, +0.0);
  ck_assert_str_eq(s21_buffer, std_buffer);
  ck_assert_int_eq(s21_len, std_len);
}
END_TEST

TCase *tcase_str(void) {
  TCase *tc = tcase_create("String Functions");
  tcase_add_test(tc, test_s21_memchr);
  tcase_add_test(tc, test_s21_memcmp);
  tcase_add_test(tc, test_s21_memcpy);
  tcase_add_test(tc, test_s21_memset);
  tcase_add_test(tc, test_s21_strncat);
  tcase_add_test(tc, test_s21_strchr);
  tcase_add_test(tc, test_s21_strncmp);
  tcase_add_test(tc, test_s21_strncpy);
  tcase_add_test(tc, test_s21_strcspn);
  tcase_add_test(tc, test_s21_strerror);
  tcase_add_test(tc, test_s21_strlen);
  tcase_add_test(tc, test_s21_strpbrk);
  tcase_add_test(tc, test_s21_strrchr);
  tcase_add_test(tc, test_s21_strstr);
  tcase_add_test(tc, test_s21_strtok);
  return tc;
}

TCase *tcase_str_3(void) {
  TCase *tc3 = tcase_create("s21_sscanf");
  tcase_add_test(tc3, test_sscanf_min_max_values);
  tcase_add_test(tc3, test_sscanf_d_i);
  tcase_add_test(tc3, test_sscanf_length_modifiers);
  tcase_add_test(tc3, test_sscanf_uoxX);
  tcase_add_test(tc3, test_sscanf_fegG);
  tcase_add_test(tc3, test_sscanf_scientific_notation);
  tcase_add_test(tc3, test_sscanf_long_double);
  tcase_add_test(tc3, test_sscanf_cs);
  tcase_add_test(tc3, test_sscanf_lc_ls);
  tcase_add_test(tc3, test_sscanf_p);
  tcase_add_test(tc3, test_sscanf_n);
  tcase_add_test(tc3, test_sscanf_percent);
  tcase_add_test(tc3, test_sscanf_assignment_suppression);
  tcase_add_test(tc3, test_sscanf_empty_input);
  tcase_add_test(tc3, test_sscanf_overflow);
  tcase_add_test(tc3, test_sscanf_complex_format);
  tcase_add_test(tc3, test_sscanf_malformed_format);
  return tc3;
}

TCase *tcase_str_4(void) {
  TCase *tc4 = tcase_create("sharp");
  tcase_add_test(tc4, test_to_upper_1);
  tcase_add_test(tc4, test_to_upper_2);
  tcase_add_test(tc4, test_to_upper_3);
  tcase_add_test(tc4, test_to_upper_4);
  tcase_add_test(tc4, test_to_upper_5);
  tcase_add_test(tc4, test_to_upper_6);
  tcase_add_test(tc4, test_to_upper_7);
  tcase_add_test(tc4, test_to_lower_1);
  tcase_add_test(tc4, test_to_lower_2);
  tcase_add_test(tc4, test_to_lower_3);
  tcase_add_test(tc4, test_to_lower_4);
  tcase_add_test(tc4, test_to_lower_5);
  tcase_add_test(tc4, test_to_lower_6);
  tcase_add_test(tc4, test_to_lower_7);
  tcase_add_test(tc4, test_insert_1);
  tcase_add_test(tc4, test_insert_2);
  tcase_add_test(tc4, test_insert_3);
  tcase_add_test(tc4, test_insert_4);
  tcase_add_test(tc4, test_insert_5);
  tcase_add_test(tc4, test_insert_6);
  tcase_add_test(tc4, test_insert_7);
  tcase_add_test(tc4, test_insert_8);
  tcase_add_test(tc4, test_insert_9);
  tcase_add_test(tc4, test_insert_10);
  tcase_add_test(tc4, test_trim_1);
  tcase_add_test(tc4, test_trim_2);
  tcase_add_test(tc4, test_trim_3);
  tcase_add_test(tc4, test_trim_4);
  tcase_add_test(tc4, test_trim_5);
  tcase_add_test(tc4, test_trim_6);
  tcase_add_test(tc4, test_trim_7);
  tcase_add_test(tc4, test_trim_8);
  tcase_add_test(tc4, test_trim_9);
  tcase_add_test(tc4, test_trim_10);
  return tc4;
}

TCase *tcase_s21_sprintf(void) {
  TCase *tc = tcase_create("s21_sprintf");
  tcase_add_test(tc, test_s21_sprintf_all_unique);
  tcase_add_test(tc, test_sprintf_nan_inf);
  return tc;
}

Suite *string_suite(void) {
  Suite *s = suite_create("String Library Tests");
  suite_add_tcase(s, tcase_str());
  suite_add_tcase(s, tcase_str_3());
  suite_add_tcase(s, tcase_str_4());
  suite_add_tcase(s, tcase_s21_sprintf());
  return s;
}

int main() {
  SRunner *sr = srunner_create(string_suite());
  srunner_set_fork_status(sr, CK_NOFORK);
  srunner_run_all(sr, CK_VERBOSE);

  int failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (failed == 0) ? 0 : 1;
}