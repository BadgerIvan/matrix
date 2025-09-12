#include <check.h>
#include <stdio.h>
#include <stdlib.h>

#include "../src/matrix.h"

START_TEST(test_simple) {
  matrix_t matrix;
  create_matrix(3, 3, &matrix);

  remove_matrix(&matrix);

  ck_assert_int_eq(0, matrix.rows);
  ck_assert_int_eq(0, matrix.columns);
  ck_assert(matrix.matrix == NULL);
}
END_TEST

START_TEST(test_with_null) { remove_matrix(NULL); }
END_TEST

Suite *remove_matrix_suite(void) {
  Suite *suite = suite_create("remove_matrix");
  TCase *tc = tcase_create("Core");

  tcase_add_test(tc, test_simple);
  tcase_add_test(tc, test_with_null);

  suite_add_tcase(suite, tc);

  return suite;
}