#include <check.h>

#include "../src/matrix.h"

START_TEST(test_A_is_null) {
  matrix_t mat;

  int result = calc_complements(NULL, &mat);

  ck_assert_int_eq(INVALID_MATRIX, result);
}
END_TEST

START_TEST(test_result_is_null) {
  matrix_t A;

  int result = calc_complements(&A, NULL);

  ck_assert_int_eq(INVALID_MATRIX, result);
}
END_TEST

START_TEST(test_invalid_rows) {
  matrix_t A = {.rows = -1, .columns = 1};
  matrix_t mat;

  int result = calc_complements(&A, &mat);

  ck_assert_int_eq(INVALID_MATRIX, result);
}
END_TEST

START_TEST(test_invalid_columns) {
  matrix_t A = {.rows = 1, .columns = -1};
  matrix_t mat;

  int result = calc_complements(&A, &mat);

  ck_assert_int_eq(INVALID_MATRIX, result);
}
END_TEST

START_TEST(test_A_matrix_null) {
  matrix_t A = {.rows = 1, .columns = 1, .matrix = NULL};
  matrix_t mat;

  int result = calc_complements(&A, &mat);

  ck_assert_int_eq(INVALID_MATRIX, result);
}
END_TEST

START_TEST(test_calc_complements_1x1) {
  matrix_t A;
  create_matrix(1, 1, &A);
  A.matrix[0][0] = 123.456;
  matrix_t mat;

  int result = calc_complements(&A, &mat);

  remove_matrix(&A);

  ck_assert_int_eq(OK, result);
  ck_assert_int_eq(1, mat.rows);
  ck_assert_int_eq(1, mat.columns);
  ck_assert_double_eq_tol(1.0, mat.matrix[0][0], 1e-6);

  remove_matrix(&mat);
}

START_TEST(test_calc_complements_3x3) {
  matrix_t A;
  create_matrix(3, 3, &A);
  A.matrix[0][0] = 123.456;
  A.matrix[0][1] = 2006.2006;
  A.matrix[0][2] = 15.49;
  A.matrix[1][0] = 456.123;
  A.matrix[1][1] = 2007.2007;
  A.matrix[1][2] = 14.50;
  A.matrix[2][0] = 412.563;
  A.matrix[2][1] = 2008.2008;
  A.matrix[2][2] = 13.51;
  matrix_t mat;
  matrix_t res;
  create_matrix(3, 3, &res);
  res.matrix[0][0] = -2001.6301429999985;
  res.matrix[0][1] = -180.05823000000055;
  res.matrix[0][2] = 87889.83110429998;
  res.matrix[1][0] = 4003.2602860000006;
  res.matrix[1][1] = -4722.71031;
  res.matrix[1][2] = 579759.7001729999;
  res.matrix[2][0] = -2001.6301430000021;
  res.matrix[2][1] = 5275.23327;
  res.matrix[2][2] = -667273.2666545999;

  int result = calc_complements(&A, &mat);

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

Suite *calc_complements_suite(void) {
  Suite *suite = suite_create("calc_complements");
  TCase *tc = tcase_create("Core");

  tcase_add_test(tc, test_A_is_null);
  tcase_add_test(tc, test_result_is_null);
  tcase_add_test(tc, test_invalid_rows);
  tcase_add_test(tc, test_invalid_columns);
  tcase_add_test(tc, test_A_matrix_null);
  tcase_add_test(tc, test_calc_complements_1x1);
  tcase_add_test(tc, test_calc_complements_3x3);

  suite_add_tcase(suite, tc);
  return suite;
}