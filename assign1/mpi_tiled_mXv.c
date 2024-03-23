
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

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

// Function to perform matrix-vector multiplication using MPI with tiling
void matrix_vector_multiply_mpi_tiled(double **matrix, double *vector, double *result, int rows, int cols, int rank, int size) {
    int tile_size = 16; // Tiling size
    int local_rows = rows / size; // Number of rows per process
    int start_row = rank * local_rows; // Starting row index for this process
    int end_row = start_row + local_rows; // Ending row index for this process
    
    // Perform partial matrix-vector multiplication with tiling
    for (int i = start_row; i < end_row; i += tile_size) {
        for (int j = 0; j < cols; j += tile_size) {
            for (int ii = i; ii < i + tile_size && ii < end_row; ii++) {
                for (int jj = j; jj < j + tile_size && jj < cols; jj++) {
                    result[ii] += matrix[ii][jj] * vector[jj];
                }
            }
        }
    }
    
    // Gather results from all processes
    MPI_Allgather(MPI_IN_PLACE, local_rows, MPI_DOUBLE, result, local_rows, MPI_DOUBLE, MPI_COMM_WORLD);
}

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv); // Initialize MPI environment
    
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Get rank of the process
    MPI_Comm_size(MPI_COMM_WORLD, &size); // Get total number of processes
    
    if (argc != 3) {
        if (rank == 0) {
            fprintf(stderr, "Usage: %s <matrix size> <vector size>\n", argv[0]);
        }
        MPI_Finalize();
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
    srand(time(NULL) + rank); // Seed for random number generation
    fill_matrix(matrix, rows, cols);
    fill_vector(vector, vector_size);
    
    // Perform matrix-vector multiplication using MPI with tiling
    matrix_vector_multiply_mpi_tiled(matrix, vector, result, rows, cols, rank, size);
    
    // Display result
    if (rank == 0) {
        printf("Resultant vector after matrix-vector multiplication using MPI with tiling:\n");
        for (int i = 0; i < rows; i++) {
            printf("%lf ", result[i]);
        }
        printf("\n");
    }
    
    // Free dynamically allocated memory
    for (int i = 0; i < rows; i++) {
        free(matrix[i]);
    }
    free(matrix);
    free(vector);
    free(result);
    
    MPI_Finalize(); // Finalize MPI environment
    
    return 0;
}
