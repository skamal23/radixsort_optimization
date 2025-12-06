#include "helpers.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>

void gen_random_ints(uint32_t *arr, size_t n) {
    srand(42);
    for(size_t i=0;i<n;i++){
        uint32_t random_int = rand();
        r=(r<<16)|rand();
        a[i]=r;
    }
}

void gen_sorted_ints(uint32_t *arr, size_t n) {
    for(size_t i=0;i<n;i++){
        arr[i]=(uint32_t)i;
    }
}

void gen_reversed_ints(uint32_t *arr, size_t n) {
    for(size_t i=0;i<n;i++){
        arr[i]=(uint32_t)(n-1-i);
    }
}

void gen_floats(float *arr, size_t n) {
    srand(42);
    for(size_t i=0;i<n;i++){
        float scale=(float)rand()/(float)RAND_MAX;
        arr[i]=(scale*2000.0f)-1000.0f; //Range from -1000 to 1000
    }
}

void gen_stability_data(tagged_int_t *arr, size_t n){
    srand(42);
    for(size_t i=0;i<n;i++){
        arr[i].key=rand()%256;
        arr[i].original_index=(uint32_t)i;
    }
}

