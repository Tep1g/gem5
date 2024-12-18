#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/time.h>

#define NUM_ROWS 50
#define NUM_COLUMNS 50

struct matrix_info {
    uint32_t **matrix;
};

void *sum_by_row_major(uint32_t **matrix) {
    struct timeval t_start, t_stop, t_result;
    gettimeofday(&t_start, NULL);
    printf("Row major thread started\n");
    uint32_t sum = 0;
    for (int i = 0; i < NUM_ROWS; i++) {
        for (int j = 0; j < NUM_COLUMNS; j++) {
            sum += matrix[i][j];
        }
    }
    gettimeofday(&t_stop, NULL);
    timersub(&t_stop, &t_start, &t_result);
    printf("Row major thread finished in %u microseconds\nSum: %u\n", (long int)t_result.tv_usec, sum);
}

void *sum_by_column_major(void *threadid) {
    struct timeval t_start, t_stop, t_result;
    gettimeofday(&t_start, NULL);
    struct matrix_info *info = (struct matrix_info *) threadid;
    printf("Column major thread started\n");
    uint32_t sum = 0;
    for (int j = 0; j < NUM_COLUMNS; j++) {
        for (int i = 0; i < NUM_ROWS; i++) {
            sum += info->matrix[i][j];
        }
    }
    gettimeofday(&t_stop, NULL);
    timersub(&t_stop, &t_start, &t_result);
    printf("Column major thread finished in %u microseconds\nSum: %u\n", (long int)t_result.tv_usec, sum);
}

int main() {
    uint32_t **matrix = (uint32_t **)malloc(NUM_ROWS * sizeof(uint32_t *));

    for (int i = 0; i < NUM_ROWS; i++) {
        matrix[i] = (uint32_t *)malloc(NUM_COLUMNS * sizeof(uint32_t));
    }

    for (int i = 0; i < NUM_ROWS; i++) {
        for (int j = 0; j < NUM_COLUMNS; j++) {
            matrix[i][j] = rand();
        }
    }
    struct matrix_info info;
    info.matrix = matrix;
    
    pthread_t column_major_thread;
    pthread_create(&column_major_thread, NULL, sum_by_column_major, (void *)&info);

    // Execute the last thread with this thread context to appease SE mode
    sum_by_row_major(matrix);

    pthread_join(column_major_thread, NULL);
    return 0;
}