#include <math.h>
#include <stdio.h>
#include <stdlib.h>

double factorial_0();

double factorial_1(double n);

int main();

double factorial(double n)
{
    if (n == 0) {
        return factorial_0();
    } else {
        return factorial_1(n);
    }
}

