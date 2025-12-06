#ifndef HELPERS_H
#define HELPERS_H
#include <stdint.h>
#include <stddef.h>

//To keep track of original indices of integers after sorting
typedef struct {
    uint32_t key;
    uint32_t original_index;
} tagged_int_t;


