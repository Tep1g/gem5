#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

static void sum_arr(uint8_t *arr, uint8_t arr_len, uint8_t *sum) {
    for (int i = 0; i < arr_len; i++) {
        *sum += arr[i];
    }
}

static uint8_t *allocate_mem(void) {
    // Create pointer to and allocate heap memory
    uint8_t *heap_ptr = (uint8_t*)malloc(4 * sizeof(uint8_t));

    // Store values in heap
    heap_ptr[0] = 1;
    heap_ptr[1] = 200;
    heap_ptr[2] = 32;
    heap_ptr[3] = 3;

    return heap_ptr;
}

int main() {

    uint8_t *heap_ptr = allocate_mem();

    // Calculate sum from the values in the heap
    uint8_t sum;
    sum_arr(heap_ptr, 4, &sum);

    // Free memory
    free(heap_ptr);

    // Print sum
    printf("%d", sum);

    return 0;
}