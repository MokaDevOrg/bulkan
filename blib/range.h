/**
 * Range - inclusive range between two constants.
 * Author: marjara35@gmail.com
 * 
 * Example usage
 * =============
 * struct range r;
 * range_init(&r, 0, 100);
 * for (int i = r.lower; range_end(&r); i = range_next(&r)) {
 *     printf("%d\n", i);
 * }
 */

#pragma once

#include <stdint.h>

/**
 * Type of all range values.
 */
typedef int32_t range_t;

/**
 * Main type used to work with ranges.
 */
struct range
{
	range_t lower;		/**< Lower bound */
	range_t upper;		/**< Upper bound */
	range_t _current;	/**< Current value, it's not safe to use, it's meant to be private. */
	range_t increment;	/**< Increment of each iteration */
};

/**
 * Initialize a range object with its bounds and an increment of 1.
 */
void range_init(struct range * self, range_t lower, range_t upper);

/**
 * Increments the current value of the range and returns the result.
 * This is the only safe way of getting the current value of the range.
 *
 * @return The current value of the range.
 */
range_t range_next(struct range * self);

/**
 * Checks if the range has ended.
 *
 * @return 0 if the range hasn't ended, 1 otherwise.
 */
int32_t range_end(struct range * self);
