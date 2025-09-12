#include <check.h>
#include <stdio.h>

#include "../src/matrix.h"

START_TEST(test_A_is_null) {
  matrix_t mat;

  int result = inverse_matrix(NULL, &mat);

  ck_assert_int_eq(INVALID_MATRIX, result);
}
END_TEST

START_TEST(test_result_is_null) {
  matrix_t A;

  int result = inverse_matrix(&A, NULL);

  ck_assert_int_eq(INVALID_MATRIX, result);
}
END_TEST

START_TEST(test_invalid_rows) {
  matrix_t A = {.rows = -1, .columns = 1};
  matrix_t mat;

  int result = inverse_matrix(&A, &mat);

  ck_assert_int_eq(INVALID_MATRIX, result);
}
END_TEST

START_TEST(test_invalid_columns) {
  matrix_t A = {.rows = 1, .columns = -1};
  matrix_t mat;

  int result = inverse_matrix(&A, &mat);

  ck_assert_int_eq(INVALID_MATRIX, result);
}
END_TEST

START_TEST(test_A_matrix_null) {
  matrix_t A = {.rows = 1, .columns = 1, .matrix = NULL};
  matrix_t mat;

  int result = inverse_matrix(&A, &mat);

  ck_assert_int_eq(INVALID_MATRIX, result);
}
END_TEST

START_TEST(test_inverse_matrix) {
  matrix_t A;
  create_matrix(3, 3, &A);
  A.matrix[0][0] = 123.456;
  A.matrix[0][1] = 2006.206;
  A.matrix[0][2] = 15.49;
  A.matrix[1][0] = 456.123;
  A.matrix[1][1] = 2007.207;
  A.matrix[1][2] = 14.50;
  A.matrix[2][0] = 412.563;
  A.matrix[2][1] = 2008.208;
  A.matrix[2][2] = 13.51;
  matrix_t mat;
  matrix_t res;
  create_matrix(3, 3, &res);
  res.matrix[0][0] = -0.00265797;
  res.matrix[0][1] = 0.00531594;
  res.matrix[0][2] = -0.00265797;
  res.matrix[1][0] = -0.000239;
  res.matrix[1][1] = -0.00627124;
  res.matrix[1][2] = 0.00700493;
  res.matrix[2][0] = 0.116709;
  res.matrix[2][1] = 0.769859;
  res.matrix[2][2] = -0.886068;

  int result = inverse_matrix(&A, &mat);

  remove_matrix(&A);

  ck_assert_int_eq(OK, result);
  ck_assert_int_eq(3, mat.rows);
  ck_assert_int_eq(3, mat.columns);
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++)
      ck_assert_double_eq_tol(res.matrix[i][j], mat.matrix[i][j], 1e-6);
  remove_matrix(&mat);
  remove_matrix(&res);
}

START_TEST(test_inverse_det_zero) {
  matrix_t A;
  create_matrix(3, 3, &A);
  A.matrix[0][0] = 123.456;
  A.matrix[0][1] = 0;
  A.matrix[0][2] = 15.49;
  A.matrix[1][0] = 456.123;
  A.matrix[1][1] = 0;
  A.matrix[1][2] = 14.50;
  A.matrix[2][0] = 412.563;
  A.matrix[2][1] = 0;
  A.matrix[2][2] = 13.51;
  matrix_t mat;

  int result = inverse_matrix(&A, &mat);

  remove_matrix(&A);

  ck_assert_int_eq(CALCULATION_ERROR, result);
}

Suite *inverse_matrix_suite(void) {
  Suite *suite = suite_create("inverse_matrix");
  TCase *tc = tcase_create("Core");

  tcase_add_test(tc, test_A_is_null);
  tcase_add_test(tc, test_result_is_null);
  tcase_add_test(tc, test_invalid_rows);
  tcase_add_test(tc, test_invalid_columns);
  tcase_add_test(tc, test_A_matrix_null);
  tcase_add_test(tc, test_inverse_matrix);
  tcase_add_test(tc, test_inverse_det_zero);

  suite_add_tcase(suite, tc);
  return suite;
}