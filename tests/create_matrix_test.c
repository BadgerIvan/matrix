#include <check.h>
#include <stdlib.h>

#include "../src/matrix.h"

START_TEST(test_simple) {
  matrix_t matrix;

  int result = create_matrix(3, 3, &matrix);

  ck_assert_int_eq(OK, result);
  ck_assert_int_eq(3, matrix.rows);
  ck_assert_int_eq(3, matrix.columns);
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++) ck_assert_int_eq(0, matrix.matrix[i][j]);

  free(matrix.matrix[0]);
  free(matrix.matrix);
}
END_TEST

START_TEST(test_with_numbers) {
  matrix_t matrix;

  int result = create_matrix(3, 4, &matrix);

  ck_assert_int_eq(OK, result);
  ck_assert_int_eq(3, matrix.rows);
  ck_assert_int_eq(4, matrix.columns);
  ck_assert_ptr_nonnull(matrix.matrix);

  int k = 0;
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 4; j++) matrix.matrix[i][j] = ++k;

  k = 0;
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 4; j++) ck_assert_int_eq(++k, matrix.matrix[i][j]);

  free(matrix.matrix[0]);
  free(matrix.matrix);
}
END_TEST

START_TEST(test_with_invalid_rows) {
  matrix_t matrix;

  int result = create_matrix(-1, 3, &matrix);

  ck_assert_int_eq(INVALID_MATRIX, result);
}
END_TEST

START_TEST(test_with_invalid_columns) {
  matrix_t matrix;

  int result = create_matrix(3, -1, &matrix);

  ck_assert_int_eq(INVALID_MATRIX, result);
}
END_TEST

START_TEST(test_with_matrix_null) {
  int result = create_matrix(3, 3, NULL);

  ck_assert_int_eq(INVALID_MATRIX, result);
}
END_TEST

Suite *create_matrix_suite(void) {
  Suite *suite = suite_create("create_matrix");
  TCase *tc = tcase_create("Core");

  tcase_add_test(tc, test_simple);
  tcase_add_test(tc, test_with_numbers);
  tcase_add_test(tc, test_with_invalid_rows);
  tcase_add_test(tc, test_with_invalid_columns);
  tcase_add_test(tc, test_with_matrix_null);

  suite_add_tcase(suite, tc);

  return suite;
}