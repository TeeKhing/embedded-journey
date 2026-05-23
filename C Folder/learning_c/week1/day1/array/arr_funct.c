/*
    read 100 integers and print them in reverse order
    use different functions to read and print the integers
*/

#include <stdio.h>
#define SIZE 5

/* function declarations */
void readnumbers(int [], int);
void printnumbers(int [], int);

int main(void)
{
    int numbers[SIZE];
    readnumbers(numbers, SIZE);     // pass array name to function
    printnumbers(numbers, SIZE);    // pass array name to function
    return 0;
}

/* function definitions */
void readnumbers(int x[], int length)   // x and numbers refer to same memory location
{
    int i;
    printf("Enter %d integers: \n", length);
    for (i = 0; i < length; i++)
    {
        printf("Enter number %3d: ", i + 1);
        scanf("%d%*c", &x[i]);
    }
}

void printnumbers(int r[], int length)  // r and numbers refer to the same memory location
{
    int i;
    printf("In reverse order: \n");

    for (i = length - 1; i >= 0; i--)
    {
        printf("%8d", r[i]);
    }

    printf(" \n");
}