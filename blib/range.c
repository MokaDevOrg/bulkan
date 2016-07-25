#include <assert.h>

#include "range.h"

void range_init(struct range * self, range_t lower, range_t upper)
{
	assert(lower < upper);

	self->lower = lower;
	self->upper = upper;
	self->increment = 1;

	self->_current = lower;
}

range_t range_next(struct range * self)
{
	// leave the current value exceed the upper bound by '+increment', as a way
	// to handle inclusive bound ranges.
	if (self->_current > self->upper) {
		// do not exceed the upper bound to much.
		return self->upper;
	} else {
		self->_current += self->increment;
	}

	return self->_current;
}

int32_t range_end(struct range * self)
{
	return self->_current <= self->upper;
}
