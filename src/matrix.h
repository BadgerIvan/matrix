#ifndef MATRIX_H_
#define MATRIX_H_

typedef struct matrix_struct {
  double **matrix;
  int rows;
  int columns;
} matrix_t;

#define OK 0
#define INVALID_MATRIX 1
#define CALCULATION_ERROR 2
#define MEMORY_ERROR 3
int create_matrix(int rows, int columns, matrix_t *result);
void remove_matrix(matrix_t *A);
int sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result);
int sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result);
int mult_number(matrix_t *A, double number, matrix_t *result);
int mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result);
int transpose(matrix_t *A, matrix_t *result);
int calc_complements(matrix_t *A, matrix_t *result);
int determinant(matrix_t *A, double *result);
int inverse_matrix(matrix_t *A, matrix_t *result);

#define SUCCESS 1
#define FAILURE 0
int eq_matrix(matrix_t *A, matrix_t *B);

#endif