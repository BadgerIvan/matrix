#include <check.h>
#include <stdio.h>

#include "../src/matrix.h"

START_TEST(test_A_is_null) {
  double res;

  int result = determinant(NULL, &res);

  ck_assert_int_eq(INVALID_MATRIX, result);
}
END_TEST

START_TEST(test_res_is_null) {
  matrix_t A = {.columns = 1, .rows = 1, .matrix = (double **)0xDEAD};

  int result = determinant(&A, NULL);

  ck_assert_int_eq(CALCULATION_ERROR, result);
}
END_TEST

START_TEST(test_invalid_rows) {
  matrix_t A = {.rows = -1, .columns = 1};
  double res;

  int result = determinant(&A, &res);

  ck_assert_int_eq(INVALID_MATRIX, result);
}
END_TEST

START_TEST(test_invalid_columns) {
  matrix_t A = {.rows = 1, .columns = -1};
  double res;

  int result = determinant(&A, &res);

  ck_assert_int_eq(INVALID_MATRIX, result);
}
END_TEST

START_TEST(test_A_matrix_null) {
  matrix_t A = {.rows = 1, .columns = 1, .matrix = NULL};
  double res;

  int result = determinant(&A, &res);

  ck_assert_int_eq(INVALID_MATRIX, result);
}
END_TEST

START_TEST(test_determinant_1x1) {
  matrix_t A;
  create_matrix(1, 1, &A);
  A.matrix[0][0] = 123.456;
  double res;
  double expected_determinant = 123.456;

  int result = determinant(&A, &res);

  remove_matrix(&A);

  ck_assert_int_eq(OK, result);
  ck_assert_double_eq_tol(expected_determinant, res, 1e-6);
}

START_TEST(test_determinant_2x2) {
  matrix_t A;
  create_matrix(2, 2, &A);
  A.matrix[0][0] = 123.456;
  A.matrix[0][1] = 2006.206;
  A.matrix[1][0] = 456.123;
  A.matrix[1][1] = 2007.207;
  double res;
  double expected_determinant = -667274.951946;

  int result = determinant(&A, &res);

  remove_matrix(&A);

  ck_assert_int_eq(OK, result);
  ck_assert_double_eq_tol(expected_determinant, res, 1e-6);
}

START_TEST(test_determinant_3x3) {
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
  double res;
  double expected_determinant = 753074.56010061;

  int result = determinant(&A, &res);

  remove_matrix(&A);

  ck_assert_int_eq(OK, result);
  ck_assert_double_eq_tol(expected_determinant, res, 1e-6);
}

START_TEST(test_determinant_4x4) {
  matrix_t A;
  create_matrix(4, 4, &A);
  A.matrix[0][0] = 3;
  A.matrix[0][1] = 2006.206;
  A.matrix[0][2] = 15.49;
  A.matrix[0][3] = 534.43;
  A.matrix[1][0] = 3;
  A.matrix[1][1] = 2007.207;
  A.matrix[1][2] = 14.50;
  A.matrix[1][3] = 56.43;
  A.matrix[2][0] = 3;
  A.matrix[2][1] = 2008.208;
  A.matrix[2][2] = 13.51;
  A.matrix[2][3] = 123.3;
  A.matrix[3][0] = 3;
  A.matrix[3][1] = 8484.84;
  A.matrix[3][2] = 6565.65;
  A.matrix[3][3] = 1212.1212;
  double res;
  double expected_determinant = -21201803.518150;

  int result = determinant(&A, &res);

  remove_matrix(&A);

  ck_assert_int_eq(OK, result);
  ck_assert_double_eq_tol(expected_determinant, res, 1e-6);
}

Suite *determinant_suite(void) {
  Suite *suite = suite_create("determinant");
  TCase *tc = tcase_create("Core");

  tcase_add_test(tc, test_A_is_null);
  tcase_add_test(tc, test_res_is_null);
  tcase_add_test(tc, test_invalid_rows);
  tcase_add_test(tc, test_invalid_columns);
  tcase_add_test(tc, test_A_matrix_null);
  tcase_add_test(tc, test_determinant_1x1);
  tcase_add_test(tc, test_determinant_2x2);
  tcase_add_test(tc, test_determinant_3x3);
  tcase_add_test(tc, test_determinant_4x4);

  suite_add_tcase(suite, tc);
  return suite;
}