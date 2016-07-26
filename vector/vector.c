#include "vector.h"

#include <stdio.h>

double increment_unit(double current)
{
    return current + 1;
}

void vector_init(struct vector* vector, size_t size, double(*genrule)(double))
{
    // Vectors start with at least size 2
    size = (size < 2 || genrule != NULL) ? 2 : size;
    
    // Init
    vector->arr = malloc(sizeof(double) * size);
    vector->size = (genrule != NULL) ? 0 : size;
    vector->length = 0;
    vector->genrule = genrule;
}

void vector_clean(struct vector* vector)
{
    free(vector->arr);
    vector->arr = NULL;
}

int vector_range(struct vector* vector, int start, int end)
{
    if (start < end) {
        vector_init(vector, 0, increment_unit);
        vector_set(vector, 0, start);
        vector->length = end - start + 1;
        return 1;
    }
    
    return 0;
}

int vector_add(struct vector* vector, double value)
{
    if (vector->genrule != NULL) {
        return 0;
    }
    
    if (vector->size <= vector->length) {
        vector->arr = realloc(vector->arr, vector->size +
                              vector->size / 2);
        vector->size += vector->size / 2;
    }
    
    vector->arr[vector->length++] = value;
    return 1;
}

int vector_set(struct vector* vector, size_t i, double value)
{
    if (vector->genrule != NULL && i == 0) {
        vector->arr[0] = value;
        vector->arr[1] = value;
        vector->size = 0;
    
    } else if (i < vector->length && vector->genrule == NULL) {
        vector->arr[i] = value;
        return 1;
    }
    
    return 0;
}

double vector_get(struct vector* vector, size_t i)
{
    double value;
    
    if (vector->genrule != NULL && i < vector->length) {
        size_t ff; // Fast forward
        
        if (i < vector->size) {
            value = vector->arr[0];
            ff = i;
        } else {
            value = vector->arr[1];
            ff = i - vector->size;
        }
        
        while (ff--) {
            value = vector->genrule(value);
        }
        
        vector->arr[1] = value;
        vector->size = i;
    
    } else if (i < vector->length) {
        value = vector->arr[i];
    }
    
    return value;
}

void vector_copy(const struct vector* src, struct vector* dst)
{
    dst->arr = malloc(sizeof(double) * src->size);
    dst->size = src->size;
    dst->length = src->length;
    dst->genrule = src->genrule;
    
    memcpy(dst->arr, src->arr, sizeof(double) * src->size);
}