/*
    Temperature details
*/


#include <stdio.h>
/*
#define DAYS 7

void take_data(int[], int);
void find_mean(int [], int);
void greater_than_10(int [], int);

int main(void)
{
    int a[DAYS];
    
    take_data(a, DAYS);
    find_mean(a, DAYS);
    greater_than_10(a, DAYS);

    return 0;
}

void take_data(int x[], int length)
{
    int i = 0;

    while (i < length)
    {
        printf("Enter temperature for day %d: ", i);
        scanf("%d%*c", &x[i]);
        ++i;
    }
}

void find_mean(int y[], int length)
{
    int i, sum;
    float mean;

    i = 0;
    sum = 0;

    for (i = 0; i < length; i++)
        sum += y[i];
    mean = (float)sum / length;

    printf("\nThe mean temperature for this week is %.2f\n", mean);
}

void greater_than_10(int z[], int length)
{
    int i;

    printf("\nAll days with a temperature > 10⁰C: ");
    for (i = 0; i < length; i++)
    {
        if (z[i] > 10)
        {
            printf("\nday %d", i);
        }
    }
}
*/

#define MAX 100

void ReadSize(int, int);
void ReadArray(int [], int);
void PrintArray(int [], int);

int main(void)
{
    int row[MAX];
    int size;
    size = ReadSize(0, MAX);
    ReadArray(row, size);
    PrintArray(row, size);
    return 0;
}

int ReadSize(int x, int y)
{
    
}

void ReadArray(int a[], int length)
{

}

void PrintArray(int a[], int length)
{

}