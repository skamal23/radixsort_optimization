#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "helpers.h"
//This is the naive, baseline LSD radix sort implementation that sorts 32-bit integers
void sort_array(uint32_t *arr, size_t size){
    uint32_t *output = malloc(size*sizeof(uint32_t));
    if(!output){
        perror("Failed to allocate memory for output array");
        exit(1);
    }
    size_t count[256];
    for(int shift=0;shift<32;shift+=8){
        //Reset
        for(int i=0;i<256;i++){
            count[i]=0;
        }
        //Frequency count/histograms
        for(size_t i=0;i<size;i++){
            uint32_t key = (arr[i]>>shift)&0xFF;
            count[key]++;
        }
        //Prefix sum
        total=0;
        for(int i=0;i<256;i++){
            size_t temp=count[i];
            count[i]=total;
            total+=temp;
        }
        //Move data to output array -- since its random access, this should cause cache thrashing
        for(size_t i=0;i<size;i++){
            uint8_t byte=(arr[i]>>shift)&0xFF;
            output[count[byte]++]=arr[i];
        }
        //Copy output buffer back to oriignal array
        for(size_t i=0;i<size;i++){
            arr[i]=output[i];
        }
    }
    free(output);
}

int main(int argc, char **argv){
    size_t size=100000000; //100 million integers
    if(argc>1){
        size=atoi(argv[1]);
    }
    if(argc>2){
        dist_mode = argv[2];
    }
    printf("Running the baseline LSD radix sort on %zu integers\n",size);
    uint32_t *arr=malloc(size*sizeof(uint32_t));
    if(!arr){
        perror("malloc failed");
        exit(1);
    }
    printf("Generating random integers");
    if(strcmp(dist_mode,"random")==0){
        gen_random_ints(arr,size);
    }
    else if(strcmp(dist_mode,"sorted")==0){
        gen_sorted_ints(arr,size);
    }
    else if(strcmp(dist_mode,"reverse")==0){
        gen_reverse_ints(arr,size);
    }
    else if(strcmp(dist_mode,"float")==0){
        gen_floats((float*)arr,size);
        for(size_t i=0;i<size;i++){
            arr[i]=float_to_sortable(arr[i]);
        }
    }
    else if(strcmp(dist_mode,"stability")==0){
        gen_stability_data(arr,size);
    }
    else{
        fprintf(stderr,"Unknown distribution mode: %s\n",dist_mode);
        exit(1);
    }
    printf("Sorting...");
    double start_time=get_time_sec();
    sort_array(arr,size);
    double end_time=get_time_sec();
    printf("Sorting took %.4f seconds\n",end_time-start_time);
    if(strcmp(dist_mode,"float")==0){
        for(size_t i=0;i<size;i++){
            arr[i]=sortable_to_float(arr[i]);
        }
        verify_floats((float*)arr,size);
    }
    else{
        verify_sorted(arr,size);
    }
    free(arr);
    return 0;
}
