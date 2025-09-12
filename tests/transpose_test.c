#include <check.h>

#include "../src/matrix.h"

START_TEST(test_A_is_null) {
  matrix_t mat;

  int result = transpose(NULL, &mat);

  ck_assert_int_eq(INVALID_MATRIX, result);
}
END_TEST

START_TEST(test_result_is_null) {
  matrix_t A;

  int result = transpose(&A, NULL);

  ck_assert_int_eq(INVALID_MATRIX, result);
}
END_TEST

START_TEST(test_invalid_rows) {
  matrix_t A = {.rows = -1, .columns = 1};
  matrix_t mat;

  int result = transpose(&A, &mat);

  ck_assert_int_eq(INVALID_MATRIX, result);
}
END_TEST

START_TEST(test_invalid_columns) {
  matrix_t A = {.rows = 1, .columns = -1};
  matrix_t mat;

  int result = transpose(&A, &mat);

  ck_assert_int_eq(INVALID_MATRIX, result);
}
END_TEST

START_TEST(test_A_matrix_null) {
  matrix_t A = {.rows = 1, .columns = 1, .matrix = NULL};
  matrix_t mat;

  int result = transpose(&A, &mat);

  ck_assert_int_eq(INVALID_MATRIX, result);
}
END_TEST

START_TEST(test_transpose) {
  matrix_t A;
  create_matrix(1, 3, &A);
  A.matrix[0][0] = 123.456;
  A.matrix[0][1] = 789.012;
  A.matrix[0][2] = 456.34342;
  matrix_t mat;
  double expected_transpose[] = {A.matrix[0][0], A.matrix[0][1],
                                 A.matrix[0][2]};

  int result = transpose(&A, &mat);

  remove_matrix(&A);

  ck_assert_int_eq(OK, result);
  ck_assert_int_eq(3, mat.rows);
  ck_assert_int_eq(1, mat.columns);
  for (int i = 0; i < 3; i++)
    ck_assert_double_eq_tol(expected_transpose[i], mat.matrix[i][0], 1e-6);
  remove_matrix(&mat);
}

Suite *transpose_suite(void) {
  Suite *suite = suite_create("transpose");
  TCase *tc = tcase_create("Core");

  tcase_add_test(tc, test_A_is_null);
  tcase_add_test(tc, test_result_is_null);
  tcase_add_test(tc, test_invalid_rows);
  tcase_add_test(tc, test_invalid_columns);
  tcase_add_test(tc, test_A_matrix_null);
  tcase_add_test(tc, test_transpose);

  suite_add_tcase(suite, tc);
  return suite;
}