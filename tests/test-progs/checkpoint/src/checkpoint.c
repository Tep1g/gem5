#include <stdio.h>
#include <stdint.h>
#include <gem5/m5ops.h>

int main(void) {
    float a = 4.8f;
    float b = 13.7f;
    float sum;
    float product;

    m5_reset_stats(0,0);
    sum = a+b;

    m5_checkpoint(0, 0);
    m5_reset_stats(0,0);
    product = a*b;
}