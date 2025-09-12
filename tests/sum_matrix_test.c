#include <check.h>

#include "../src/matrix.h"

START_TEST(test_A_is_null) {
  matrix_t B;
  matrix_t mat;

  int result = sum_matrix(NULL, &B, &mat);

  ck_assert_int_eq(INVALID_MATRIX, result);
}
END_TEST

START_TEST(test_B_is_null) {
  matrix_t A;
  matrix_t mat;

  int result = sum_matrix(&A, NULL, &mat);

  ck_assert_int_eq(INVALID_MATRIX, result);
}
END_TEST

START_TEST(test_result_is_null) {
  matrix_t A;
  matrix_t B;

  int result = sum_matrix(&A, &B, NULL);

  ck_assert_int_eq(INVALID_MATRIX, result);
}
END_TEST

START_TEST(test_not_eq_rows) {
  matrix_t A;
  create_matrix(2, 1, &A);
  matrix_t B;
  create_matrix(1, 1, &B);
  matrix_t mat;

  int result = sum_matrix(&A, &B, &mat);

  remove_matrix(&A);
  remove_matrix(&B);

  ck_assert_int_eq(CALCULATION_ERROR, result);
}
END_TEST

START_TEST(test_not_eq_columns) {
  matrix_t A;
  create_matrix(1, 2, &A);
  matrix_t B;
  create_matrix(1, 1, &B);
  matrix_t mat;

  int result = sum_matrix(&A, &B, &mat);

  remove_matrix(&A);
  remove_matrix(&B);

  ck_assert_int_eq(CALCULATION_ERROR, result);
}
END_TEST

START_TEST(test_invalid_rows) {
  matrix_t A = {.rows = -1, .columns = 1};
  matrix_t B = {.rows = -1, .columns = 1};
  matrix_t mat;

  int result = sum_matrix(&A, &B, &mat);

  ck_assert_int_eq(INVALID_MATRIX, result);
}
END_TEST

START_TEST(test_invalid_columns) {
  matrix_t A = {.rows = 1, .columns = -1};
  matrix_t B = {.rows = 1, .columns = -1};
  matrix_t mat;

  int result = sum_matrix(&A, &B, &mat);

  ck_assert_int_eq(INVALID_MATRIX, result);
}
END_TEST

START_TEST(test_A_matrix_null) {
  matrix_t A = {.rows = 1, .columns = 1, .matrix = NULL};
  matrix_t B;
  create_matrix(1, 1, &B);
  matrix_t mat;

  int result = sum_matrix(&A, &B, &mat);

  remove_matrix(&B);

  ck_assert_int_eq(INVALID_MATRIX, result);
}
END_TEST

START_TEST(test_B_matrix_null) {
  matrix_t A;
  create_matrix(1, 1, &A);
  matrix_t B = {.rows = 1, .columns = 1, .matrix = NULL};
  matrix_t mat;

  int result = sum_matrix(&A, &B, &mat);

  remove_matrix(&A);

  ck_assert_int_eq(INVALID_MATRIX, result);
}
END_TEST

START_TEST(test_A_and_B_matrix_null) {
  matrix_t A = {.rows = 1, .columns = 1, .matrix = NULL};
  matrix_t B = {.rows = 1, .columns = 1, .matrix = NULL};
  matrix_t mat;

  int result = sum_matrix(&A, &B, &mat);

  ck_assert_int_eq(INVALID_MATRIX, result);
}
END_TEST

START_TEST(test_sum) {
  matrix_t A;
  create_matrix(1, 3, &A);
  A.matrix[0][0] = 123.456;
  A.matrix[0][1] = 789.012;
  A.matrix[0][2] = 456.34342;
  matrix_t B;
  create_matrix(1, 3, &B);
  B.matrix[0][0] = 342006.1024;
  B.matrix[0][1] = 5252.5252;
  B.matrix[0][2] = 666.555;
  matrix_t mat;
  double expected_sum[] = {342129.5584, 6041.5372, 1122.89842};

  int result = sum_matrix(&A, &B, &mat);

  remove_matrix(&A);
  remove_matrix(&B);

  ck_assert_int_eq(OK, result);
  ck_assert_int_eq(1, mat.rows);
  ck_assert_int_eq(3, mat.columns);
  for (int i = 0; i < 3; i++)
    ck_assert_double_eq_tol(expected_sum[i], mat.matrix[0][i], 1e-6);
  remove_matrix(&mat);
}

START_TEST(test_sum_4x4) {
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
  matrix_t B;
  create_matrix(4, 4, &B);
  B.matrix[0][0] = 3;
  B.matrix[0][1] = 2006.206;
  B.matrix[0][2] = 15.49;
  B.matrix[0][3] = 534.43;
  B.matrix[1][0] = 3;
  B.matrix[1][1] = 2007.207;
  B.matrix[1][2] = 14.50;
  B.matrix[1][3] = 56.43;
  B.matrix[2][0] = 3;
  B.matrix[2][1] = 2008.208;
  B.matrix[2][2] = 13.51;
  B.matrix[2][3] = 123.3;
  B.matrix[3][0] = 3;
  B.matrix[3][1] = 8484.84;
  B.matrix[3][2] = 6565.65;
  B.matrix[3][3] = 1212.1212;
  matrix_t mat;
  double expected_sum[4][4] = {{6.0, 4012.412, 30.98, 1068.86},
                               {6.0, 4014.414, 29.0, 112.86},
                               {6.0, 4016.416, 27.02, 246.6},
                               {6.0, 16969.68, 13131.3, 2424.2424}};

  int result = sum_matrix(&A, &B, &mat);

  remove_matrix(&A);
  remove_matrix(&B);

  ck_assert_int_eq(OK, result);
  ck_assert_int_eq(4, mat.rows);
  ck_assert_int_eq(4, mat.columns);
  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++)
      ck_assert_double_eq_tol(expected_sum[i][j], mat.matrix[i][j], 1e-6);
  remove_matrix(&mat);
}

START_TEST(test_sum_5x5) {
  matrix_t A;
  create_matrix(5, 5, &A);
  A.matrix[0][0] = 1.5;
  A.matrix[0][1] = 2.7;
  A.matrix[0][2] = 3.9;
  A.matrix[0][3] = 4.2;
  A.matrix[0][4] = 5.8;

  A.matrix[1][0] = 6.1;
  A.matrix[1][1] = 7.3;
  A.matrix[1][2] = 8.5;
  A.matrix[1][3] = 9.7;
  A.matrix[1][4] = 10.9;

  A.matrix[2][0] = 11.2;
  A.matrix[2][1] = 12.4;
  A.matrix[2][2] = 13.6;
  A.matrix[2][3] = 14.8;
  A.matrix[2][4] = 15.0;

  A.matrix[3][0] = 16.1;
  A.matrix[3][1] = 17.3;
  A.matrix[3][2] = 18.5;
  A.matrix[3][3] = 19.7;
  A.matrix[3][4] = 20.9;

  A.matrix[4][0] = 21.2;
  A.matrix[4][1] = 22.4;
  A.matrix[4][2] = 23.6;
  A.matrix[4][3] = 24.8;
  A.matrix[4][4] = 25.0;

  matrix_t B;
  create_matrix(5, 5, &B);
  B.matrix[0][0] = 1.5;
  B.matrix[0][1] = 2.7;
  B.matrix[0][2] = 3.9;
  B.matrix[0][3] = 4.2;
  B.matrix[0][4] = 5.8;

  B.matrix[1][0] = 6.1;
  B.matrix[1][1] = 7.3;
  B.matrix[1][2] = 8.5;
  B.matrix[1][3] = 9.7;
  B.matrix[1][4] = 10.9;

  B.matrix[2][0] = 11.2;
  B.matrix[2][1] = 12.4;
  B.matrix[2][2] = 13.6;
  B.matrix[2][3] = 14.8;
  B.matrix[2][4] = 15.0;

  B.matrix[3][0] = 16.1;
  B.matrix[3][1] = 17.3;
  B.matrix[3][2] = 18.5;
  B.matrix[3][3] = 19.7;
  B.matrix[3][4] = 20.9;

  B.matrix[4][0] = 21.2;
  B.matrix[4][1] = 22.4;
  B.matrix[4][2] = 23.6;
  B.matrix[4][3] = 24.8;
  B.matrix[4][4] = 25.0;

  matrix_t mat;
  // Ожидаемая сумма (каждый элемент умножен на 2)
  double expected_sum[5][5] = {{3.0, 5.4, 7.8, 8.4, 11.6},
                               {12.2, 14.6, 17.0, 19.4, 21.8},
                               {22.4, 24.8, 27.2, 29.6, 30.0},
                               {32.2, 34.6, 37.0, 39.4, 41.8},
                               {42.4, 44.8, 47.2, 49.6, 50.0}};

  int result = sum_matrix(&A, &B, &mat);

  remove_matrix(&A);
  remove_matrix(&B);

  ck_assert_int_eq(OK, result);
  ck_assert_int_eq(5, mat.rows);
  ck_assert_int_eq(5, mat.columns);
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
      ck_assert_double_eq_tol(expected_sum[i][j], mat.matrix[i][j], 1e-6);
    }
  }
  remove_matrix(&mat);
}

Suite *sum_matrix_suite(void) {
  Suite *suite = suite_create("sum_matrix");
  TCase *tc = tcase_create("Core");

  tcase_add_test(tc, test_A_is_null);
  tcase_add_test(tc, test_B_is_null);
  tcase_add_test(tc, test_result_is_null);
  tcase_add_test(tc, test_not_eq_columns);
  tcase_add_test(tc, test_not_eq_rows);
  tcase_add_test(tc, test_invalid_rows);
  tcase_add_test(tc, test_invalid_columns);
  tcase_add_test(tc, test_A_matrix_null);
  tcase_add_test(tc, test_B_matrix_null);
  tcase_add_test(tc, test_A_and_B_matrix_null);
  tcase_add_test(tc, test_sum);
  tcase_add_test(tc, test_sum_4x4);
  tcase_add_test(tc, test_sum_5x5);

  suite_add_tcase(suite, tc);
  return suite;
}