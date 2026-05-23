/*
    The Sieve of Eratosthenes
    find all prime numbers < MAX using the sieve of Eratosthenes
*/

#include <stdio.h>
#define MAX 1000

void initialize(int[]);
void mark_multiples(int[]);
void print_sieve(int[]);

int main(void)
{
    int sieve[MAX];

    initialize(sieve);
    mark_multiples(sieve);
    print_sieve(sieve);
    return 0;
}

void initialize(int sieve[])
{
    int i;

    sieve[0] = sieve[1] = 0;

    for (i = 2; i < MAX; i++)
        sieve[i] = 1;
}

void mark_multiples(int sieve[])
{
    int i, j;

    for (i = 2; i<MAX; i++)
    {
        if (sieve[i]){
            for (j = 2 * i; j < MAX; j += i)
                sieve[j] = 0;
        }
    }
}

void print_sieve(int sieve[])
{
    int i;

    printf("Prime numbers:\n");

    for (i = 2; i < MAX; i++)
    {
        if (sieve[i])
        printf("%4d", i);
    }

    printf("\n");
}