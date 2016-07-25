#include "output_prelude.h"

double factorial_0()
{
    return 1;
}

double factorial_1(double n)
{
    return (n * factorial((n - 1)));
}

int main()
{
    double n = 10;
    double x = factorial(n);
    printf("%lf! = %lf\n", n, x);
    return 0;
}

