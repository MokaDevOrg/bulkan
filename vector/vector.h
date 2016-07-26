#ifndef VECTOR_H
#define VECTOR_H

/**
 * Vector - simple vector implementation including generation rules.
 * Author: rodolfocastillomateluna@gmail.com
 *
 * Example usage
 * =============
 * struct vector vector;
 * vector_init(&vector, 0, NULL);
 * vector_add(&vector, 1);
 * vector_add(&vector, 2);
 * vector_add(&vector, 3);
 *
 * double value;
 * int valid = vector_get(&vector, 1, &value); // value = 2
 * assert(valid == 1); // OK
 *
 * //////////////
 *
 * vector_clean(&vector);
 * vector_range(&vector, 1, 4);
 * size_t i;
 * for (i = 0; i < vector.length; i++) {
 *     printf("%lf\n", vector_get(&vector, i));
 * }
 */


#include <stdlib.h>
#include <string.h>

/**
 * Container structure for vector. It has an underlying
 * array to manage the data.
 *
 * In a normal vector (with no generation rule i.e.: genrule = NULL)
 * size is the real size of the vector. In a generator (i.e.:
 * genrule != NULL) size is an indicator for caching purposes, it is not
 * necessary to keep a generator's real size since it is always 2; its
 * first element is the starting condition (from where generation rule
 * proliferates) whilst its second element is the element cached at position
 * size.
 *
 * Length is the virtual size of the vector i.e. the usable one. In a generator
 * it can be used to limit its growth.
 *
 * genrule, function pointer to define generation rule behaviour. Function
 * should receive a number (current element) and return the next one.
 */
struct vector
{
	double* arr;
	size_t size;
    size_t length;
    double(*genrule)(double);
};

/**
 * Utility function.
 */
double increment_unit(double current);

/**
 * Initializes vector.
 *
 * @param vector Reference to the vector
 * @param size Initial size. If lesser than 2, initial size is 2.
 * @param genrule Rule for generator. Callback that receives
 *              current element and a reference to stop signal
 *              and returns next element. e.g.:
 *                  genrule(1, &valid)
 */
void vector_init(struct vector* vector, size_t size, double(*genrule)(double));

/**
 * Frees vector. Just a shortcut.
 */
void vector_clean(struct vector* vector);

/**
 * Initializes a vector with genrule fitted to make an inclusive range
 * [start, end].
 *
 * ATTENTION: vector must be un-initialized!
 *
 * @returns Wether it could make the range. (Basically end > start)
 */
int vector_range(struct vector* vector, int start, int end);

/**
 * Adds @param value to vector incrementing its size if necessary.
 */
int vector_add(struct vector* vector, double value);

/**
 * Sets fixed position value in vector.
 *
 * @param i Position
 * @param value The value
 * @returns Wether the value could be set
 */
int vector_set(struct vector* vector, size_t i, double value);

/**
 * Gets fixed position value in vector. Works for both, fixed vector,
 * and generated one. This function can fail if such element is out of range
 * (in case of fixed vectors) and if it is a not valid entry according to the
 * generation rule.
 *
 * @returns Value retrieved. ATTENTION: this function will not complain
 *          when an out-of-range index is requested, the value returned
 *          is garbage.
 */
double vector_get(struct vector* vector, size_t i);

/**
 * Copies a vector. The user is responsible in all circumstances on memory
 * management. i.e.: dst is expected to be an un-initialized vector.
 */
void vector_copy(const struct vector* src, struct vector* dst);

#endif