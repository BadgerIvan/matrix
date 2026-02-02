#include "matrix.h"

#include <immintrin.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

int create_matrix(int rows, int columns, matrix_t *result) {
  if (rows <= 0 || columns <= 0 || !result) return INVALID_MATRIX;
  double **pointers = (double **)malloc(sizeof(double *) * rows);
  if (!pointers) return MEMORY_ERROR;
  double *matrix = (double *)calloc(rows * columns, sizeof(double));
  if (!matrix) {
    free(pointers);
    return MEMORY_ERROR;
  }
  for (int i = 0; i < rows; i++) pointers[i] = matrix + i * columns;
  result->rows = rows;
  result->columns = columns;
  result->matrix = pointers;
  return OK;
}

void remove_matrix(matrix_t *A) {
  if (!A) return;
  if (A->matrix) {
    free(A->matrix[0]);
    free(A->matrix);
    A->matrix = NULL;
  }
  A->columns = 0;
  A->rows = 0;
}

int eq_matrix(matrix_t *A, matrix_t *B) {
  if (!A || !B || A->columns != B->columns || A->rows != B->rows ||
      A->columns <= 0 || A->rows <= 0 || !A->matrix || !B->matrix) {
    return FAILURE;
  }
#ifdef __AVX__
  __m256d epsilon = _mm256_set1_pd(1e-6);
#endif  // __AVX__
  for (int i = 0; i < A->rows; i++) {
    int j = 0;
#ifdef __AVX__
    for (; j + 4 <= A->columns; j += 4) {
      __m256d a = _mm256_loadu_pd(&A->matrix[i][j]);
      __m256d b = _mm256_loadu_pd(&B->matrix[i][j]);
      __m256d sub = _mm256_sub_pd(a, b);
      __m256d abs_sub = _mm256_andnot_pd(_mm256_set1_pd(-0.0), sub);
      __m256d result = _mm256_cmp_pd(abs_sub, epsilon, _CMP_GT_OQ);
      int mask = _mm256_movemask_pd(result);
      if (mask != 0) return FAILURE;
    }
#endif  //__AVX__
    for (; j < A->columns; j++) {
      if (fabs(A->matrix[i][j] - B->matrix[i][j]) > 1e-6) return FAILURE;
    }
  }
  return SUCCESS;
}

int sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  if (!A || !B || !result || A->columns <= 0 || A->rows <= 0 || !A->matrix ||
      !B->matrix) {
    return INVALID_MATRIX;
  }
  if (A->columns != B->columns || A->rows != B->rows ||
      create_matrix(A->rows, A->columns, result) != OK) {
    return CALCULATION_ERROR;
  }
  for (int i = 0; i < A->rows; i++) {
    int j = 0;
#ifdef __AVX__
    for (; j + 4 <= A->columns; j += 4) {
      __m256d a = _mm256_loadu_pd(&A->matrix[i][j]);
      __m256d b = _mm256_loadu_pd(&B->matrix[i][j]);
      __m256d sum = _mm256_add_pd(a, b);
      _mm256_storeu_pd(&result->matrix[i][j], sum);
    }
#endif  //__AVX__
    for (; j < A->columns; j++) {
      result->matrix[i][j] = A->matrix[i][j] + B->matrix[i][j];
    }
  }
  return OK;
}

int sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  if (!A || !B || !result || A->columns <= 0 || A->rows <= 0 || !A->matrix ||
      !B->matrix) {
    return INVALID_MATRIX;
  }
  if (A->columns != B->columns || A->rows != B->rows ||
      create_matrix(A->rows, A->columns, result) != OK) {
    return CALCULATION_ERROR;
  }
  for (int i = 0; i < A->rows; i++) {
    int j = 0;
#ifdef __AVX__
    for (; j + 4 <= A->columns; j += 4) {
      __m256d a = _mm256_loadu_pd(&A->matrix[i][j]);
      __m256d b = _mm256_loadu_pd(&B->matrix[i][j]);
      __m256d sub = _mm256_sub_pd(a, b);
      _mm256_storeu_pd(&result->matrix[i][j], sub);
    }
#endif  //__AVX__
    for (; j < A->columns; j++) {
      result->matrix[i][j] = A->matrix[i][j] - B->matrix[i][j];
    }
  }
  return OK;
}

int mult_number(matrix_t *A, double number, matrix_t *result) {
  if (!A || !result || A->columns <= 0 || A->rows <= 0 || !A->matrix)
    return INVALID_MATRIX;
  if (create_matrix(A->rows, A->columns, result) != OK)
    return CALCULATION_ERROR;
#ifdef __AVX__
  __m256d num = _mm256_set1_pd(number);
#endif
  for (int i = 0; i < A->rows; i++) {
    int j = 0;
#ifdef __AVX__
    for (; j + 4 <= A->columns; j += 4) {
      __m256d a = _mm256_loadu_pd(&A->matrix[i][j]);
      __m256d res = _mm256_mul_pd(a, num);
      _mm256_storeu_pd(&result->matrix[i][j], res);
    }
#endif  //__AVX__
    for (; j < A->columns; j++) result->matrix[i][j] = A->matrix[i][j] * number;
  }
  return OK;
}

int mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  if (!A || !B || !result || A->columns <= 0 || A->rows <= 0 ||
      B->columns <= 0 || !A->matrix || !B->matrix) {
    return INVALID_MATRIX;
  }
  if (A->columns != B->rows) return CALCULATION_ERROR;
  if (create_matrix(A->rows, B->columns, result) != OK)
    return CALCULATION_ERROR;
  for (int i = 0; i < A->rows; i++)
    for (int j = 0; j < B->columns; j++)
      for (int k = 0; k < A->columns; k++)
        result->matrix[i][j] += A->matrix[i][k] * B->matrix[k][j];
  return OK;
}

int transpose(matrix_t *A, matrix_t *result) {
  if (!A || !result || A->columns <= 0 || A->rows <= 0 || !A->matrix)
    return INVALID_MATRIX;
  if (create_matrix(A->columns, A->rows, result) != OK)
    return CALCULATION_ERROR;
  for (int i = 0; i < A->rows; i++)
    for (int j = 0; j < A->columns; j++) result->matrix[j][i] = A->matrix[i][j];
  return OK;
}

static inline void make_minor_matrix(matrix_t *A, int a, int b,
                                     matrix_t *result) {
  int dst_row = 0;
  for (int i = 0; i < A->rows; i++) {
    if (a == i) continue;
    memcpy(result->matrix[dst_row], A->matrix[i], b * sizeof(double));
    memcpy(result->matrix[dst_row] + b, A->matrix[i] + b + 1,
           (A->columns - b - 1) * sizeof(double));
    dst_row++;
  }
}

int calc_complements(matrix_t *A, matrix_t *result) {
  if (!A || !result || A->columns <= 0 || A->rows <= 0 || !A->matrix)
    return INVALID_MATRIX;
  if (A->rows != A->columns ||
      create_matrix(A->rows, A->columns, result) != OK) {
    return CALCULATION_ERROR;
  }
  if (A->rows == 1) {
    result->matrix[0][0] = 1.0;
    return OK;
  }
  matrix_t mat = {0};
  if (create_matrix(A->rows - 1, A->columns - 1, &mat) != OK) {
    remove_matrix(result);
    return CALCULATION_ERROR;
  }
  int status = OK;
  for (int i = 0; i < A->rows && status == OK; i++) {
    for (int j = 0; j < A->columns && status == OK; j++) {
      make_minor_matrix(A, i, j, &mat);
      status = determinant(&mat, &result->matrix[i][j]);
      result->matrix[i][j] *= (i + j) % 2 == 0 ? 1 : -1;
    }
  }
  remove_matrix(&mat);
  if (status != OK) remove_matrix(result);
  return status;
}

inline static void swap_rows(matrix_t *A, int a, int b) {
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

static inline void copy_matrix(matrix_t *src, matrix_t *dst) {
  for (int i = 0; i < src->rows; i++) {
    int j = 0;
#ifdef __AVX__
    for (; j + 4 <= src->columns; j += 4) {
      __m256d src_pd = _mm256_loadu_pd(&src->matrix[i][j]);
      _mm256_storeu_pd(&dst->matrix[i][j], src_pd);
    }
#endif
    for (; j < src->columns; j++) dst->matrix[i][j] = src->matrix[i][j];
  }
}

int determinant(matrix_t *A, double *result) {
  if (!A || A->columns <= 0 || A->rows <= 0 || !A->matrix)
    return INVALID_MATRIX;
  if (A->columns != A->rows || !result) return CALCULATION_ERROR;
  if (A->rows == 1) {
    *result = A->matrix[0][0];
    return OK;
  }
  if (A->rows == 2) {
    *result =
        A->matrix[0][0] * A->matrix[1][1] - A->matrix[0][1] * A->matrix[1][0];
    return OK;
  }
  if (A->rows == 3) {
    *result = A->matrix[0][0] * A->matrix[1][1] * A->matrix[2][2] +
              A->matrix[0][1] * A->matrix[1][2] * A->matrix[2][0] +
              A->matrix[1][0] * A->matrix[2][1] * A->matrix[0][2] -
              A->matrix[0][2] * A->matrix[1][1] * A->matrix[2][0] -
              A->matrix[0][1] * A->matrix[1][0] * A->matrix[2][2] -
              A->matrix[1][2] * A->matrix[2][1] * A->matrix[0][0];
    return OK;
  }
  matrix_t mat;
  if (create_matrix(A->rows, A->columns, &mat) != OK) return CALCULATION_ERROR;
  double *ptr_for_free = mat.matrix[0];
  copy_matrix(A, &mat);
  *result = to_triangle_matrix(&mat);
  for (int i = 0; i < A->rows; i++) *result *= mat.matrix[i][i];
  free(ptr_for_free);
  free(mat.matrix);
  return OK;
}

int inverse_matrix(matrix_t *A, matrix_t *result) {
  if (!A || !result || A->columns <= 0 || A->rows <= 0 || !A->matrix)
    return INVALID_MATRIX;
  if (A->rows != A->columns) return CALCULATION_ERROR;
  double det = 0;
  int status = determinant(A, &det);
  if ((fabs(det)) < 1e-6 && status == OK) status = CALCULATION_ERROR;
  matrix_t mat_compl = {0};
  if (status == OK) status = calc_complements(A, &mat_compl);
  matrix_t mat_compl_transp = {0};
  if (status == OK) status = transpose(&mat_compl, &mat_compl_transp);
  if (status == OK) status = mult_number(&mat_compl_transp, 1.0 / det, result);
  if (mat_compl.matrix) remove_matrix(&mat_compl);
  if (mat_compl_transp.matrix) remove_matrix(&mat_compl_transp);
  return status;
}