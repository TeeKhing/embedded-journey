#include <stdio.h>

/*
int main(void)
{
    int temperature = 25;
    int *ptr = &temperature;
    printf("%d\n", *ptr);
    return 0;
}
*/

/*
Pointers are used:
1.  to access hardware registers 
    directly. 
    e.g. - #define PORTB (*(volatile uint8_t*)0x25) 
            means "treat address 0x25 as a pointer to a byte, and dereference it immediately."

2.  to pass large data without copying. Copying large data 
    by passing a function consumes memory space, but with
    a pointer, space is conserved while data is retained.

3.  for dynamic data structures.
*/