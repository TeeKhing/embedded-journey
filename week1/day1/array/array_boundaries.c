/*
    Array Boundaries
*/

#include <stdio.h>

int main(void)
{
    int b[2];
    int a[2];
    b[0] = 1;
    b[1] = 1;
    a[0] = 1;
    a[1] = 1;
    printf("a[0] = %d, a[1] = %d, b[0] = %d, b[1] = %d\n", a[0], a[1], b[0], b[1]);

    /* unpredictable behaviour: b[2] is an invalid array location! */
    b[2] = 100;
    printf("a[0] = %d, a[1] = %d, b[0] = %d, b[1] = %d\n", a[0], a[1], b[0], b[1]);

    return 0;
}