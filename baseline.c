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
