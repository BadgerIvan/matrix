#include <check.h>

#include "../src/matrix.h"

START_TEST(test_A_is_null) {
  matrix_t mat;

  int result = mult_number(NULL, 1.0, &mat);

  ck_assert_int_eq(INVALID_MATRIX, result);
}
END_TEST

START_TEST(test_result_is_null) {
  matrix_t A;

  int result = mult_number(&A, 1.0, NULL);

  ck_assert_int_eq(INVALID_MATRIX, result);
}
END_TEST

START_TEST(test_invalid_rows) {
  matrix_t A = {.rows = -1, .columns = 1};
  matrix_t mat;

  int result = mult_number(&A, 1.0, &mat);

  ck_assert_int_eq(INVALID_MATRIX, result);
}
END_TEST

START_TEST(test_invalid_columns) {
  matrix_t A = {.rows = 1, .columns = -1};
  matrix_t mat;

  int result = mult_number(&A, 1.0, &mat);

  ck_assert_int_eq(INVALID_MATRIX, result);
}
END_TEST

START_TEST(test_A_matrix_null) {
  matrix_t A = {.rows = 1, .columns = 1, .matrix = NULL};
  matrix_t mat;

  int result = mult_number(&A, 1.0, &mat);

  ck_assert_int_eq(INVALID_MATRIX, result);
}
END_TEST

START_TEST(test_mult_number) {
  matrix_t A;
  create_matrix(1, 3, &A);
  A.matrix[0][0] = 123.456;
  A.matrix[0][1] = 789.012;
  A.matrix[0][2] = 456.34342;
  matrix_t mat;
  double expected_mult_number[] = {A.matrix[0][0] * 2, A.matrix[0][1] * 2,
                                   A.matrix[0][2] * 2};

  int result = mult_number(&A, 2.0, &mat);

  remove_matrix(&A);

  ck_assert_int_eq(OK, result);
  ck_assert_int_eq(1, mat.rows);
  ck_assert_int_eq(3, mat.columns);
  for (int i = 0; i < 3; i++)
    ck_assert_double_eq_tol(expected_mult_number[i], mat.matrix[0][i], 1e-6);
  remove_matrix(&mat);
}

START_TEST(test_mult_number_4x4) {
  matrix_t A;
  create_matrix(4, 4, &A);

  double values_4x4[4][4] = {{1.0, 2.0, 3.0, 4.0},
                             {5.0, 6.0, 7.0, 8.0},
                             {9.0, 10.0, 11.0, 12.0},
                             {13.0, 14.0, 15.0, 16.0}};
  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++) A.matrix[i][j] = values_4x4[i][j];

  matrix_t mat;
  double expected_mult_number[4][4];
  double multiplier = 2.5;

  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++)
      expected_mult_number[i][j] = A.matrix[i][j] * multiplier;

  int result = mult_number(&A, multiplier, &mat);

  remove_matrix(&A);

  ck_assert_int_eq(OK, result);
  ck_assert_int_eq(4, mat.rows);
  ck_assert_int_eq(4, mat.columns);

  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++)
      ck_assert_double_eq_tol(expected_mult_number[i][j], mat.matrix[i][j],
                              1e-6);

  remove_matrix(&mat);
}
END_TEST

START_TEST(test_mult_number_5x5) {
  matrix_t A;
  create_matrix(5, 5, &A);

  double values_5x5[5][5] = {{1.1, 2.2, 3.3, 4.4, 5.5},
                             {6.6, 7.7, 8.8, 9.9, 10.1},
                             {11.11, 12.12, 13.13, 14.14, 15.15},
                             {16.16, 17.17, 18.18, 19.19, 20.20},
                             {21.21, 22.22, 23.23, 24.24, 25.25}};
  for (int i = 0; i < 5; i++)
    for (int j = 0; j < 5; j++) A.matrix[i][j] = values_5x5[i][j];

  matrix_t mat;
  double expected_mult_number[5][5];
  double multiplier = 3.0;

  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
      expected_mult_number[i][j] = A.matrix[i][j] * multiplier;
    }
  }

  int result = mult_number(&A, multiplier, &mat);

  remove_matrix(&A);

  ck_assert_int_eq(OK, result);
  ck_assert_int_eq(5, mat.rows);
  ck_assert_int_eq(5, mat.columns);
  for (int i = 0; i < 5; i++)
    for (int j = 0; j < 5; j++)
      ck_assert_double_eq_tol(expected_mult_number[i][j], mat.matrix[i][j],
                              1e-6);

  remove_matrix(&mat);
}
END_TEST

Suite *mult_number_suite(void) {
  Suite *suite = suite_create("mult_number");
  TCase *tc = tcase_create("Core");

  tcase_add_test(tc, test_A_is_null);
  tcase_add_test(tc, test_result_is_null);
  tcase_add_test(tc, test_invalid_rows);
  tcase_add_test(tc, test_invalid_columns);
  tcase_add_test(tc, test_A_matrix_null);
  tcase_add_test(tc, test_mult_number);
  tcase_add_test(tc, test_mult_number_4x4);
  tcase_add_test(tc, test_mult_number_5x5);

  suite_add_tcase(suite, tc);
  return suite;
}