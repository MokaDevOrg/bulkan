#include <stdio.h>
#include <assert.h>
#include "vector.h"

int main(int argc, char *argv[])
{
	struct vector vector;
	vector_init(&vector, 0, NULL);
	vector_add(&vector, 1);
	vector_add(&vector, 2);
	vector_add(&vector, 3);

	printf("%lf\n\n", vector_get(&vector, 1));

	vector_clean(&vector);
	vector_range(&vector, 1, 4);
	size_t i;
	for (i = 0; i < vector.length; i++) {
		printf("%lf\n", vector_get(&vector, i));
	}
}