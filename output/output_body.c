#include "output_prelude.h"

double fib_0(double n)
{
    return (fib((n - 2)) + fib((n - 1)));
}

double fib_1()
{
    return 1;
}

double fib_2()
{
    return 0;
}

int main()
{
    double n = 10;
    double x = fib(n);
    printf("the %lf-nth fib is: %lf\n", n, x);
    return 0;
}

