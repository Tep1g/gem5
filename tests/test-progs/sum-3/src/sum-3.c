#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#define NUM_ROWS 35
#define NUM_COLUMNS 35

struct matrix_info {
    uint32_t **matrix;
};

void *sum_by_row_major(void *threadid) {
    struct matrix_info *info = (struct matrix_info *) threadid;
    uint32_t sum = 0;
    for (int i = 0; i < NUM_ROWS; i++) {
        for (int j = 0; j < NUM_COLUMNS; j++) {
            sum += info->matrix[i][j];
        }
    }
    printf("Row major thread finished at tick %d\n", clock());
}

void *sum_by_column_major(void *threadid) {
    struct matrix_info *info = (struct matrix_info *) threadid;
    uint32_t sum = 0;
    for (int j = 0; j < NUM_COLUMNS; j++) {
        for (int i = 0; i < NUM_ROWS; i++) {
            sum += info->matrix[i][j];
        }
    }
    printf("Column major thread finished at tick %d\n", clock());
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
    
    pthread_t row_major_thread;
    pthread_t column_major_thread;
    pthread_create(&row_major_thread, NULL, sum_by_row_major, (void *)&info);
    pthread_create(&column_major_thread, NULL, sum_by_column_major, (void *)&info);
    pthread_join(row_major_thread, NULL);
    pthread_join(column_major_thread, NULL);
    return 0;
}