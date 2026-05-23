/*
    merging arrays
*/

#include <stdio.h>
#define SIZE 5

void ReadArray(int[], int);
void PrintArray(int[], int[], int);

int main(void)
{

    int a[SIZE];
    int b[SIZE];

    ReadArray(a, SIZE);
    ReadArray(b, SIZE);
    PrintArray(a, b, SIZE);

    return 0;
}

void ReadArray(int x[], int length)
{
    int i;

    printf("Enter %d integer numbers:\n", length);

    for (i = 0; i < length; i++){
        printf("Enter number %3d: ", i + 1);
        scanf("%d%*c", &x[i]);
    }
}

void PrintArray(int a[], int b[], int length)
{
    int i, j;

    i = 0;
    j = 0;

    printf("\nMerging the 2 lists results in:\n");

    while (i < length && j < length)
    {
        if (a[i] < b[j])
        {
            printf("%d ", a[i]);
            ++i;
        }
        else{
            printf("%d ", b[j]);
            ++j;
        }
    }
    
    while (i < length)
    {
        printf("%d ", a[i]);
        ++i;
    }

    while (j < length)
    {
        printf("%d ", b[j]);
        ++j;
    }

    printf("\n");
}