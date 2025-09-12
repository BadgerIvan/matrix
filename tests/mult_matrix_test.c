#include <check.h>

#include "../src/matrix.h"

START_TEST(test_A_is_null) {
  matrix_t B;
  matrix_t mat;

  int result = mult_matrix(NULL, &B, &mat);

  ck_assert_int_eq(INVALID_MATRIX, result);
}
END_TEST

START_TEST(test_B_is_null) {
  matrix_t A;
  matrix_t mat;

  int result = mult_matrix(&A, NULL, &mat);

  ck_assert_int_eq(INVALID_MATRIX, result);
}
END_TEST

START_TEST(test_result_is_null) {
  matrix_t A;
  matrix_t B;

  int result = mult_matrix(&A, &B, NULL);

  ck_assert_int_eq(INVALID_MATRIX, result);
}
END_TEST

START_TEST(test_invalid_mult) {
  matrix_t A;
  create_matrix(4, 1, &A);
  matrix_t B;
  create_matrix(2, 4, &B);
  matrix_t mat;

  int result = mult_matrix(&A, &B, &mat);

  remove_matrix(&A);
  remove_matrix(&B);

  ck_assert_int_eq(CALCULATION_ERROR, result);
}
END_TEST

START_TEST(test_invalid_rows_in_A) {
  matrix_t A = {.rows = -1, .columns = 2};
  matrix_t B = {.rows = 2, .columns = 4};
  matrix_t mat;

  int result = mult_matrix(&A, &B, &mat);

  ck_assert_int_eq(INVALID_MATRIX, result);
}
END_TEST

START_TEST(test_invalid_rows_in_B_and_columns_in_A) {
  matrix_t A = {.rows = 1, .columns = -1};
  matrix_t B = {.rows = -1, .columns = 4};
  matrix_t mat;

  int result = mult_matrix(&A, &B, &mat);

  ck_assert_int_eq(INVALID_MATRIX, result);
}
END_TEST

START_TEST(test_invalid_columns_in_B) {
  matrix_t A = {.rows = 1, .columns = 2};
  matrix_t B = {.rows = 2, .columns = -1};
  matrix_t mat;

  int result = mult_matrix(&A, &B, &mat);

  ck_assert_int_eq(INVALID_MATRIX, result);
}
END_TEST

START_TEST(test_A_matrix_null) {
  matrix_t A = {.rows = 1, .columns = 1, .matrix = NULL};
  matrix_t B;
  create_matrix(1, 1, &B);
  matrix_t mat;

  int result = mult_matrix(&A, &B, &mat);

  remove_matrix(&B);

  ck_assert_int_eq(INVALID_MATRIX, result);
}
END_TEST

START_TEST(test_B_matrix_null) {
  matrix_t A;
  create_matrix(1, 1, &A);
  matrix_t B = {.rows = 1, .columns = 1, .matrix = NULL};
  matrix_t mat;

  int result = mult_matrix(&A, &B, &mat);

  remove_matrix(&A);

  ck_assert_int_eq(INVALID_MATRIX, result);
}
END_TEST

START_TEST(test_A_and_B_matrix_null) {
  matrix_t A = {.rows = 1, .columns = 1, .matrix = NULL};
  matrix_t B = {.rows = 1, .columns = 1, .matrix = NULL};
  matrix_t mat;

  int result = mult_matrix(&A, &B, &mat);

  ck_assert_int_eq(INVALID_MATRIX, result);
}
END_TEST

START_TEST(test_mult) {
  matrix_t A;
  create_matrix(1, 3, &A);
  A.matrix[0][0] = 123.456;
  A.matrix[0][1] = 789.012;
  A.matrix[0][2] = 456.3434;
  matrix_t B;
  create_matrix(3, 1, &B);
  B.matrix[0][0] = 342006.1;
  B.matrix[1][0] = 5252.525;
  B.matrix[2][0] = 666.555;
  matrix_t mat;
  double expected_mult = 46671188.311887;

  int result = mult_matrix(&A, &B, &mat);

  remove_matrix(&A);
  remove_matrix(&B);

  ck_assert_int_eq(OK, result);
  ck_assert_int_eq(1, mat.rows);
  ck_assert_int_eq(1, mat.columns);
  ck_assert_double_eq_tol(expected_mult, mat.matrix[0][0], 1e-6);
  remove_matrix(&mat);
}

Suite *mult_matrix_suite(void) {
  Suite *suite = suite_create("mult_matrix");
  TCase *tc = tcase_create("Core");

  tcase_add_test(tc, test_A_is_null);
  tcase_add_test(tc, test_B_is_null);
  tcase_add_test(tc, test_result_is_null);
  tcase_add_test(tc, test_invalid_mult);
  tcase_add_test(tc, test_invalid_rows_in_A);
  tcase_add_test(tc, test_invalid_rows_in_B_and_columns_in_A);
  tcase_add_test(tc, test_invalid_columns_in_B);
  tcase_add_test(tc, test_A_matrix_null);
  tcase_add_test(tc, test_B_matrix_null);
  tcase_add_test(tc, test_A_and_B_matrix_null);
  tcase_add_test(tc, test_mult);

  suite_add_tcase(suite, tc);
  return suite;
}