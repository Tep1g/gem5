#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#define NUM_ELEMENTS 250

int main() {
    float *float_arr = (float *) malloc(NUM_ELEMENTS * sizeof(float));
    for (uint8_t i=0; i < NUM_ELEMENTS; i++) {
        float_arr[i] = (float)i;
    }

    float sum = 0;
    for (uint8_t i=0; i < NUM_ELEMENTS; i++) {
        sum += float_arr[i];
    }
    printf("Sum: %f\n", sum);
}