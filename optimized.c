#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <omp.h>
#include <sched.h>
#include "helpers.h"

//Process in 8MB blocks to take advantage of L3 cache.
#define L2_CHUNK (2048 * 1024)

//We pin thread to core to prevent migrating threads to different cores and cold caches
void pin_thread_to_core(int id) {
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(id, &cpuset);
    sched_setaffinity(0, sizeof(cpu_set_t), &cpuset);
}

void sort_array(uint32_t *arr,size_t size,uint32_t *workspace) {
    uint32_t *in = arr;
    uint32_t *out = workspace; 
    size_t num_blocks = (size+L2_CHUNK-1)/L2_CHUNK;
    
    //We use a two pass approach: first pass to count frequencies, second pass to scatter
    size_t (*block_offsets)[256] = malloc(num_blocks*256*sizeof(size_t));
    if (!block_offsets) { 
        perror("malloc meta"); exit(1); 
    }

    #pragma omp parallel
    pin_thread_to_core(omp_get_thread_num());

    for (int shift=0; shift<32; shift+=8) {
        
        #pragma omp parallel for schedule(static)
        for (size_t b=0;b<num_blocks;b++){
            memset(block_offsets[b], 0, 256 * sizeof(size_t));
            size_t start=b*L2_CHUNK;
            size_t end =start+L2_CHUNK;
            if (end>size){
                end=size;
            }
            for (size_t i=start;i<end;i++){
                block_offsets[b][(in[i]>>shift)&0xFF]++;
            }
        }
        size_t total = 0;
        for (int k=0; k<256; k++){
            for (size_t b=0;b<num_blocks; b++){
                size_t count=block_offsets[b][k];
                block_offsets[b][k]=total;
                total+=count;
            }
        }
        #pragma omp parallel for schedule(static)
        for (size_t b=0;b<num_blocks;b++) {
            size_t start = b*L2_CHUNK;
            size_t end = start+L2_CHUNK;
            if (end > size){
                end = size;
            }
            size_t *offsets=block_offsets[b];
            for (size_t i=start; i<end;i++) {
                out[offsets[(in[i]>>shift) & 0xFF]++]=in[i];
            }
        }
        uint32_t *tmp = in; in=out; out=tmp;
    }

    if (in != arr){
        memcpy(arr, in, size*sizeof(uint32_t));
    }
    free(block_offsets);
}

int main(int argc, char **argv) {
    size_t size = 100000000; 
    char *dist = "random";
    int threads = 16; 

    if (argc > 1){
        size = atol(argv[1]);
    }
    if (argc > 2){
        dist = argv[2];
    }
    if (argc > 3){
        threads = atoi(argv[3]);
    }

    omp_set_num_threads(threads);
    printf("Running optimized LSD Radix sort, N=%zu, Dist=%s, Threads=%d\n", size, dist, threads);
    
    uint32_t *arr = malloc(size * sizeof(uint32_t));
    if (!arr) { perror("malloc arr");
        exit(1);
    }

    uint32_t *workspace = malloc(size * sizeof(uint32_t));
    if (!workspace) { perror("malloc workspace");
        exit(1);
    }
    
    printf("Generating dataset\n");
    if (!strcmp(dist, "random")) gen_random_ints(arr, size);
    else if (strcmp(dist, "sorted") == 0){
        gen_sorted_ints(arr, size);
    }
    else if (strcmp(dist, "reverse") == 0){
        gen_reverse_ints(arr, size);
    }
    else if (strcmp(dist, "float") == 0){
        gen_floats((float*)arr, size);
        for (size_t i = 0; i < size; i++){
            arr[i] = float_to_sortable(arr[i]);
        }
    }
    else {
        fprintf(stderr, "Unknown dist: %s\n", dist);
        exit(1);
    }

    printf("Sorting dataset\n");
    double start_time = get_time_sec();     
    __asm__ volatile("" ::: "memory");    
    
    sort_array(arr, size, workspace);      
    
    __asm__ volatile("" ::: "memory");
    double end_time = get_time_sec();
    
    printf("Sorting took %.4f seconds\n", end_time - start_time);

    printf("Verifying...\n");
    if (!strcmp(dist, "float")) {
        #pragma omp parallel for
        for (size_t i = 0; i < size; i++) arr[i] = sortable_to_float(arr[i]);
        verify_floats((float*)arr, size);
    } else {
        verify_sorted(arr, size);
    }

    free(arr);
    free(workspace);
    return 0;
}
