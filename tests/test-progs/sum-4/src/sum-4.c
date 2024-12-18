#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/time.h>

#define NUM_ROWS 50
#define NUM_COLUMNS 50

static uint8_t row_indexes[NUM_ROWS] = {0};
static uint8_t column_indexes[NUM_COLUMNS] = {0};

static void shuffle(uint8_t arr[], uint8_t arr_len) {
    // Seed the random number generator
    srand(time(NULL));
    
    for (uint8_t i = arr_len - 1; i > 0; i--) {
        // Pick a random index from 0 to i
        uint8_t j = rand() % (i + 1);

        // Swap arr[i] with the element at random index
        uint8_t temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}

static void generate_indexes(uint8_t *indexes) {
    for (uint8_t i = 0; i < NUM_ROWS; i++) {
        indexes[i] = i;
    }
}

struct matrix_info {
    uint32_t **matrix;
};

void *sum_by_row_major(void *threadid) {
    struct timeval t_start, t_stop, t_result;
    gettimeofday(&t_start, NULL);
    struct matrix_info *info = (struct matrix_info *) threadid;
    printf("Row major thread started\n");
    uint32_t sum = 0;
    for (int i = 0; i < NUM_ROWS; i++) {
        uint8_t row = row_indexes[i];
        for (int j = 0; j < NUM_COLUMNS; j++) {
            sum += info->matrix[row][j];
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
        uint8_t column = column_indexes[j];
        for (int i = 0; i < NUM_ROWS; i++) {
            sum += info->matrix[i][column];
        }
    }
    gettimeofday(&t_stop, NULL);
    timersub(&t_stop, &t_start, &t_result);
    printf("Column major thread finished in %u microseconds\nSum: %u\n", (long int)t_result.tv_usec, sum);
}

int main() {
    generate_indexes(row_indexes);
    generate_indexes(column_indexes);
    shuffle(row_indexes, NUM_ROWS);
    shuffle(column_indexes, NUM_COLUMNS);
    uint32_t **matrix1 = (uint32_t **)malloc(NUM_ROWS * sizeof(uint32_t *));
    uint32_t **matrix2 = (uint32_t **)malloc(NUM_ROWS * sizeof(uint32_t *));

    for (int i = 0; i < NUM_ROWS; i++) {
        matrix1[i] = (uint32_t *)malloc(NUM_COLUMNS * sizeof(uint32_t));
        matrix2[i] = (uint32_t *)malloc(NUM_COLUMNS * sizeof(uint32_t));
    }

    for (int i = 0; i < NUM_ROWS; i++) {
        for (int j = 0; j < NUM_COLUMNS; j++) {
            matrix1[i][j] = j;
            matrix2[i][j] = j;
        }
    }
    struct matrix_info info1;
    struct matrix_info info2;
    info1.matrix = matrix1;
    info2.matrix = matrix2;
    
    pthread_t column_major_thread;
    pthread_t row_major_thread;
    pthread_create(&column_major_thread, NULL, sum_by_column_major, (void *)&info1);
    pthread_create(&row_major_thread, NULL, sum_by_row_major, (void *)&info2);

    pthread_join(column_major_thread, NULL);
    pthread_join(row_major_thread, NULL);
    return 0;
}