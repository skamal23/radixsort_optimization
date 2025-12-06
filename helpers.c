#include "helpers.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

void gen_random_ints(uint32_t *arr, size_t n) {
    srand(42);
    for(size_t i = 0; i < n; i++) {
        uint32_t r = (rand() & 0xFF) | 
                     ((rand() & 0xFF) << 8) | 
                     ((rand() & 0xFF) << 16) | 
                     ((rand() & 0xFF) << 24);
        arr[i] = r;
    }
}

void gen_sorted_ints(uint32_t *arr, size_t n) {
    for(size_t i=0;i<n;i++){
        arr[i]=(uint32_t)i;
    }
}

void gen_reverse_ints(uint32_t *arr, size_t n) {
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

//Verifying helper functions
void verify_sorted(uint32_t *arr, size_t n){
    for(size_t i=1;i<n;i++){
        if(arr[i-1]>arr[i]){
            fprintf(stderr,"Integer array is not sorted\n");
            exit(1);
        }
    }
    printf("The integer array is sorted\n");
}

void verify_floats(float *arr, size_t n){
    for(size_t i=1;i<n;i++){
        if(arr[i]<arr[i-1]){
            fprintf(stderr,"Float array is not sorted\n");
            exit(1);
        }
    }
    printf("The float array is sorted\n");
}

void verify_stability(tagged_int_t *arr, size_t n){
    for(size_t i=1;i<n;i++){
        if(arr[i].key<arr[i-1].key){
            fprintf(stderr,"Stability check failed at index %zu: %u < %u\n",i,arr[i].key,arr[i-1].key);
            exit(1);
        }
        if(arr[i].key==arr[i-1].key){
            if(arr[i].original_index<arr[i-1].original_index){
                fprintf(stderr,"Stability check failed at index %zu: %u < %u\n",i,arr[i].original_index,arr[i-1].original_index);
                exit(1);
            }
        }
    }
    printf("The stability data is stable\n");
}

double get_time_sec(){
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC,&ts);
    return ts.tv_sec + ts.tv_nsec * 1e-9;
}

uint32_t float_to_sortable(uint32_t u){
    //Convert float to uint32_t in IEEE 754 format
    uint32_t mask = (-(int32_t)(u >>31)) | 0x80000000;
    return u ^ mask;
}

uint32_t sortable_to_float(uint32_t u) {
    // Inverse operation to restore original float bits
    uint32_t mask = ((u >> 31) - 1) | 0x80000000;
    return u ^ mask;
}
