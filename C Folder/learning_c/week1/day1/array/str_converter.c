/*
    convert lowercase letters into uppercase letters
*/

#include <stdio.h>
void convert (char[], char[]);

int main(void)
{
    char in[32];
    char out[32];
    printf("Enter a string: ");
    gets(in);
    convert(in, out);
    printf("in = %s \nout = %s \n", in, out);
    return 0;
}

void convert(char in[], char out[])
{
    int i = 0;
    while ((out[i] = in[i]) != '\0')
    {
        if (out[i] >= 'a' && out[i] <= 'z') out[i] += 'A' - 'a';
            i++;
    }
}