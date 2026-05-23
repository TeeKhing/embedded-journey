#include <stdio.h>

int main(void)
{
    int temperature = 25;
    int humidity = 60;

    int *ptr;   // 'int *' here is means pointer to int, a type modifier.

    ptr = &temperature; // ptr stores the address of temperature because of '&'.
    printf("Address of temperature : %p\n", (void *)ptr);   // '(void *)ptr' points directly to the literal hardware address.
    printf("Value at that address: %d\n", *ptr);
    ptr = &humidity; // ptr stores the address of humidity because of '&'.
    printf("Address of humidity: %p\n", (void *)ptr);   // '(void *)ptr' points directly to the literal hardware address.
    printf("Value at that address: %d\n", *ptr);

    *ptr = 75;  // '*' here is a dereference operator. This line means "go to the address stored in ptr and update the value.
    printf("humidity after *ptr=75: %d\n", humidity);

    return 0;
}