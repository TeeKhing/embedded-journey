#include <stdio.h>
#define SIZE 5

int main(void)
{
    int a[SIZE];

    printf("\ta = %d\n\t&a[0] = %d\n", a, &a[0]);

    return 0;
}