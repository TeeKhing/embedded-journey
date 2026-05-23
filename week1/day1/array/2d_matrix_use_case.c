#include <stdio.h>
#define MAX_ROWS 5
#define MAX_COLUMNS 2

int main(void)
{
    float marks[MAX_ROWS][MAX_COLUMNS];
    int row, column;

    // loop through rows
    for (row = 0; row < MAX_ROWS; row++)
    {
        // loop through all columns in one row
        for (column = 0; column < MAX_COLUMNS; column++)
        {
            printf("Enter marks for student %d, subject %d: ", row+1, column+1);
            scanf("%f%*c", &marks[row][column]);
        }
    }
    return 0;
}