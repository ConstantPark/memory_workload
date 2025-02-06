#include <stdio.h>
#include <stdlib.h>
#include <Accelerate/Accelerate.h> // This code is tested on Apple Silicon
#include <time.h>

void gemv_optimized(int m, int n, float alpha, float *A, float *x, float beta, float *y) {
    // A: m x n Matrix
    // x: N-Vector
    // y: M-Vector
    // alpha & beta: Number of Scalar

    // GEMV Computaiton: y = alpha * A * x + beta * y
    cblas_sgemv(CblasRowMajor, CblasNoTrans, m, n, alpha, A, n, x, 1, beta, y, 1);
}

int main() {
    int m = 50000; // Number of Row
    int n = 1024; // Number of Column
    
    // Memory Allocaiton
    float *A = (float*)malloc(m * n * sizeof(float));  // m x n Matrix
    float *x = (float*)malloc(n * sizeof(float));      // N-Vector
    float *y = (float*)malloc(m * sizeof(float));      // M-Vector
    
    if (A == NULL || x == NULL || y == NULL) {
        printf("Memory allocation failed\n");
        return -1;
    }

    
    for (int i = 0; i < m * n; i++) {
        A[i] = 1.0;  
    }

    for (int i = 0; i < n; i++) {
        x[i] = 1.0;;  
    }
    \
    for (int i = 0; i < m; i++) {
        y[i] = 0.0;  
    }
    
    // Alpha, Beta Value
    float alpha = 1.0;
    float beta = 0.0;

    // Time Measurement
    clock_t start_time = clock();

    // GEMV Operaiton
    gemv_optimized(m, n, alpha, A, x, beta, y);

    // Time Measurement    
    clock_t end_time = clock(); 
    
    // Time Calculation
    double time_taken = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    
    // Data Size (A is a m x n matrix, x is a n-sized vector, y is a m-sized vector)
    size_t data_size = (m * n + n + m) * sizeof(float); // Summation of A, x, y array size (Byte)
    
    // Bandwidth Calculation (GB/s Unit)
    double bandwidth = data_size / (time_taken * 1e9); 
    
    // Print Time and Bandwidth
    printf("\nTime taken: %f seconds\n", time_taken);
    printf("Data Size: %.2f bytes\n", data_size);
    printf("Bandwidth: %.2f GB/s\n", bandwidth);

    // Memory Free
    free(A);
    free(x);
    free(y);

    return 0;
}

