#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BUFFER_SIZE 100000000  // 1GB Memory Buffer

// Time Measurment
static inline uint64_t get_time_ns() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * 1000000000 + ts.tv_nsec;
}

void memory_read_bandwidth_test() {
    uint64_t start, end;
    uint64_t *src;

    src = (uint64_t*)malloc(BUFFER_SIZE*sizeof(uint64_t));
   
    // Timer Start
    start = get_time_ns();

    // Memory Read (ARM64 인라인 어셈블리)
    asm volatile (
	"mov x0, %[src]\n"
	"mov x1, %[size]\n"
	"1:\n"
	"ldr w2, [x0], #64\n"
	"subs x1, x1, #8\n"
	"bne 1b\n"
	:
	: [src] "r" (src), [size] "r" (BUFFER_SIZE/sizeof(uint64_t))
	: "x0", "x1", "x2"
	);
	
    // Timer End
    end = get_time_ns();

    // Time and Bandwidth Calculation
    double elapsed_time = (double)(end - start) / 1e9; // Second Unit
    double bandwidth = (double)(BUFFER_SIZE) / elapsed_time / 1e6 * sizeof(double);
    free(src);

    printf("Read Elapsed Time: %.6f seconds\n", elapsed_time);
    printf("Read Memory Bandwidth: %.2f MB/s\n", bandwidth);
}

int main() {
    printf("Memory Read Bandwidth Test:\n");

    memory_read_bandwidth_test();
    
    return 0;
}
