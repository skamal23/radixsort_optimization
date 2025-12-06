#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "helpers.h"

//This is the baseline LSD Radix sort
void sort_array(uint32_t *arr,size_t size,uint32_t *workspace){
    size_t count[256];
    uint32_t *output=workspace; 
    for (int shift=0;shift<32;shift+=8){
        // Reset the count array
        for (int i=0;i<256;i++){
            count[i] = 0;
        }
        // Count frequencies, sequentially 
        for (size_t i = 0; i < size; i++) {
            uint32_t key = (arr[i] >> shift) & 0xFF;
            count[key]++;
        }
        // Prefix sum
        size_t total = 0;
        for (int i=0; i<256;i++) {
            size_t temp=count[i];
            count[i]=total;
            total += temp;
        }
        
        // Scatter -- this will cause TLB misses/page walks and cache misses
        for (size_t i=0;i<size;i++) {
            uint8_t byte = (arr[i]>>shift)&0xFF;
            output[count[byte]++]=arr[i];
        }
        
        // Copy Back
        for (size_t i = 0; i < size; i++) {
            arr[i] = output[i];
        }
    }
}

int main(int argc, char **argv) {
    size_t size = 100000000; 
    char *dist_mode = "random";
    if (argc > 1){
        size = atol(argv[1]);
    }   
    if (argc > 2){
        dist_mode = argv[2];
    }

    printf("Running baseline LSD Radix sort, N=%zu\n", size);
    uint32_t *arr = malloc(size*sizeof(uint32_t));
    if (!arr){
        perror("malloc arr");
        exit(1);
    }
    uint32_t *workspace = malloc(size*sizeof(uint32_t));
    if (!workspace){
        perror("malloc workspace");
        exit(1);
    }
    printf("Generating dataset\n");
    if (strcmp(dist_mode, "random") == 0){
        gen_random_ints(arr, size); 
    } 
    else if (strcmp(dist_mode, "sorted") == 0){
        gen_sorted_ints(arr, size);
    } 
    else if (strcmp(dist_mode, "reverse") == 0){
        gen_reverse_ints(arr, size);
    } 
    else if (strcmp(dist_mode, "float") == 0){
        gen_floats((float*)arr, size);
        for (size_t i = 0; i < size; i++) arr[i] = float_to_sortable(arr[i]);
    }
    else {
        fprintf(stderr, "Unknown dist: %s\n", dist_mode); exit(1);
    }

    printf("Sorting dataset\n");
    double start_time = get_time_sec();     
    __asm__ volatile("" ::: "memory");    
    
    sort_array(arr, size, workspace);      
    
    __asm__ volatile("" ::: "memory");
    double end_time = get_time_sec();
    
    printf("Sorting took %.4f seconds\n", end_time - start_time);

    if (strcmp(dist_mode, "float")== 0){
        for (size_t i = 0; i < size; i++){
            arr[i] = sortable_to_float(arr[i]);
        }
        verify_floats((float*)arr, size);
    } 
    else {
        verify_sorted(arr, size);
    }

    free(arr);
    free(workspace);
    return 0;
}
