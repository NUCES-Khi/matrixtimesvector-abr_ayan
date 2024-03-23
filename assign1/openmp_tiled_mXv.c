
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

// Function to allocate memory for a matrix
double **allocate_matrix(int rows, int cols) {
    double **matrix = (double **)malloc(rows * sizeof(double *));
    if (matrix == NULL) {
        fprintf(stderr, "Memory allocation failed for matrix\n");
        exit(1);
    }
    for (int i = 0; i < rows; i++) {
        matrix[i] = (double *)malloc(cols * sizeof(double));
        if (matrix[i] == NULL) {
            fprintf(stderr, "Memory allocation failed for matrix\n");
            exit(1);
        }
    }
    return matrix;
}

// Function to allocate memory for a vector
double *allocate_vector(int size) {
    double *vector = (double *)malloc(size * sizeof(double));
    if (vector == NULL) {
        fprintf(stderr, "Memory allocation failed for vector\n");
        exit(1);
    }
    return vector;
}

// Function to generate random values for a matrix
void fill_matrix(double **matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = rand() % 10; // Generating random values between 0 and 9
        }
    }
}

// Function to generate random values for a vector
void fill_vector(double *vector, int size) {
    for (int i = 0; i < size; i++) {
        vector[i] = rand() % 10; // Generating random values between 0 and 9
    }
}

// Function to perform matrix-vector multiplication using OpenMP with tiling
void matrix_vector_multiply_openmp_tiled(double **matrix, double *vector, double *result, int rows, int cols) {
    int tile_size = 16; // Tiling size
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < rows; i += tile_size) {
        for (int j = 0; j < cols; j += tile_size) {
            for (int ii = i; ii < i + tile_size; ii++) {
                for (int jj = j; jj < j + tile_size; jj++) {
                    result[ii] += matrix[ii][jj] * vector[jj];
                }
            }
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <matrix size> <vector size>\n", argv[0]);
        exit(1);
    }
    
    int rows = atoi(argv[1]); // Number of rows in the matrix
    int cols = atoi(argv[1]); // Number of columns in the matrix
    int vector_size = atoi(argv[2]); // Size of the vector
    
    // Allocate memory for matrix, vector, and result
    double **matrix = allocate_matrix(rows, cols);
    double *vector = allocate_vector(vector_size);
    double *result = allocate_vector(rows);
    
    // Fill matrix and vector with random values
    srand(time(NULL)); // Seed for random number generation
    fill_matrix(matrix, rows, cols);
    fill_vector(vector, vector_size);
    
    // Perform matrix-vector multiplication using OpenMP with tiling
    matrix_vector_multiply_openmp_tiled(matrix, vector, result, rows, cols);
    
    // Display result
    printf("Resultant vector after matrix-vector multiplication using OpenMP with tiling:\n");
    for (int i = 0; i < rows; i++) {
        printf("%lf ", result[i]);
    }
    printf("\n");
    
    // Free dynamically allocated memory
    for (int i = 0; i < rows; i++) {
        free(matrix[i]);
    }
    free(matrix);
    free(vector);
    free(result);
    
    return 0;
}
