/*
 * Programmer(s) : List all those who contributed in this file
 * Date: 
 * Desc: Sequential version of matrix vector multiplication. 
 * Note: you will have to make similar files for other 4 version as explained in assign1.md and a batch file to get running times of all 5 programs.
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

// Function to perform matrix-vector multiplication sequentially
void matrix_vector_multiply(double **matrix, double *vector, double *result, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        result[i] = 0;
        for (int j = 0; j < cols; j++) {
            result[i] += matrix[i][j] * vector[j];
        }
    }
}


int main(int argc, char* argv){
    if (argc != 3) {
        fprintf(stderr, "Usage: %d <matrix size> <vector size>\n", argv[0]);
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
    
    // Perform matrix-vector multiplication
    matrix_vector_multiply(matrix, vector, result, rows, cols);
    
    // Display result
    printf("Resultant vector after matrix-vector multiplication:\n");
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
}//end main
