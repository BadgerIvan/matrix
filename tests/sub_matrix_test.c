#include <check.h>

#include "../src/matrix.h"

START_TEST(test_A_is_null) {
  matrix_t B;
  matrix_t mat;

  int result = sub_matrix(NULL, &B, &mat);

  ck_assert_int_eq(INVALID_MATRIX, result);
}
END_TEST

START_TEST(test_B_is_null) {
  matrix_t A;
  matrix_t mat;

  int result = sub_matrix(&A, NULL, &mat);

  ck_assert_int_eq(INVALID_MATRIX, result);
}
END_TEST

START_TEST(test_result_is_null) {
  matrix_t A;
  matrix_t B;

  int result = sub_matrix(&A, &B, NULL);

  ck_assert_int_eq(INVALID_MATRIX, result);
}
END_TEST

START_TEST(test_not_eq_rows) {
  matrix_t A;
  create_matrix(2, 1, &A);
  matrix_t B;
  create_matrix(1, 1, &B);
  matrix_t mat;

  int result = sub_matrix(&A, &B, &mat);

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

  int result = sub_matrix(&A, &B, &mat);

  remove_matrix(&A);
  remove_matrix(&B);

  ck_assert_int_eq(CALCULATION_ERROR, result);
}
END_TEST

START_TEST(test_invalid_rows) {
  matrix_t A = {.rows = -1, .columns = 1};
  matrix_t B = {.rows = -1, .columns = 1};
  matrix_t mat;

  int result = sub_matrix(&A, &B, &mat);

  ck_assert_int_eq(INVALID_MATRIX, result);
}
END_TEST

START_TEST(test_invalid_columns) {
  matrix_t A = {.rows = 1, .columns = -1};
  matrix_t B = {.rows = 1, .columns = -1};
  matrix_t mat;

  int result = sub_matrix(&A, &B, &mat);

  ck_assert_int_eq(INVALID_MATRIX, result);
}
END_TEST

START_TEST(test_A_matrix_null) {
  matrix_t A = {.rows = 1, .columns = 1, .matrix = NULL};
  matrix_t B;
  create_matrix(1, 1, &B);
  matrix_t mat;

  int result = sub_matrix(&A, &B, &mat);

  remove_matrix(&B);

  ck_assert_int_eq(INVALID_MATRIX, result);
}
END_TEST

START_TEST(test_B_matrix_null) {
  matrix_t A;
  create_matrix(1, 1, &A);
  matrix_t B = {.rows = 1, .columns = 1, .matrix = NULL};
  matrix_t mat;

  int result = sub_matrix(&A, &B, &mat);

  remove_matrix(&A);

  ck_assert_int_eq(INVALID_MATRIX, result);
}
END_TEST

START_TEST(test_A_and_B_matrix_null) {
  matrix_t A = {.rows = 1, .columns = 1, .matrix = NULL};
  matrix_t B = {.rows = 1, .columns = 1, .matrix = NULL};
  matrix_t mat;

  int result = sub_matrix(&A, &B, &mat);

  ck_assert_int_eq(INVALID_MATRIX, result);
}
END_TEST

START_TEST(test_sub) {
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
  double expected_sub[] = {-341882.6464, -4463.5132, -210.21158};

  int result = sub_matrix(&A, &B, &mat);

  remove_matrix(&A);
  remove_matrix(&B);

  ck_assert_int_eq(OK, result);
  ck_assert_int_eq(1, mat.rows);
  ck_assert_int_eq(3, mat.columns);
  for (int i = 0; i < 3; i++)
    ck_assert_double_eq_tol(expected_sub[i], mat.matrix[0][i], 1e-6);
  remove_matrix(&mat);
}

START_TEST(test_sub_4x4) {
  matrix_t A;
  create_matrix(4, 4, &A);
  A.matrix[0][0] = 3;
  A.matrix[0][1] = 3;
  A.matrix[0][2] = 3;
  A.matrix[0][3] = 3;
  A.matrix[1][0] = 3;
  A.matrix[1][1] = 3;
  A.matrix[1][2] = 3;
  A.matrix[1][3] = 3;
  A.matrix[2][0] = 3;
  A.matrix[2][1] = 3;
  A.matrix[2][2] = 3;
  A.matrix[2][3] = 3;
  A.matrix[3][0] = 3;
  A.matrix[3][1] = 3;
  A.matrix[3][2] = 3;
  A.matrix[3][3] = 3;
  matrix_t B;
  create_matrix(4, 4, &B);
  B.matrix[0][0] = 3;
  B.matrix[0][1] = 3;
  B.matrix[0][2] = 3;
  B.matrix[0][3] = 3;
  B.matrix[1][0] = 3;
  B.matrix[1][1] = 3;
  B.matrix[1][2] = 3;
  B.matrix[1][3] = 3;
  B.matrix[2][0] = 3;
  B.matrix[2][1] = 3;
  B.matrix[2][2] = 3;
  B.matrix[2][3] = 3;
  B.matrix[3][0] = 3;
  B.matrix[3][1] = 3;
  B.matrix[3][2] = 3;
  B.matrix[3][3] = 3;
  matrix_t mat;
  double expected_sum[4][4] = {0};

  int result = sub_matrix(&A, &B, &mat);

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

START_TEST(test_sub_5x5) {
  matrix_t A;
  create_matrix(5, 5, &A);
  A.matrix[0][0] = 10.5;
  A.matrix[0][1] = 12.7;
  A.matrix[0][2] = 15.9;
  A.matrix[0][3] = 18.2;
  A.matrix[0][4] = 20.8;

  A.matrix[1][0] = 25.1;
  A.matrix[1][1] = 27.3;
  A.matrix[1][2] = 30.5;
  A.matrix[1][3] = 33.7;
  A.matrix[1][4] = 35.9;

  A.matrix[2][0] = 40.2;
  A.matrix[2][1] = 42.4;
  A.matrix[2][2] = 45.6;
  A.matrix[2][3] = 48.8;
  A.matrix[2][4] = 50.0;

  A.matrix[3][0] = 55.1;
  A.matrix[3][1] = 57.3;
  A.matrix[3][2] = 60.5;
  A.matrix[3][3] = 63.7;
  A.matrix[3][4] = 65.9;

  A.matrix[4][0] = 70.2;
  A.matrix[4][1] = 72.4;
  A.matrix[4][2] = 75.6;
  A.matrix[4][3] = 78.8;
  A.matrix[4][4] = 80.0;

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

  double expected_sub[5][5] = {{9.0, 10.0, 12.0, 14.0, 15.0},
                               {19.0, 20.0, 22.0, 24.0, 25.0},
                               {29.0, 30.0, 32.0, 34.0, 35.0},
                               {39.0, 40.0, 42.0, 44.0, 45.0},
                               {49.0, 50.0, 52.0, 54.0, 55.0}};

  int result = sub_matrix(&A, &B, &mat);

  remove_matrix(&A);
  remove_matrix(&B);

  ck_assert_int_eq(OK, result);
  ck_assert_int_eq(5, mat.rows);
  ck_assert_int_eq(5, mat.columns);
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
      ck_assert_double_eq_tol(expected_sub[i][j], mat.matrix[i][j], 1e-6);
    }
  }
  remove_matrix(&mat);
}

Suite *sub_matrix_suite(void) {
  Suite *suite = suite_create("sub_matrix");
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
  tcase_add_test(tc, test_sub);
  tcase_add_test(tc, test_sub_4x4);
  tcase_add_test(tc, test_sub_5x5);

  suite_add_tcase(suite, tc);
  return suite;
}