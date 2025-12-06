#ifndef HELPERS_H
#define HELPERS_H
#include <stdint.h>
#include <stddef.h>

//To keep track of original indices of integers after sorting
typedef struct {
    uint32_t key;
    uint32_t original_index;
} tagged_int_t;

//Generate data for testing
void gen_random_ints(uint32_t *arr, size_t n);
void gen_sorted_ints(uint32_t *arr, size_t n);
void gen_reverse_ints(uint32_t *arr, size_t n);
void gen_floats(float *arr, size_t n);
void gen_stability_data(tagged_int_t *arr, size_t n);

//Verifying helper functions
void verify_sorted(uint32_t *arr, size_t n);
void verify_floats(float *arr, size_t n);
void verify_stability(tagged_int_t *arr, size_t n);

double get_time_sec();

uint32_t float_to_sortable(uint32_t u);
uint32_t sortable_to_float(uint32_t u);

#endif
