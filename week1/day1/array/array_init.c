#include <stdio.h>
#define SIZE 100

int main(void)
{
    int numbers[SIZE];
    int i;

    printf("Enter %d integers: \n", SIZE);
    // READ THE INTEGERS ONE BY ONE IN A FOR LOOP
    for (i = 0; i < SIZE; i++) 
    {
        printf("Enter integer %3d: ", i + 1);
        scanf("%d%*c", &numbers[i]);
    }
    
    printf("In reverse order: \n");
    // print the array elements starting from the last one down
    for (i = SIZE - 1; i >= 0; i--)
    {
        printf("%8d", numbers[i]);
    }

    printf(" \n");
    return 0;
}