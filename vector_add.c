#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>

#include <arm_neon.h> // For NEON intrinsic functions

#define VECTOR_SIZE 100000000

// 메모리 초기화 함수
void initialize_vectors(float *a, float *b, float *c, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        a[i] = (float)i;
        b[i] = (float)(size - i);
        c[i] = 0.0f;
    }
}

// 대역폭 측정 함수
void measure_bandwidth(size_t bytes, double elapsed_time) {
    double bandwidth = (double)bytes / (elapsed_time * 1e9); // GB/s
    printf("Data Size: %zu bytes\n", bytes);
    printf("Elapsed Time: %.6f seconds\n", elapsed_time);
    printf("Bandwidth: %.6f GB/s\n", bandwidth);
}

// 벡터 덧셈 함수
void vector_addition(float *a, float *b, float *c, size_t size) {
    size_t i;
    for (i = 0; i + 4 <= size; i += 4) {
        float32x4_t va = vld1q_f32(&a[i]); // Load 4 floats from a[i]
        float32x4_t vb = vld1q_f32(&b[i]); // Load 4 floats from b[i]
        float32x4_t vc = vaddq_f32(va, vb); // Perform vector addition
        vst1q_f32(&c[i], vc); // Store 4 floats into c[i]
    }

    // 남은 요소 처리
    for (; i < size; ++i) {
        c[i] = a[i] + b[i];
    }
}

int main() {
    float *a = (float *)aligned_alloc(16, VECTOR_SIZE * sizeof(float));
    float *b = (float *)aligned_alloc(16, VECTOR_SIZE * sizeof(float));
    float *c = (float *)aligned_alloc(16, VECTOR_SIZE * sizeof(float));

    if (!a || !b || !c) {
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }

    initialize_vectors(a, b, c, VECTOR_SIZE);

    clock_t start_time = clock();

    vector_addition(a, b, c, VECTOR_SIZE);

    clock_t end_time = clock();

    double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    measure_bandwidth(VECTOR_SIZE * sizeof(float) * 3, elapsed_time);

    // 결과 확인 (테스트용)
    printf("c[0] = %f, c[%zu] = %f\n", c[0], VECTOR_SIZE - 1, c[VECTOR_SIZE - 1]);

    free(a);
    free(b);
    free(c);

    return 0;
}

