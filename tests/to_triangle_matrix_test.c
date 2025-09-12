#include <check.h>
#include <immintrin.h>
#include <stdio.h>
#include <stdlib.h>

#include "../src/matrix.h"

static void swap_rows(matrix_t *A, int a, int b) {
  double *row = A->matrix[a];
  A->matrix[a] = A->matrix[b];
  A->matrix[b] = row;
}

static int to_triangle_matrix(matrix_t *A) {
  int sign = 1;
  for (int i = 0; i < A->rows; i++) {
    int index_max_ell_in_col = 0;
    double max_ell_in_col = 0;
    for (int j = i; j < A->columns; j++) {
      if (fabs(A->matrix[j][i]) > max_ell_in_col) {
        index_max_ell_in_col = j;
        max_ell_in_col = A->matrix[j][i];
      }
    }
    if (index_max_ell_in_col != i) {
      swap_rows(A, index_max_ell_in_col, i);
      sign *= -1;
    }
    for (int j = i + 1; j < A->rows; j++) {
      double factor = A->matrix[j][i] / A->matrix[i][i];
      int k = i;
#ifdef __AVX__
      __m256d factor_vec = _mm256_set1_pd(factor);
      for (; k + 4 <= A->columns; k += 4) {
        __m256d upper_row = _mm256_loadu_pd(&A->matrix[i][k]);
        __m256d row = _mm256_loadu_pd(&A->matrix[j][k]);
        __m256d mul = _mm256_mul_pd(upper_row, factor_vec);
        __m256d sub = _mm256_sub_pd(row, mul);
        _mm256_storeu_pd(&A->matrix[j][k], sub);
      }
#endif  // __AVX__
      for (; k < A->columns; k++) {
        A->matrix[j][k] -= factor * A->matrix[i][k];
      }
    }
  }
  return sign;
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

  double *ptr_for_free = A.matrix[0];

  matrix_t res;
  create_matrix(3, 3, &res);
  res.matrix[0][0] = 456.123000;
  res.matrix[0][1] = 2007.200700;
  res.matrix[0][2] = 14.500000;
  res.matrix[1][0] = 0.000000;
  res.matrix[1][1] = 1462.923963;
  res.matrix[1][2] = 11.565374;
  res.matrix[2][0] = 0.000000;
  res.matrix[2][1] = 0.000000;
  res.matrix[2][2] = -1.128574;

  int sign = to_triangle_matrix(&A);

  ck_assert_int_eq(sign, -1);
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++)
      ck_assert_double_eq_tol(res.matrix[i][j], A.matrix[i][j], 1e-6);

  free(ptr_for_free);
  free(A.matrix);
  remove_matrix(&res);
}
END_TEST

Suite *to_triangle_matrix_suite(void) {
  Suite *suite = suite_create("to_triangle_matrix");
  TCase *tc = tcase_create("Core");

  tcase_add_test(tc, test_simple);

  suite_add_tcase(suite, tc);
  return suite;
}