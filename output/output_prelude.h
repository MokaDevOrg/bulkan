#include <math.h>
#include <stdio.h>
#include <stdlib.h>

double fib_0(double n);

double fib_1();

double fib_2();

int main();

double fib(double n)
{
    if (n == 1) {
        return fib_1();
    } else     if (n == 0) {
        return fib_2();
    } else {
        return fib_0(n);
    }
}

