#include <stdio.h>
#include <stdlib.h>
#include <Accelerate/Accelerate.h>
#include <time.h>

void gemv_optimized(int m, int n, float alpha, float *A, float *x, float beta, float *y) {
    // A: m x n 매트릭스
    // x: n-벡터
    // y: m-벡터
    // alpha와 beta: 스칼라 계수

    // GEMV 연산 수행: y = alpha * A * x + beta * y
    cblas_sgemv(CblasRowMajor, CblasNoTrans, m, n, alpha, A, n, x, 1, beta, y, 1);
}

int main() {
    int m = 50000; // 행의 수
    int n = 1024; // 열의 수
    
    // 동적 메모리 할당
    float *A = (float*)malloc(m * n * sizeof(float));  // m x n 매트릭스
    float *x = (float*)malloc(n * sizeof(float));      // n 크기 벡터
    float *y = (float*)malloc(m * sizeof(float));      // m 크기 벡터
    
    if (A == NULL || x == NULL || y == NULL) {
        printf("Memory allocation failed\n");
        return -1;
    }
    
    // 예시 매트릭스 A (3x4)
    for (int i = 0; i < m * n; i++) {
        A[i] = 1.0;  // A에 예시 데이터 할당
    }

    // 벡터 x (크기 4)
    for (int i = 0; i < n; i++) {
        x[i] = 1.0;;  // x에 예시 데이터 할당
    }
    
    // 벡터 y (크기 3)
    for (int i = 0; i < m; i++) {
        y[i] = 0.0;  // y 초기화
    }
    
    // 알파와 베타 계수
    float alpha = 1.0;
    float beta = 0.0;

    // 타임 측정 시작
    clock_t start_time = clock();

    // GEMV 연산 수행
    gemv_optimized(m, n, alpha, A, x, beta, y);

    // 타임 측정 종료
    clock_t end_time = clock();
    
    // 연산 시간 계산
    double time_taken = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    
    // 데이터 크기 계산 (A는 m x n 매트릭스, x는 n 크기 벡터, y는 m 크기 벡터)
    size_t data_size = (m * n + n + m) * sizeof(float); // A, x, y 배열 크기 합산 (바이트 단위)
    
    // 대역폭 계산 (GB/s 단위)
    double bandwidth = data_size / (time_taken * 1e9); // 데이터 크기를 GB로 변환하고, 시간은 초로 변환

    // 결과 출력
    //printf("Resulting vector y: \n");
    //for (int i = 0; i < m; i++) {
    //    printf("%f\n", y[i]);
    //}
    
    // 타임과 대역폭 출력
    printf("\nTime taken: %f seconds\n", time_taken);
    printf("Data Size: %.2f bytes\n", data_size);
    printf("Bandwidth: %.2f GB/s\n", bandwidth);

    // 동적 메모리 해제
    free(A);
    free(x);
    free(y);

    return 0;
}

