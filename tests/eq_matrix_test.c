#include <check.h>
#include <stdlib.h>

#include "../src/matrix.h"

START_TEST(test_A_is_null) {
  matrix_t B;

  int result = eq_matrix(NULL, &B);

  ck_assert_int_eq(FAILURE, result);
}
END_TEST

START_TEST(test_B_is_null) {
  matrix_t A;

  int result = eq_matrix(&A, NULL);

  ck_assert_int_eq(FAILURE, result);
}
END_TEST

START_TEST(test_not_eq_rows) {
  matrix_t A = {.rows = 2, .columns = 1};
  matrix_t B = {.rows = 1, .columns = 1};

  int result = eq_matrix(&A, &B);

  ck_assert_int_eq(FAILURE, result);
}
END_TEST

START_TEST(test_not_eq_columns) {
  matrix_t A = {.rows = 1, .columns = 2};
  matrix_t B = {.rows = 1, .columns = 1};

  int result = eq_matrix(&A, &B);

  ck_assert_int_eq(FAILURE, result);
}
END_TEST

START_TEST(test_invalid_rows) {
  matrix_t A = {.rows = -1, .columns = 1};
  matrix_t B = {.rows = -1, .columns = 1};

  int result = eq_matrix(&A, &B);

  ck_assert_int_eq(FAILURE, result);
}
END_TEST

START_TEST(test_invalid_columns) {
  matrix_t A = {.rows = 1, .columns = -1};
  matrix_t B = {.rows = 1, .columns = -1};

  int result = eq_matrix(&A, &B);

  ck_assert_int_eq(FAILURE, result);
}
END_TEST

START_TEST(test_A_matrix_null) {
  matrix_t A = {.rows = 1, .columns = 1, .matrix = NULL};
  matrix_t B;
  create_matrix(1, 1, &B);

  int result = eq_matrix(&A, &B);

  remove_matrix(&B);

  ck_assert_int_eq(FAILURE, result);
}
END_TEST

START_TEST(test_B_matrix_null) {
  matrix_t A;
  create_matrix(1, 1, &A);
  matrix_t B = {.rows = 1, .columns = 1, .matrix = NULL};

  int result = eq_matrix(&A, &B);

  remove_matrix(&A);

  ck_assert_int_eq(FAILURE, result);
}
END_TEST

START_TEST(test_A_and_B_matrix_null) {
  matrix_t A = {.rows = 1, .columns = 1, .matrix = NULL};
  matrix_t B = {.rows = 1, .columns = 1, .matrix = NULL};

  int result = eq_matrix(&A, &B);

  ck_assert_int_eq(FAILURE, result);
}
END_TEST

START_TEST(test_matrix_six_digits_eq) {
  matrix_t A;
  create_matrix(3, 3, &A);
  matrix_t B;
  create_matrix(3, 3, &B);
  A.matrix[0][0] = 1.123456;
  B.matrix[0][0] = 1.1234568;

  int result = eq_matrix(&A, &B);

  remove_matrix(&A);
  remove_matrix(&B);

  ck_assert_int_eq(SUCCESS, result);
}
END_TEST

START_TEST(test_matrix_six_digits_not_eq) {
  matrix_t A;
  create_matrix(3, 3, &A);
  matrix_t B;
  create_matrix(3, 3, &B);
  A.matrix[0][0] = 1.123456;
  B.matrix[0][0] = 1.123458;

  int result = eq_matrix(&A, &B);

  remove_matrix(&A);
  remove_matrix(&B);

  ck_assert_int_eq(FAILURE, result);
}
END_TEST

START_TEST(test_matrix_4x4_eq) {
  matrix_t A;
  create_matrix(4, 4, &A);
  matrix_t B;
  create_matrix(4, 4, &B);

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      A.matrix[i][j] = 1.123456 + i * 0.1 + j * 0.01;
      B.matrix[i][j] = 1.123456 + i * 0.1 + j * 0.01;
    }
  }

  int result = eq_matrix(&A, &B);

  remove_matrix(&A);
  remove_matrix(&B);

  ck_assert_int_eq(SUCCESS, result);
}
END_TEST

START_TEST(test_matrix_4x4_not_eq) {
  matrix_t A;
  create_matrix(4, 4, &A);
  matrix_t B;
  create_matrix(4, 4, &B);

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      A.matrix[i][j] = 1.123456 + i * 0.1 + j * 0.01;
      B.matrix[i][j] = 1.123456 + i * 0.1 + j * 0.01;
    }
  }
  B.matrix[2][3] = 1.123458;

  int result = eq_matrix(&A, &B);

  remove_matrix(&A);
  remove_matrix(&B);

  ck_assert_int_eq(FAILURE, result);
}
END_TEST

START_TEST(test_matrix_5x5_eq) {
  matrix_t A;
  create_matrix(5, 5, &A);
  matrix_t B;
  create_matrix(5, 5, &B);

  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
      A.matrix[i][j] = 2.987654 - i * 0.05 + j * 0.02;
      B.matrix[i][j] = 2.987654 - i * 0.05 + j * 0.02;
    }
  }

  int result = eq_matrix(&A, &B);

  remove_matrix(&A);
  remove_matrix(&B);

  ck_assert_int_eq(SUCCESS, result);
}
END_TEST

START_TEST(test_matrix_5x5_not_eq) {
  matrix_t A;
  create_matrix(5, 5, &A);
  matrix_t B;
  create_matrix(5, 5, &B);

  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
      A.matrix[i][j] = 2.987654 - i * 0.05 + j * 0.02;
      B.matrix[i][j] = 2.987654 - i * 0.05 + j * 0.02;
    }
  }
  B.matrix[4][0] = 2.987652;

  int result = eq_matrix(&A, &B);

  remove_matrix(&A);
  remove_matrix(&B);

  ck_assert_int_eq(FAILURE, result);
}
END_TEST

START_TEST(test_matrix_5x5_near_threshold) {
  matrix_t A;
  create_matrix(5, 5, &A);
  matrix_t B;
  create_matrix(5, 5, &B);

  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
      A.matrix[i][j] = 1.0 + i * 0.1 + j * 0.01;
      B.matrix[i][j] = 1.0 + i * 0.1 + j * 0.01 + 1e-7;
    }
  }

  int result = eq_matrix(&A, &B);

  remove_matrix(&A);
  remove_matrix(&B);

  ck_assert_int_eq(SUCCESS, result);
}
END_TEST

Suite *eq_matrix_suite(void) {
  Suite *suite = suite_create("eq_matrix");
  TCase *tc = tcase_create("Core");

  tcase_add_test(tc, test_A_is_null);
  tcase_add_test(tc, test_B_is_null);
  tcase_add_test(tc, test_not_eq_columns);
  tcase_add_test(tc, test_not_eq_rows);
  tcase_add_test(tc, test_invalid_rows);
  tcase_add_test(tc, test_invalid_columns);
  tcase_add_test(tc, test_A_matrix_null);
  tcase_add_test(tc, test_B_matrix_null);
  tcase_add_test(tc, test_A_and_B_matrix_null);
  tcase_add_test(tc, test_matrix_six_digits_eq);
  tcase_add_test(tc, test_matrix_six_digits_not_eq);
  tcase_add_test(tc, test_matrix_4x4_eq);
  tcase_add_test(tc, test_matrix_4x4_not_eq);
  tcase_add_test(tc, test_matrix_5x5_eq);
  tcase_add_test(tc, test_matrix_5x5_not_eq);
  tcase_add_test(tc, test_matrix_5x5_near_threshold);

  suite_add_tcase(suite, tc);

  return suite;
}