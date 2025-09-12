#include <check.h>
#include <string.h>

#include "../src/matrix.h"

static void make_minor_matrix(matrix_t *A, int a, int b, matrix_t *result) {
  int dst_row = 0;
  for (int i = 0; i < A->rows; i++) {
    if (a == i) continue;
    memcpy(result->matrix[dst_row], A->matrix[i], b * sizeof(double));
    memcpy(result->matrix[dst_row] + b, A->matrix[i] + b + 1,
           (A->columns - b - 1) * sizeof(double));
    dst_row++;
  }
}

START_TEST(test_simple) {
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
  create_matrix(2, 2, &mat);

  make_minor_matrix(&A, 0, 0, &mat);

  for (int i = 1; i < 3; i++)
    for (int j = 1; j < 3; j++)
      ck_assert_double_eq_tol(A.matrix[i][j], mat.matrix[i - 1][j - 1], 1e-6);

  remove_matrix(&mat);
  remove_matrix(&A);
}

Suite *make_minor_matrix_suite(void) {
  Suite *suite = suite_create("make_minor_matrix");
  TCase *tc = tcase_create("Core");

  tcase_add_test(tc, test_simple);

  suite_add_tcase(suite, tc);
  return suite;
}